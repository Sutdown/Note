> **1 Gin源码解析**
>
> 1.1 Gin框架路由详解
>
> 1.2 Gin框架中间件详解
>
> **2 Go连接MySQL/Redis**
>
> 2.1 database/sql以及sqlx使用
>
> 2.2 go-redis库使用
>
> **3 搭建Go Web开发脚手架**
>
> 3.1 zap日志库使用
>
> 3.2 Viper配置管理库使用
>
> 3.3 优雅关机与平滑重启
>
> 3.4 CLD代码分层
>
> **4 仿Reddit论坛项目**
>
> 4.1 分布式ID生成
>
> 4.2 JWT认证
>
> 4.3 基于MySQL实现主业务
>
> 4.4 基于Redis实现投票业务
>
> 4.5 基于Docker搭建开发环境
>
> 4.6 代码发布与项目部署
>
> 4.5 实战经验与技巧



# Gin源码解析

Gin作为web框架的原因在于

- 支持中间件操作（ handlersChain 机制 ）
- 更方便的使用（ gin.Context ）
- 更强大的路由解析能力（ radix tree 路由树 ）

### 1.1 Gin框架路由详解

#### Radix树介绍

radix树可以认为是一种更节省空间的前缀树。

> 为什么使用前缀树而不是哈希或者map？

路由器为每个请求方法管理一个单独的树，为每个结点设置一个优先级。

#### 请求处理

```go
go mod tidy
```

> Gin框架底层基于HTTP标准库开发，如何理解这句话f
>
> 接口和方法 



#### 路由方法树



#### 路由注册与路由匹配



### 1.2 Gin框架中间件详解





### 参考资料

1. [Go Web开发进阶实战（gin框架） - 网易云课堂](https://study.163.com/course/introduction/1210171207.htm)
2. [gin框架源码解析](https://www.liwenzhou.com/posts/Go/gin-sourcecode/)
