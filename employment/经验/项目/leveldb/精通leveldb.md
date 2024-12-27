

### chapter2 基本数据结构

##### 1.slice中的私有结构其实就是一个`size`和一个`const char*`的常量指针，那为什么不用`string`(直接封装了`size()`)或者用`const char*`。

在`leveldb`这样的高性能数据库中，非常注重良好的性能。因此自己主动设计了很多不依赖外部库自己实现的组件。`slice`主要在于操作指针防止进行不必要的拷贝。

比如

###### slice（string）

###### arena（内存池）

###### snappy（压缩）

###### random（随机数）

###### 线程池`threadpool`

###### 日志WAL



##### 2.错误处理`status`。其中的状态码采用的是`enum`，C++11中对其做出改进，请在源码中修改并且详细说明。

[`enum`和`enum class`](https://zhuanlan.zhihu.com/p/501309032)

`enum class`更加安全和强类型化。

- 枚举值的名称在枚举类的作用域是局部的，不会发生命名冲突，并且只能通过`::`作用域解析符访问。
- 底层可以是任何整数类型，不能隐式转换成底层类型。



##### 3.comparator比较函数接口

###### `BytewiseComparatorImpl`

- 按照字典序对字符串进行对比

###### `InternalKetComparator`(这其中两个函数有什么作用)

- `FindShortestSeparator（start，limit）`是获得大于start但小于limit的最小值。
- `FindShortSuccessor（start）`是获得比`start`大的最小值。比较都基于`user-commparator`，二者会被用来确定`sstable`中`block`的`end-key`。

> （可用于压缩数据）
>
> **`FindShortestSeparator`** 主要用于生成最短的分隔符键，优化存储结构。
>
> **`FindShortSuccessor`** 则用于查找给定键的下一个最小键，提升查找效率。



##### 4.`FindShortestSeparator`会用于插入，`FindShortSuccessor`*会用于查找，请问分别是如何提高效率的。



##### 5.一个key，value，sequence都分别什么用