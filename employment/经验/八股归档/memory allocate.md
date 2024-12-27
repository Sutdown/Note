# Memory allocate

> 因为最近想基于LSM实现一个简单的`kv`存储系统，现在在想如何实现其中的内存分配机制，刚好打算联系一下之前学过的项目，比较一下各种内存分配机制，最终决定一个实现方案。
>
> 打算涉及的项目：
>
> `levelDB`, `mytinystl`, `tinywebserver`



`c++`的很多项目都挺喜欢自己去实现内存分配机制的，毕竟针对不同项目，合适才是最重要的。

**内存分配机制**是计算机系统中如何分配和管理内存的策略。内存分配主要是为了提高内存的利用率，减少内存碎片，提升性能，保证程序的正确性。最为常见的内存分配机制有`linux`中的静态内存分配，堆分配内存，栈内存分配，内存池，内存映射，垃圾回收，`stl`中的`allocator`等。接下来我讲讲我在项目源码中所见过的。



## `levelDB`中的`Arena`

它的私有函数：

```cpp
  // Allocation state
  char* alloc_ptr_; // 当前可分配指针
  size_t alloc_bytes_remaining_; // 当前分配块的剩余字节

  // Array of new[] allocated memory blocks 以及分配内存块的数组
  std::vector<char*> blocks_;
```

- 所需内存大于4kb时，直接用`malloc/new`；
- 所需内存小于4kb时，
  - 内存比较大时，分配特定大小然后传入数组
  - 内存比较小时，为了避免内存碎片，会直接分配一个新的块，然后更新剩余内存和指针位置

