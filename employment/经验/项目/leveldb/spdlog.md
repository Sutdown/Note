# `spdlog`简单介绍



## 前言

写这篇文章是因为最近在写一个基于LSM结构的KV存储，里面的日志是打算直接用`spdlog`库，所以来看看源码），也刚好对日志这块挺生疏，学习一下。由于当前主线是在`LSMKV`那个项目，所以这个日志库不会看的很详细，主要着力点在于正文中的三个问题，这个日志库是什么，如何使用，重点`part`在哪里。

[`spdlog - github`](https://github.com/gabime/spdlog)



## 正文

第一个问题：

### `spdlog`日志库为什么这么命名，它相比其它的日志库好在哪里

`speed log`，它的核心目标在于**实现一个高性能的日志库**。

正常的日志库需要实现日志级别控制，日志格式化功能，日志输出目标，同步和异步日志记录，线程安全，日志缓存和刷新机制，日志轮转等。

优点在于：

- 支持异步日志，能够保障线程安全
- 内存开销小
- 简单并且强大的格式化功能



第二个问题：

### 如何使用这个日志库

```cpp
#include <spdlog/spdlog.h> // 包含头文件

std::shared_ptr<spdlog::logger> logger = nullptr;  // 创建日志

// 记录日志和日志的格式化功能
logger->trace("This is a trace message"); 
logger->debug("This is a debug message");
logger->info("The value of x is: {}", x);
logger->warn("The function returned an error: {}", error_message);
logger->error("This is an error message");
logger->critical("This is a critical message");

// 异步日志记录器，采用工厂模式
auto async_logger = spdlog::basic_logger_mt<spdlog::async_factory>("async_logger", "logs/async_log.txt");

// 自动滚动的日志记录器,可以日志分割
auto rotating_logger = spdlog::rotating_logger_mt("rotating_logger", "logs/rotating_log.txt", 1048576, 5);

// 自定义日志格式
spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
```



第三个问题：

### 值得关注的重点部分

#### 整体流程:

调用`spdlog`时,首先获取一个`logger`对象 ——>同时记录日志消息,包括级别,时间戳,线程ID,文件名,行号等信息,——>将消息传递给`Formatter`将消息转换成特定的格式,——>再将格式化后的消息传递给`Async logger`,——>异步日志将消息写入`Sink`,日志记录完成

- **Logger**（日志记录器）：负责接收和管理日志消息。
- **Sink**（输出目标）：负责将日志消息写入指定的输出设备（文件、控制台等）。
- **Pattern Formatter**（日志格式化器）：定义日志消息的格式。
- **Async Logging**（异步日志）：提供异步日志记录以提高性能。
- **Log Level**（日志级别）：控制日志记录的细节和过滤不必要的日志。
- **Thread Safety**（线程安全）：支持多线程并发的日志记录。

注：线程池，工厂模式，单例模式

```
 |                registry
 |------------------------------------------
 |              async_logger
 |------------------------------------------
 |      logger         |    thread pool
 |---------------------|
 | |skin               |
 |-|-------------------|
 | |formatter          |
```



#### 同步日志,异步日志

- 同步日志可以保证日志消息的实时性,但是会影响程序性能,尤其在记录大量日志消息时.
- 异步日志中,日志消息会加入到阻塞队列,异步的将其写入目标`sink`中,同时能够保障多线程条件下的线程安全.需要注意的事项在于
  - 处理内存队列时容易出现内存分配和锁的竞争
  - 内存队列大小需要根据应用程序的资源进行一定的调整
  - 应用程序退出时需要等待所有的日志消息写入完成



#### 线程池

`spdlog` 允许我们自由创建线程安全和非线程安全（单线程）的日志，其设置在基类`base_skin` 中.



线程池的存在同时避免了线程频繁的创建和销毁,以及线程池调用过多导致的内核压力过大

稍微回顾一下通用线程池,这里假设存在核心线程数和最大线程数,(有些线程池只会有最大线程数)

当想要执行一个任务时,任务首先会判断线程数,这里存在三种可能

- 当前线程数少于核心线程数,直接取得线程
- 大于核心线程数,存入队列等待线程执行完毕后取得任务执行
- 大于核心线程数,但是队列已满,小于最大线程数,仍然可以取得线程
- 其它情况就拒绝接受任务了.



`spdlog::thread_pool`不是通用线程池，而是用于处理异步log消息的专用线程池。用环形队列存放异步log消息，支持阻塞、非阻塞两种方式插入数据；取出数据，只支持阻塞方式。

## 参考资料

1. [超详细！`spdlog`源码解析（上） - 知乎](https://zhuanlan.zhihu.com/p/674073158)
2. [官方文档](https://github.com/gabime/spdlog/wiki/1.-QuickStart)
3. [`spdlog`源码解析 - 博客园](https://www.cnblogs.com/fortunely/p/17388565.html)