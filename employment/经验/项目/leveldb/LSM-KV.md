# LSM-KV(Log Structure Merge Key-Value storage)

> 本项目预计基于LSM结构实现一个kv存储引擎。预计要实现：

- [x] 日志
- [x] 布隆过滤器
- [x] 内存分配器
- [x] cache
- [x] 文件读写
- [x] sstable
- [x] WAL
- [x] memtable
- [x] db
- [x] sst_parser
- [x] test

有待改进

- [ ] Skiplist优化，保障线程安全
  skiplist: Insert可优化为 `std::vector<Node *> prev(GetCurrentHeight, nullptr);`
- [ ] datablock写满后，进行异步持久化，目前是同步
- [ ] filter_block中存在可优化点
- [ ] allocate可以优化



[TOC]



内存分配 memory

缓存 cache

日志 log

文件读写和WAL 

memtable和skiplist table和memtable

数据库接口 db

压测



## 日志

> 实现方案：利用单例模式实现日志。

- [单例模式的实现方式](https://blog.csdn.net/unonoi/article/details/121138176)；
- [spdlog - C++日志库](https://blog.xiyoulinux.com/blog/104106245)；
- [lock_guard and unique_ptr](https://www.cnblogs.com/linuxAndMcu/p/14576646.html);

参考的库是典型的**c++日志库`spdlog`**，它使用内存映射文件和异步日志记录技术，能够快速记录；支持多线程，能够保障线程安全（手段为互斥锁）；具有多种日志级别，采取了灵活的日志格式化选项，支持跨平台，多后端。

对于`spdlog::logger`加上**共享指针`shared_ptr`**，便携化资源释放。

对于创建一个日志单例采取的是最为经典的**静态局部变量的懒汉单例**，`static`既能保证共享性下只存在一个实例，也能保证变量的创建不被打乱，相比于双重锁和智能指针实现的更完善也更简单。

## 布隆过滤器

> 和leveldb中的实现大致类似。
>
> 利用googletest进行单元测试

相比于`levelDB`中根据键的位数确立哈希函数个数，这里采用**键的数量和假阳性率**直接确立最佳的**哈希函数数量和位数组大小**，这是一篇讲解`bloom filter`中的结论得到的。

创建过滤器时，会通过键和**双重哈希增量**模拟每次的哈希函数，哈希的方法采用的是murmur_hash方法，最终存储到相应的位中。

同时，判断是否存在其中也是对齐进行哈希计算，只要判断出一次不存在那就一定不存在与数组中，但是全部存在也不能证明一定在数组中。

## 内存分配器

> 它只会为小于4kb的内存块分配内存，
>
> - 内存池：一大块预分配的内存区域
>
> - 内存槽 + 内存块：对内存池进行组织和分类内存池中大小不同内存块的工具；
>
>   内存槽的核心在于记录和组织内存池中的空闲内存块
>
> ```cpp
>     private:
>         std::array<BlockNode *, SLOT_NUM> memory_slot;
>         char *mem_pool_start = nullptr;
>         int32_t mem_pool_size = 0;
> ```

内存池总共是4MB

组成：slot1(8字节/块)，slot2(16字节/块)，…，slot512(4096字节/块)

每个sloti指向的是该链表第一个空闲块

### 构造和析构

### 公共接口：allocate，deallocate，reallocate

- allocate：满足大于0小于4kb时，找到相应的内存槽填入内存槽，内存槽指针指向下一个内存块。
- deallocate：满足大于0小于4kb时，找到相应的内存槽，将该内存加入其中，内存槽指针指向新加入的位置，因为一般是头插法。
- reallocate：先deallocate，再allocate

### 私有函数：fill_slot，fill_mem_pool

- 第二个函数用于分配新的4MB的内存池。
- 第一个函数用于填内存槽，也就是当需要分配空间但是内存槽中没有相应字节的内存管理时，寻找到对应的sloti，一次性分配`FILL_BLOCK_CNT`也就是10个块。填内存槽时会出现三种情况：
  - 内存池容量足够分配所有块，直接分配
  - 内存池容量只够分配一部分块，先分配一部分块
  - 内存池容量一块也不够，剩余部分挂在到slot上，重新申请内存池（内存池会修改指针和大小，但是slot作为某种形式的数组/链表，是始终存在的，挂载也可以避免内存碎片；之后重新运行该函数，继续分配。

## Cache

**核心**是实现分片的LRU缓存

**Cache中持有N（默认为5）个指向CachePolicy的指针，相当于5个分片，可以减少哈希冲突以及减少锁的范围；LRUCache和LFUCache都是CachePolicy的子类。**

> 为什么使用LRU缓存，LFU呢，其它缓存为什么不行？

- LRU:淘汰最久未被访问的数据项
- LFU:淘汰访问频率最低的缓存项

LRU实现简单,并且更加适合具有局部性原理的访问模式.

> leveldb中为什么存在lru链表和in-use链表，两个链表？

一个存放lru中的结点,另一个存放被lru淘汰的结点,结点被淘汰了但是不一定没有被其它引用,贸然删除可能造成不好的结果,每隔一段时间,遍历in-use链表,如果引用计数变为0,将其删除.

> 两个哈希函数的作用；回调函数的作用。

这两个哈希和leveldb中两个链表是一样的作用.

回调函数: **在缓存条目被删除或替换时，执行一些清理操作**，通常是在缓存项被淘汰、移除或替换后进行特定的资源释放或额外的操作。

```cpp
std::unordered_map<K, typename std::std::list<Node<K, V> *>::iterator> index; // 保存键到结点的映射
/*
 * 用于跟踪当前正在被使用的缓存条目
 * 确保这些条目不会被替换。这有助于提高性能，因为活跃的条目通常是程序当前需要的数据。
  通过将活跃条目与不活跃条目分开管理，可以减少对整个缓存的锁竞争
*/
std::unordered_map<K, Node<K, V> *> garbage_station;  // 待删除列表，从index利用LRU策略删除后记录在此处
std::function<void(const K &key, V *val)> destructor; // 回调函数
```

[lrucache-leetcode](https://leetcode.cn/problems/lru-cache/) 另：锁的设计

## 文件读写

实现基本的文件读写功能。

### `file_write`

主要用于对`WAL Write-Ahead Log`文件和`Sorted String Table`的持久化操作，实现关注于高效的文件写入，缓冲区，内存映射文件等技术，优化文件写入。

- 将文件中的数据从用户空间写到文件描述符fd指向的文件，
- 将fd指向文件的缓冲数据同步到磁盘中，

在缓冲区中追加数据时，需要考虑到缓冲区内存是否可以容纳所有的数据，如果可以就直接写入，不行的话先将缓冲区中的数据刷新到磁盘再写入。

### `file_read`

主要用于打开文件，从缓冲区中读取数据，注意实现多线程环境下的并发读取。

## SSTable

在leveldb中，当将`memory db`的数据持久化文件中时，`leveldb`会以一定的规则进行文件组织，文件格式变为`sstable`。这个部分也模仿一下`leveldb`，那先回顾一下`leveldb`中的结构。见[leveldbS源码阅读3 - File System (sstable,cache,option)](https://zhuanlan.zhihu.com/p/812053605)，[详谈leveldb中的sstable](https://zhuanlan.zhihu.com/p/7615608552)

查询时，一般会通过`footer`先在`meta block`中利用`bloom filter`查询，如果不存在则直接返回，减少了磁盘IO，然后再通过`Index block`找到之后对于相应的`Data block`即可。（通过 `Footer -> IndexBlock -> DataBlock` 的多级索引方式）

> 有个问题，sstable中的地址是footer最小吗，如果是的话，那怎么存放datablock的，一个sstable是一整个完整的连续空间吗？

SSTable 是一个**完整的、连续的磁盘文件**。Footer 是整个 SSTable 文件的最后一个固定部分，不是存储的最小地址。Footer 的大小是**固定的**，程序可以**通过文件总大小减去 Footer 大小**快速找到 Footer 的起始位置。

### Data Block

> 存储key，value数据对
>
> DataBlock_1 ~ DataBlock_N：即DataBlock，由DataBLockBuilder操作

```sql
+-----------------------------+
| Record_1 - Record_n        |  <-- n条记录，每条记录是一个键值对
+-----------------------------+
| Restart Point_1 - ..._k    |  <-- k个重启点，从该位置开始一组记录
+-----------------------------+
| Restart_Num(4B)            |  <-- 重启点数量
+-----------------------------+
| Restart_Offset(8B)         |  <-- 重启点数组的起始偏移量
+-----------------------------+
```

`datablock`中主要需要存储数据，为了最有效的存储数据，我们在类的`private`中设置了`pre_key`，这是为了比较前缀，只存储相同的数据，在它的记录中，它的存储格式为`shared_key_len,unshared_key_len,value_len,unshared_key_conten,value_content`，同时为了查找时的效率加快，每相隔16条记录会设置一个重启点，同时利用一个数组记录重启点数组每次的偏移量；

由于偏移量的大小只能写完16条才能记录，不能实时，同时为了防止多次进行磁盘IO，因此每个数据块都是先存放在缓冲区中，等到最后满时创建数据块，创建之后就不能再添加数据了。

### Meta Block

> 存储`filter`相关信息
>
> MetaBlock：存放Filter等信息，这里每个SST只设置一个MetaBlock

`filter_block`和`datablock`的机制差不多，更简单一点，基本调用`bloomfilter`中的函数，创建过滤器，添加键，判断键是否存在之类的。

### Index Block

> 存储每个`data block`的索引信息
>
> IndexBlock_1 ~ IndexBlock_N：存放对应的DataBLock的Offset信息、最大Key信息

```sql
 +------------------------+---------------+-----------------+
 | _shortest_key_size(4B) | _shortest_key | _offsetInfo(8B) |
 +------------------------+---------------+-----------------+
```

### Footer

> 存储`meta index block`和`index block`的索引信息
>
> Footer：存放MetaBlock、IndexBlock的Offset信息

```sql
+---------------------------+----------------------------+
| MetaBlock_OffsetInfo (8B) | IndexBlock_OffsetInfo (8B) |
+---------------------------+----------------------------+
```

## WAL

顾名思义，`Write-Ahead Logging`先写日志，再更新数据。

> levelDB中的文件读写和WAL都是在log_writer和log_reader中实现的。leveldb中的结构为 
>
> ```sql
> +--------------+---------+----------+------+
> | checksum(4B) | len(2B) | type(1B) | data |
> +--------------+---------+----------+------+
> ```
>
> checksum是校验数据的准确性；len为长度；type为类型，即如果一条数据在一个block中存放不下，这里会例句该block是数据的前面，中间或者后面部分；data为数据。
>
> 这里将WAL和文件读写分开实现。

`WAL`和文件读写分开实现有助于遵循单一职责原则和依赖注入原则。

> q1：WAL如何利用filewrite作为私有变量实现的

利用`filewriter`作为私有成员，利用了crc循环冗余校验值存储，同时保留长度，作为wal读写。

## Memtable

> memtable就是一个在内存中进行数据组织与维护的结构。memtable中，所有的数据按**用户定义的排序方法**排序之后按序存储，等到其存储内容的容量达到阈值时（默认为4MB），便将其转换成一个**不可修改**的memtable，与此同时创建一个新的memtable，供用户继续进行读写操作。memtable底层使用了一种[跳表skiplist](https://zh.wikipedia.org/wiki/跳跃列表)数据结构，这种数据结构效率可以比拟二叉查找树，绝大多数操作的时间复杂度为O(log n)。

主要是实现`memtable`和`skiplist`，同样可以参考leveldb，见我曾经写过的一篇文章:[Leveldb源码阅读2 - memtable(skiplist,arena,random) - 知乎](https://zhuanlan.zhihu.com/p/812023477)

### SkipList

> 需要详细描述一下这里实现的skiplist，还有很大的改进空间

## 参考

1. [leveldb - google](https://github.com/google/leveldb)
2. [Leveldb源码阅读 - 知乎](https://zhuanlan.zhihu.com/p/811970982)