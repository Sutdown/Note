### ucontext详解

`ucontext`机制是Glibc提供的一组创建，保存，切换用户态执行上下文的API，很适合利用此实现用户级线程，也就是协程。

上下文指的是当前的cpu寄存器，栈指针，程序计数器，信号掩码等信息，ucontext可以保存，切换和恢复这些上下文，从而实现协程的功能。



##### ucontext结构体

```c
#include <ucontext.h>
typedef struct ucontext_t {
  struct ucontext_t* uc_link; // 指向当前上下文结束时切换的上下文
  sigset_t uc_sigmask;        // 信号屏蔽字
  stack_t uc_stack;           // 栈信息
  mcontext_t uc_mcontext;     // 具体的cpu寄存器上下文
  ...
};
```



##### 核心函数

`getcontext(ucontext_t *ucp)`

初始化`ucp`结构体，获取当前上下文信息存储到`ucp`



`setcontext(const ucontext_t *ucp)`

恢复指定的上下文，然后从指定上下文恢复的地方继续运行



`makecontext(ucontext_t *ucp, void (*func)(), int argc, ...)`

修改上下文，调用指定的函数`func`

详细过程：

- 修改由`getcontext`获取的上下文指针`ucp`，将其和函数`func`绑定，支持指定`func`运行时的参数，`makecontext`指向完后，`ucp`就会和`func`绑定，调用`setcontext or swapcontext`会运行`func`
- 在调用`makecontext`之前，需要手动给`ucp`分配一段栈空间，存储在`ucp->uc_stack`中，这也是`func`运行时的栈空间，
- 同时也可以指定`ucp->uc_link`，函数结束后会自动恢复`uc_link`指向的上下文；如果不赋值`uc_link`，`func`接收后必须`setcontext or swapcontext`重新指定有效的上下文。



`swapcontext(ucontext_t *oucp, const ucontext_t *ucp)`

保存当前上下文到`oucp`，切换到`ucp`，跳转到`ucp`上下文对应的函数中执行，相当于调用了函数。



##### 简单使用

```cpp
#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>

ucontext_t ctx_main, ctx_func1, ctx_func2;

void func1() {
    printf("func1: start\n");
    swapcontext(&ctx_func1, &ctx_func2);  // 切换到 func2
    printf("func1: resume\n");
    swapcontext(&ctx_func1, &ctx_main);   // 返回主上下文
}

void func2() {
    printf("func2: start\n");
    swapcontext(&ctx_func2, &ctx_func1);  // 切换回 func1
    printf("func2: resume\n");
    swapcontext(&ctx_func2, &ctx_main);   // 返回主上下文
}

int main() {
    // 为 func1 和 func2 分配栈
    char stack1[1024 * 128];
    char stack2[1024 * 128];

    // 获取当前上下文
    getcontext(&ctx_func1);
    ctx_func1.uc_stack.ss_sp = stack1;
    ctx_func1.uc_stack.ss_size = sizeof(stack1);
    ctx_func1.uc_link = &ctx_main;  // 返回主上下文
    makecontext(&ctx_func1, func1, 0);

    getcontext(&ctx_func2);
    ctx_func2.uc_stack.ss_sp = stack2;
    ctx_func2.uc_stack.ss_size = sizeof(stack2);
    ctx_func2.uc_link = &ctx_main;  // 返回主上下文
    makecontext(&ctx_func2, func2, 0);

    printf("main: switch to func1\n");
    swapcontext(&ctx_main, &ctx_func1);  // 切换到 func1
    printf("main: back\n");

    return 0;
}
```

程序输出

```txt
main: switch to func1
func1: start
func2: start
func1: resume
func2: resume
main: back
```



##### 参考资料

1.[ucontext族函数详解](https://langzi989.github.io/2017/10/06/ucontext%E5%87%BD%E6%95%B0%E8%AF%A6%E8%A7%A3/)

2.[ucontext-人人都可以实现的简单协程库-阿里云开发者社区](https://developer.aliyun.com/article/52886)