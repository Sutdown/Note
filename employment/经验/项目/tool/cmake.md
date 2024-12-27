# `makefile`



## `make`

1. 读入所有的Makefile。
2. 读入被include的其它Makefile。
3. 初始化文件中的变量。
4. 推导隐式规则，并分析所有规则。
5. 为所有的目标文件创建依赖关系链。
6. 根据依赖关系，决定哪些目标要重新生成。
7. 执行生成命令。



## `makefile`基本规则

> 遵循于IEEE 1003.2-1992标准的（POSIX.2）

make命令执行时，需要一个`makefile`文件，以告诉make命令需要怎么样的去编译和链接程序。

```makefile
target ... : prerequisites ...
    recipe
    ...
    ...
```

- **target**：可以是一个object file（目标文件），也可以是一个可执行文件，还可以是一个标签（label）。对于标签这种特性，在后续的“伪目标”章节中会有叙述。
- **prerequisites**：生成该target所依赖的文件和/或target。
- **recipe**：该target要执行的命令（任意的shell命令）。



# `cmake`

> `cmake`产生的成因主要是为了解决不同`make`工具之间规范标准不同，执行的`makefile`格式不同，不支持跨平台。



## `cmake`简述

`cmake`允许开发者编写一种平台无关的` CMakeList.txt` 文件来定制整个编译流程，然后再根据目标用户的平台进一步生成所需的本地化 `Makefile` 和工程文件，如 Unix 的 `Makefile` 或 `Windows` 的 `Visual Studi` 工程。从而做到“`Write once, run everywhere`”。

**在 `linux` 平台下使用 `CMake` 生成 `Makefile` 并编译的流程如下：**

- 编写` CMake` 配置文件 `CMakeLists.txt `。
- 执行命令 `cmake PATH `或者 `ccmake PATH `生成`Makefile `（`ccmake `和 `cmake` 的区别在于前者提供了一个交互式的界面）。其中， PATH 是 `CMakeLists.txt` 所在的目录。
- 使用 `make` 命令进行编译。



## Reference

1. [概述 — 跟我一起写`Makefile `1.0 文档](https://seisman.github.io/how-to-write-makefile/overview.html)
2. [（转载+整理）超详细的`cmake`教程-CSDN博客](https://blog.csdn.net/qq_41375609/article/details/110535697)