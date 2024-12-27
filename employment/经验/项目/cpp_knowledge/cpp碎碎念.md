### 唠嗑c++知识1



**探究c++的过程希望能围绕cpp语言的目标展开。**

**C++ 语言的最终目标是：**（来源现代c++白皮书）

- 使用和学习上都要比 C 或当前的 C++ 容易得多
- 完全**类型**安全——没有隐式类型违规，没有悬空指针
- 完全**资源**安全——没有泄漏，不需要垃圾收集器
- 为其构建工具要相对简单——不要有宏
- 跟当前 C++ 一样快或更快——零开销原则
- 性能可预测——适用于嵌入式系统
- 表达力不亚于当前的 C++——很好地处理硬件



#### `c`和`cpp`

**首先提出第一个问题，`cpp`相对c，`class`相对于`struct`，`cpp`的核心优势在哪里，我们通常所说的面向对象编程和面向过程编程思想是体现在什么知识点上。**



#### 虚函数

**虚函数的机制是什么，class的机制是什么，在底层都是如何实现出来的**

推荐文章：[虚函数 - 果冻虾仁](https://www.zhihu.com/question/389546003/answer/1194780618?utm_psn=1832213938915381248)

关于上面两点，以上链接的回答算得上非常好了。c++和c最大的区别在于一个是面向对象，一个面向过程，但是同时也要注意，c++的演化过程的初始就是C with class。（留个点位，我觉得我应该去看看**C++的设计和演化**这本书，应该能对c++有个更好的了解）在上面那个知乎的回答中也提到，虽然写法是面向对象的，其实在编译之后仍然是面向过程的。（推荐看**程序员的自我修养**这本书）

class相比struct，类中具有相应函数的指针，在编译时期编译器将函数的地址确定下来运行时可以直接调用，这也就是我们常说的**静态绑定**。静态绑定和**重载**相关，通一作用域内多个重名函数，因为参数列表不同，编译器会在编译时根据调用参数类型和数量选择合适的函数。

有静态绑定必然也存在**动态绑定**，此时就是**虚函数**大显身手的时候了，同时也是在实现c++三大特性之一的**多态**。动态绑定也成为延迟绑定，也就是运行时才确定函数地址，实现的关键在于虚表。

在含有虚函数的类编译期间，每个类中都会有四个字节去存储一个**虚表指针**，虚表指针指向一个虚表，虚表存储了实际的函数地址，虚表指针会直接指向虚函数的位置。当发生**继承**时，父类和子类的虚表位置基本上处于一个连续的内存空间，子类没有重写父类的函数时，会直接指向父类函数所在的地址。

讲都讲到这了，联系一下modern c++，

- **final/override**：（主要是优化性能）

  `final`：使派生类不可覆盖它修饰的虚函数。当然虚函数中也可也以使用final，但是这样子虚函数无法重载，就失去了虚函数的意义，若不想虚函数被重载，完全可以直接将该成员函数定义为非虚函数即可。

  `override`：如果派生类在虚函数声明时使用了override描述符，那么该函数必须重载基类中的同名函数，否则代码无法通过编译。

- **assert/static_assert：**这两也是一个静态断言，一个动态断言。

  模板实例化时采用静态断言比较合适，也就是编译时期产生断言。



#### **讲讲inline**

> 我初学c++时是搞不太清它的用法的，只是依稀记得经常出现多重定义的错误，貌似是用这个解决。最近看到些面试题所问的inline和我记忆里的用法大相径庭。于是开始在网上遍寻文章，这篇实在太nice了，都去看！
>
> [C++ inline - 吼姆桑](https://www.zhihu.com/question/24185638/answer/2404153835?utm_psn=1832233607923650561)

C++98之前：建议编译器将函数内联展开至调用处；然而现代编译器对于内联展开几乎不再参考inline；C++11之后，能够修饰函数，变量；C++17之后，甚至能修饰命名空间；此外，**inline**更为关键的作用是**允许同一个函数或变量的定义出现在多个编译单元**之中；最后，修饰命名空间的inline关键字为程序库的**版本控制**提供了语言层面上的支持，这与最初的函数内联的含义更是相去甚远。

inline的实际含义，就正式的成为了「允许函数在多个编译单元中存在」了



#### 智能指针，读写锁

**今天看到一段有关智能指针，读写锁的话：**

> C++11引入`shared_ptr`和`unique_ptr`
>
> C++11引入`make_unique`,C++14引入`mutex`模块,`make_shared`
>
> C++17引入`shared_mutex`读写锁，`lock_guard`

**引入智能指针好理解，利用RALL机制把指针进一步封装，减少指针使用的错误。然后make_shared和make_unique则是避免了直接对指针的使用，从而将其封装成对象，那对象相对于智能指针增加了什么？读写锁也是利用了RALL机制，其中的各种锁的使用有什么差别？ **

C++11中智能指针`unique_ptr`，`shared_ptr`，`weak_ptr`的出现代替了C++98的模板类型`auto_ptr`。核心思想是`RALL`机制，即`Resource Acquisition Is Intialization`，也就是将资源的生命周期和对象的生命周期绑定，构造期间获取资源，析构时释放资源，实现资源的自动管理，减少内存泄露的风险。



**`shared_ptr`**

采用引用计数机制，能够记录多少个`shared_ptr`共同指向一个对象，从而消除显示调用`delete`，引用计数为0时对象自动删除。

它的内部维护了一个引用计数，两个指针成员，一个指针指向管理的数据的地址；另一个指针指向控制块的地址

```cpp
public:
    typedef _Tp element_type;

private:
    element_type*	   _M_ptr;         // 原生指针
    __shared_weak_count* __cntrl_;    // 控制块指针  
```



**`unique_ptr`**

`std::unique_ptr` 是一种独占的智能指针，它禁止其他智能指针与其共享同一个对象，从而保证代码的安全。

唯一的数据成员就是原生指针和析构器的 pair。

```cpp
public:
  typedef _Tp element_type; // 原生指针类型
  typedef _Dp deleter_type; // 析构器
  typedef typename __pointer_type<_Tp, deleter_type>::type pointer;

private:
  __compressed_pair<pointer, deleter_type> __ptr_;
```



**`weak_ptr`**

解决`shared_ptr`循环引用的问题，它可以指向`Shared_ptr`指向的对象内存，但是并不拥有该内存，而使用`weak_ptr`的成员`lock()`，就可以返回指向内存的一个`shared_ptr`对象，并且在所指对象内存无效时，会返回指针空值。



鉴于智能指针是存储“指向动态分配（在堆上）的对象的指针”的对象，创建对象时，基于以下思想

> 来源（[Sutter’s Mill](https://herbsutter.com/)）：
>
> - 希望利用从同一线程调用的函数不会交错的事实，希望提供一个函数`make_shared`和`make_unique`原子性的完成对象的分配和构造以及`shared_ptr`和`unique_ptr`的构造工作。
> - 因为该函数应该能和任何类型一起使用，因此标识为模板。
> - 调用者想要`make_xxx`能够从外部传递构造函数参数，因此利用C++11的完美转发将参数传递给内部的`new`表达式。

对于`make_unique`

```cpp
template<typename T, typename ...Args>
std::unique_ptr<T> make_unique( Args&& ...args )
{
    return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
}
```



参考资料：

1.[现代c++教程](https://changkun.de/modern-cpp/zh-cn/05-pointers/)

2.[异常安全函数调用 - Sutter’s Mill](https://herbsutter.com/gotw/_102/)

3.[现代C++智能指针详解：原理、应用和陷阱](https://www.cnblogs.com/qiangz/p/17911186.html)

4.[C++ 智能指针最佳实践&源码分析](https://zhuanlan.zhihu.com/p/436290273)



#### bind

**你了解bind吗，为什么在Effective modern c++中说优先选择lambda表达式，而不是std::bind**



注：

除上面优先选择lambda表达式，而不是std::bind还有：

- 优先使用`nullptr`，而不是`0`或者`NULL`（NULL实际上就是#define NULL 0，而且利用的还是define预处理指令，0其实是整数，每次指针指向等于0时具有很强的二义性，`nullpter`则直接认为是空指针，消除了这种二义性）
- 优先使用 `using` ，而不是 `typedef`。最直接的差别在于using别名声明与模板兼容，而 C 风格的 typedef 则不兼容。
- 优先使用 `enum class` ，而不是 `enum`



#### `c++`并发编程

**你了解c++[并发编程](https://paul.pub/cpp-concurrency/#id-future)吗，从C++11到C++17分别存在哪些变化。**





#### std::function

**讲讲std::function，还挺常用的**





#### `if constexpr`



#### `string_view`





#### 进程调度



#### 设计模式（工厂模式，单例模式）





#### B树和B+树





#### 内存泄露，内存分配





#### 进程线程协程





**本篇最后，唠唠Effective C++**

参考来源：[腾讯技术工程](https://www.zhihu.com/question/587851490/answer/2951706901?utm_psn=1832172405658898433)

这主要是些关于c++的守则，

- 用const，enum，inline代替#define。这挺好解释，define作为预处理指令，只会进行简单的文本替换，存在的安全隐患过于的高了，即使使用，一般也只会全大写，不过c++提出的一些关键字基本也能完全替换它的功能。
- const和non-const等价实现时，多用const
- 使用对象前要对对象进行初始化
- 知道编译器会为class生成，默认构造函数，默认拷贝构造，默认复制构造，析构函数，这些都是public并且inline的。
- 禁用函数可以用`=delete`
- 有多态的基类最好被声称为虚析构函数。
- 析构函数不应该有异常，可以加上`noexcept`
- Never call virtual functions during construction or destruction
- 赋值函数注意点（这个在剑指offer的第一章有讲到）
  - 比较来源对象和目标对象地址，相同则返回
  - 先记住原对象，再复制新值，最后销毁原有对象
  - 利用copy-and-swap技术
- RALL，智能指针