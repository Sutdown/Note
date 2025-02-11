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



|| 主要是[Go Web开发进阶实战（gin框架） - 网易云课堂](https://study.163.com/course/introduction/1210171207.htm)第一部分的笔记。



Gin作为web框架的原因在于

- 支持中间件操作（ handlersChain 机制 ）
- 更方便的使用（ gin.Context ）
- 更强大的路由解析能力（ radix tree 路由树 ）

### 1.1 Gin框架路由详解

#### Radix树介绍

radix树可以认为是一种更节省空间的前缀树。

> 为什么使用前缀树而不是哈希或者map？

路由器为每个请求方法管理一个单独的树，为每个结点设置一个优先级。

1. 优先匹配被大多数路由路径包含的结点，让尽可能多的路由被快速定位。
2. 类似于成本补偿，最长的路径被优先匹配，补偿体现在最长的路径需要更多的时间定位。

#### 请求处理

```go
 go mod tidy
```

> Gin框架底层基于HTTP标准库开发，如何理解这句话
>
> 接口和方法 

#### 路由方法树

路由树是由一个个节点构成的，gin框架路由树的节点由`node`结构体表示，它有以下字段：

```go
// tree.go

type node struct {
   // 节点路径，比如上面的s，earch，和upport
	path      string
	// 和children字段对应, 保存的是分裂的分支的第一个字符
	// 例如search和support, 那么s节点的indices对应的"eu"
	// 代表有两个分支, 分支的首字母分别是e和u
	indices   string
	// 儿子节点
	children  []*node
	// 处理函数链条（切片）
	handlers  HandlersChain
	// 优先级，子节点、子子节点等注册的handler数量
	priority  uint32
	// 节点类型，包括static, root, param, catchAll
	// static: 静态节点（默认），比如上面的s，earch等节点
	// root: 树的根节点
	// catchAll: 有*匹配的节点
	// param: 参数节点
	nType     nodeType
	// 路径上最大参数个数
	maxParams uint8
	// 节点是否是参数节点，比如上面的:post
	wildChild bool
	// 完整路径
	fullPath  string
}
```

#### 路由注册与路由匹配

注册路由

```go
// gin.go gin框架会为每一个请求方法创建一棵对应的树。
func (engine *Engine) addRoute(method, path string, handlers HandlersChain) {
   // liwenzhou.com...
   
   // 获取请求方法对应的树
	root := engine.trees.get(method)
	if root == nil {
	
	   // 如果没有就创建一个
		root = new(node)
		root.fullPath = "/"
		engine.trees = append(engine.trees, methodTree{method: method, root: root})
	}
	root.addRoute(path, handlers)
}

// engine.trees
type methodTree struct {
	method string
	root   *node
}

type methodTrees []methodTree  // slice

// 获取请求方法对应树的get方法
func (trees methodTrees) get(method string) *node {
	for _, tree := range trees {
		if tree.method == method {
			return tree.root
		}
	}
	return nil
}

```

注册路由的逻辑主要有`addRoute`函数和`insertChild`方法。

```txt
addRoute
1. 第一次注册路由，例如注册search
2. 继续注册一条没有公共前缀的路由，例如blog
3. 注册一条与先前注册的路由有公共前缀的路由，例如support

insertChild
根据path本身进行分割，将/分开的部分分别作为节点保存，形成一棵树结构。参数匹配中的:和*的区别是，前者是匹配一个字段而后者是匹配后面所有的路径。
```

路由匹配

```go
// gin框架处理请求的入口函数ServeHTTP
// gin.go
func (engine *Engine) ServeHTTP(w http.ResponseWriter, req *http.Request) {
  // 这里使用了对象池
	c := engine.pool.Get().(*Context)
  // 这里有一个细节就是Get对象后做初始化
	c.writermem.reset(w)
	c.Request = req
	c.reset()

	engine.handleHTTPRequest(c)  // 我们要找的处理HTTP请求的函数

	engine.pool.Put(c)  // 处理完请求后将对象放回池子
}

// 路由匹配是由节点的 getValue方法实现的。getValue根据给定的路径(键)返回nodeValue值，保存注册的处理函数和匹配到的路径参数数据。如果找不到任何处理函数，则会尝试TSR(尾随斜杠重定向)。可以借助注释看一下路由查找及参数匹配的逻辑。

```



### 1.2 Gin框架中间件详解

gin框架涉及中间件相关有4个常用的方法，它们分别是`c.Next()`、`c.Abort()`、`c.Set()`、`c.Get()`

Default函数内部构造一个新的engine之后通过Use()函数注册Logger中间件和Recovery中间件。

```go
r := gin.Default()

// 注册中间件其实就是将中间件函数追加到group.Handlers
func (group *RouterGroup) Use(middleware ...HandlerFunc) IRoutes {
	group.Handlers = append(group.Handlers, middleware...)
	return group.returnObj()
}

// 注册路由时会将对应路由的函数和之前的中间件函数结合到一起
func (group *RouterGroup) handle(httpMethod, relativePath string, handlers HandlersChain) IRoutes {
	absolutePath := group.calculateAbsolutePath(relativePath)
	handlers = group.combineHandlers(handlers)  // 将处理请求的函数与中间件函数结合
	group.engine.addRoute(httpMethod, absolutePath, handlers)
	return group.returnObj()
}
const abortIndex int8 = math.MaxInt8 / 2
func (group *RouterGroup) combineHandlers(handlers HandlersChain) HandlersChain {
	finalSize := len(group.Handlers) + len(handlers)
	if finalSize >= int(abortIndex) {  // 这里有一个最大限制
		panic("too many handlers")
	}
	mergedHandlers := make(HandlersChain, finalSize)
	copy(mergedHandlers, group.Handlers)
	copy(mergedHandlers[len(group.Handlers):], handlers)
	return mergedHandlers
}

// 会将一个路由的中间件函数和处理函数结合到一起组成一条处理函数链条HandlersChain，而它本质上就是一个由HandlerFunc组成的切片
type HandlersChain []HandlerFunc
```

c.Next()

```go
// 通过索引遍历HandlersChain链条，从而实现依次调用该路由的每一个函数（中间件或处理请求的函数）
func (c *Context) Next() {
	c.index++
	for c.index < int8(len(c.handlers)) {
		c.handlers[c.index](c)
		c.index++
	}
}
```

c.Abort()

```go
// 中断整个调用链条，从当前函数返回
func (c *Context) Abort() {
	c.index = abortIndex  // 直接将索引置为最大限制值，从而退出循环
}
```

`c.Set()`和`c.Get()`这两个方法多用于在多个函数之间通过`c`传递数据的，比如我们可以在认证中间件中获取当前请求的相关信息（userID等）通过`c.Set()`存入`c`，然后在后续处理业务逻辑的函数中通过`c.Get()`来获取当前请求的用户。`c`就像是一根绳子，将该次请求相关的所有的函数都串起来了。



### 总结

1. gin框架路由使用前缀树，路由注册的过程是构造前缀树的过程，路由匹配的过程就是查找前缀树的过程。
2. gin框架的中间件函数和处理函数是以切片形式的调用链条存在的，我们可以顺序调用也可以借助`c.Next()`方法实现嵌套调用。
3. 借助`c.Set()`和`c.Get()`方法我们能够在不同的中间件函数中传递数据。



### 参考资料

1. [Go Web开发进阶实战（gin框架） - 网易云课堂](https://study.163.com/course/introduction/1210171207.htm)
2. [gin框架源码解析](https://www.liwenzhou.com/posts/Go/gin-sourcecode/)
