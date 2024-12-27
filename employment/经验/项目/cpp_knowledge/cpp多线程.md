# c++多线程

> 略有点烦躁，看到一篇讲c++多线程的文章，感觉还不错，大致的看看这部分。
>
> 这篇写的有点潦草，之后再补充吧。



### 前言

看到**多线程**其实容易想到**多进程**，还有几讲出现在标准库的**协程**，协程这块了解不多，大致是对线程之间更加细化的分配，深入学习的话可以看看网上的文章，或者腾讯的`libco`源码，这是我的打算，不过目前还没看。

回到正题，多线程和多进程的多任务处理都是一种并发执行。

**多进程：**由于进程之间的隔离性比较强，资源的获取所需的难度随着进程数目的增加会更加明显，效率弱于多线程，因此得到的谈论并不多。

[**进程间通信**](https://xiaolincoding.com/os/4_process/process_commu.html)也是面试的常考点。

**多线程**：线程天然能够共享地址空间，全局变量，指针，引用，这些资源在线程中自然的传递，开销小很多，但是也容易引发**线程安全**的问题，操作系统中利用锁，条件变量等解决的多，大多利用`os`的`API`，

比如`linux`的`<pthread.h>`，`windows`下的`<windows.h>`。

直到`C++11`中推出一系列头文件支持多线程编程，比如`<thread>,<mutex>,<atomic>,<condition_variable>,<future>`，解决了跨平台的问题，同时提供管理线程，保护共享数据，线程间同步操作，原子操作等。

单核下的多线程其实是一种同步的方式，多核的情况下可以实现真正的并行计算。



### C++多线程

#### `<thread>`

- 创建线程
- `join`和`detach`
- `this_thread`：是一个类，有四个功能函数：
  - `get_id`获取线程`id`
  - `yield`放弃线程执行回到就绪状态
  - `sleep_for`暂停一秒
  - `sleep_util`一分钟后执行

#### `<mutex>`

- `mutex`

- `lock_guard`——RALL

  创建lock_guard对象时，它将尝试获取提供给它的互斥锁的所有权。当控制流离开lock_guard对象的作用域时，lock_guard析构并释放互斥量。lock_guard的特点：

  - 创建即加锁，作用域结束自动析构并解锁，无需手工解锁
  - 不能中途解锁，必须等作用域结束才解锁
  - 不能复制

- `unique_lock`

  简单地讲，unique_lock 是 lock_guard 的升级加强版，它具有 lock_guard 的所有功能，同时又具有其他很多方法，使用起来更加灵活方便，能够应对更复杂的锁定需要。unique_lock的特点：

  - 创建时可以不锁定（通过指定第二个参数为std::defer_lock），而在需要时再锁定
  - 可以随时加锁解锁
  - 作用域规则同 lock_grard，析构时自动释放锁
  - 不可复制，可移动
  - 条件变量需要该类型的锁作为参数（此时必须使用unique_lock）

  所有 lock_guard 能够做到的事情，都可以使用 unique_lock 做到，反之则不然。那么何时使lock_guard呢？很简单，需要使用锁的时候，首先考虑使用 lock_guard，因为lock_guard是最简单的锁。

#### `<atomic>`

**特点**：1.可以实现内存占用极小的锁。 2.当临界区操作可以等价于一个原子操作时，性能会更高。

**链式数据结构的场景**非常适合使用 `atomic `变量。

1.内存占用少：即使每个节点都实现一个自旋锁（`SpinLock`），也不会浪费太多内存。 

2.链式数据结构的临界区通常可以优化成一个指针的 `CAS` 操作。

- `atomic`
- `atomic_flag`

#### `<condition_variable>`

- `condition_variable`—condition_variable必须结合unique_lock使用。
- `condition_variable_any` —condition_variable_any可以使用任何的锁。

#### `<future>`

- `future`
- `shared_future`
- `promise`
- `packaged_task`

#### 综合：线程池

[`Threadpool`](https://zhuanlan.zhihu.com/p/714937989)

```cpp
#include <iostream>
#include <thread>
#include <mutex>

#include <chrono> // 处理时间相关的操作
#include <ctime>

#include <vector>
#include <queue>
#include <string>

#include <future> // 处理异步操作
#include <functional>
#include <utility>            // 提供通用工具类和函数，比如右值引用，完美转发等
#include <condition_variable> //用于线程同步，阻塞线程
#include <shared_mutex>
using namespace std;

template <typename T>
struct safe_queue
{
    queue<T> que;
    shared_mutex _m; // 互斥锁，支持共享锁个独占锁。
    // 允许同时读取，但是不能写入

    bool empty()
    {
        // 共享锁来锁定互斥量_m
        shared_lock<shared_mutex> lc(_m);
        return que.empty();
    }

    auto size()
    {
        unique_lock<shared_mutex> lc(_m);
        return que.size();
    }

    void push(T &t)
    {
        // 独占锁来锁定互斥量_m，保证只有一个线程能写入
        unique_lock<shared_mutex> lc(_m);
        que.push(t);
    }

    bool pop(T &t)
    {
        unique_lock<shared_mutex> lc(_m);
        if (que.empty())
            return false;
        // 右值引用，避免的队列元素的拷贝，利用的是移动复制而不是拷贝构造的思想
        t = move(que.front());
        que.pop();
        return true;
    }
};

// 积木2：线程池
// 通过复用一定数量的线程减少频繁创建和销毁线程的开销。
class ThreadPool
{
private:
    class worker
    {
    public:
        ThreadPool *pool;
        worker(ThreadPool *_pool) : pool{_pool} {}

        // 定义了每个线程的主要执行逻辑
        // 允许线程池中的每个线程不断从任务队列中取出任务并执行，直到队列为空或者线程池关闭
        void operator()()
        {
            while (!pool->is_shut_down)
            {
                {
                    unique_lock<mutex> lock(pool->_m);
                    // 条件变量的一个重载方法，使得当前线程进入等待状态
                    // 直到线程池关闭或者任务队列不为空时 结束等待
                    pool->cv.wait(lock, [this]()
                                  { return this->pool->is_shut_down ||
                                           !this->pool->que.empty(); });
                }

                // 如果能够成功去除一个任务，就执行它。
                function<void()> func;
                bool flag = pool->que.pop(func);
                if (flag) func();
            }
        }
    };

public:
    bool is_shut_down;
    safe_queue<std::function<void()>> que; // 队列
    vector<std::thread> threads;           // 线程
    mutex _m;                              // 互斥锁
    condition_variable cv;                 // 条件变量

    // 构造函数
    ThreadPool(int n) : threads(n), is_shut_down{false}
    {
        for (auto &t : threads)
            t = thread{worker(this)};
    }

    // 禁止了ThreadPool对象的拷贝构造，移动构造，拷贝赋值和移动赋值操作
    // 原因：ThreadPool管理线程，任务队列和同步原语。
    // 1.如果允许拷贝或者移动，ThreadPool必须正确处理所有资源，防止资源泄露或者发生竞争条件。
    // 2.ThreadPool的实例应该是唯一的，多个实例共享或者争夺资源会引发错误。
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;
    ThreadPool &operator=(ThreadPool &&) = delete;

    /*
     * submit函数通过function和packaged_task将任务和它的返回值封装，
     * 然后提交到线程池执行。调用者可以通过future异步获取任务执行结果。
     * 允许用户灵活提交各种类型的任务，避免手动管理线程和任务的复杂性。
     */
    template <typename F, typename... Args>
    // future: 表示异步操作的结果
    // 在不阻塞主线程的情况下启动异步任务，在未来某个时刻获取该任务结果
    // future和packaged_task一起使用
    auto submit(F &&f, Args &&...args) -> std::future<decltype(f(args...))>
    {

        // 封装任务
        function<decltype(f(args...))()> func = [&f, args...]()
        { return f(args...); };
        // 创建任务对象
        /*packaged_task作用：
         * std::packaged_task 将一个可调用对象包装起来，并将其与 std::future 绑定。
         * 包装的任务可以在不同的线程中异步执行，而主线程或其他线程可以通过 std::future 获取任务的结果
         * 通过 std::packaged_task，可以在任务执行后，使用 std::future 对象来获取任务的返回值或处理异常。
         */
        auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

        // 包装任务
        std::function<void()> warpper_func = [task_ptr]()
        {
            (*task_ptr)(); // 这个是对它指向的packaged_task对象的调用，变成了无参数的函数对象
        };

        que.push(warpper_func);
        cv.notify_one();

        return task_ptr->get_future();
    }

    // 析构函数
    ~ThreadPool()
    {
        // 主要目的是确保所有的工作线程都能接收到一个任务，以便随后被唤醒进行处理
        // 保证线程处理完当前任务
        auto f = submit([]() {});
        f.get();
        is_shut_down = true;
        cv.notify_all(); // 通知，唤醒所有工作线程
        // 保证每个线程正常退出，阻塞和安全检查
        for (auto &t : threads)
        {
            if (t.joinable())
                t.join();
        }
    }
};

mutex _m;

/* 整体流程：
 * 1.线程池初始化
 * 2.提交任务
 * 3.任务执行
 * 4.互斥锁保护
 */
int main()
{
    ThreadPool pool(8);
    int n = 20;
    for (int i = 1; i <= n; i++)
    {
        pool.submit([](int id)
                    {
            // 模拟延迟
            if (id % 2 == 1) {
                this_thread::sleep_for(0.2s);
            }

        unique_lock<mutex> lc(_m);
        cout << "id : " << id << endl; }, i);
    }
}
```

### 多线程性能分析

#### `lock contension` — 用锁处理多线程同步问题

多个线程都尝试获得一个锁时，很容易发生竞争现象，甚至导致死锁，从而影响系统的性能和响应时间。目前可以尝试的优化方法：

- 减少临界区大小。
- 对共享资源进行分桶操作。比如`LevelDB`中的`LRUCache`。

#### Cache Coherency— 用原子操作处理多线程同步问题

当一个共享变量的数据发生变化时，按照缓存一致性，这个变量的更新需要同步到其它线程的缓存中，否则会出现问题：

`Cache Ping-pong`多个处理器频繁地对同一个缓存行（Cache Line）进行读写操作，导致该缓存行在不同处理器的缓存之间频繁地来回传递。

`False Sharing`多个处理器访问不同的数据，但这些数据恰好位于同一个缓存行中，导致该缓存行在不同处理器的缓存之间频繁传递。



## 参考资料

1. [深入探讨C++多线程性能优化 - 知乎](https://zhuanlan.zhihu.com/p/4245202423)
2. [【NO.610】C++多线程详解（全网最全）](https://github.com/0voice/cpp_backend_awsome_blog/blob/main/%E3%80%90NO.610%E3%80%91C%2B%2B%E5%A4%9A%E7%BA%BF%E7%A8%8B%E8%AF%A6%E8%A7%A3%EF%BC%88%E5%85%A8%E7%BD%91%E6%9C%80%E5%85%A8%EF%BC%89.md)
3. [C++多线程 - 菜鸟教程](https://www.runoob.com/cplusplus/cpp-multithreading.html)