# Go Web开发常用组件



参考文章：

[在Go语言项目中使用Zap日志库](https://www.liwenzhou.com/posts/Go/zap/)



### 在Go语言项目中使用Zap日志库

日志能够提供的功能：

- 能够将事件**记录**到文件中，而不是应用程序控制台。
- 日志**切割**-能够根据文件大小、时间或间隔等来切割日志文件。
- 支持不同的日志**级别**。例如INFO，DEBUG，ERROR等。
- 能够**打印**基本信息，如调用文件/函数名和行号，日志时间等

#### **Go logger**

优点：使用简单。

能够将事件记录到文件中。但是只有print选项，不支持info/debug等多个级别，对于错误日志有fatal（通过调用os.Exit(1)结束程序和panic。不过缺少一个error日志级别。同时不能进行日志格式化，比如记录调用者的函数名和行号；不提供日志切割的能力。

```go
func SetupLogger() {
	logFileLocation, _ := os.OpenFile("/Users/q1mi/test.log", os.O_CREATE|os.O_APPEND|os.O_RDWR, 0744)
	log.SetOutput(logFileLocation)
}

func main() {
	SetupLogger()
	simpleHttpGet("www.google.com")
	simpleHttpGet("http://www.google.com")
}
```



#### **Uber-go zap**

- 它同时提供了结构化日志记录和printf风格的日志记录
- 它非常的快

zap提供两种类型的日志记录器—Sugared Logger和Logger。对性能要求高用logger。

- 通过调用`zap.NewProduction()`/`zap.NewDevelopment()`或者`zap.Example()`创建一个Logger。
- 上面的每一个函数都将创建一个logger。唯一的区别在于它将记录的信息不同。例如production logger默认记录调用函数信息、日期和时间等。
- 通过Logger调用Info/Error等。
- 默认情况下日志都会打印到应用程序的console界面。

##### **logger**

```go
var logger *zap.Logger

func main() {
	InitLogger()
    defer logger.Sync()
	simpleHttpGet("www.google.com")
	simpleHttpGet("http://www.google.com")
}

func InitLogger() {
	logger, _ = zap.NewProduction()
}

func simpleHttpGet(url string) {
	resp, err := http.Get(url)
	if err != nil {
		logger.Error(
			"Error fetching url..",
			zap.String("url", url),
			zap.Error(err))
	} else {
		logger.Info("Success..",
			zap.String("statusCode", resp.Status),
			zap.String("url", url))
		resp.Body.Close()
	}
}
```

**日志记录器。**中`MethodXXX`是一个可变参数函数，可以是Info / Error/ Debug / Panic等。每个方法都接受一个消息字符串和任意数量的`zapcore.Field`场参数。每个`zapcore.Field`其实就是一组键值对参数。

```go
func (log *Logger) MethodXXX(msg string, fields ...Field) 
```

##### **Sugared Logger**

和logger对比。`Sugar Logger` 是 `Logger` 的一个增强版本。它通过一些“糖衣语法”提供了更方便的API，使得日志记录的操作更加简洁和易读。它通常自动处理一些默认配置（如日志格式、日志级别），并且支持更简化的日志记录方法。

- 大部分的实现基本都相同。
- 惟一的区别是，我们通过调用主logger的`. Sugar()`方法来获取一个`SugaredLogger`。
- 然后使用`SugaredLogger`以`printf`格式记录语句

##### 定制logger

**将日志写入文件而不是终端**

- 我们将使用`zap.New(…)`方法来手动传递所有配置，而不是使用像`zap.NewProduction()`这样的预置方法来创建logger。

```go
/* 
zapcore.Core需要三个配置——Encoder，WriteSyncer，LogLevel
 * Encoder:编码器(如何写入日志)。我们将使用开箱即用的NewJSONEncoder()，并使用预先设置的ProductionEncoderConfig()
 * WriterSyncer ：指定日志将写到哪里去。我们使用zapcore.AddSync()函数并且将打开的文件句柄传进去
 * Log Level：哪种级别的日志将被写入
*/
func New(core zapcore.Core, options ...Option) *Logger
```

```go
// 重写上文的initlogger()
func InitLogger() {
	writeSyncer := getLogWriter()
	encoder := getEncoder()
	core := zapcore.NewCore(encoder, writeSyncer, zapcore.DebugLevel)

	// logger := zap.New(core)
    
    // 添加将调用函数信息记录到日志中的功能。为此，我们将在zap.New(..)函数中添加一个Option 
    // logger := zap.New(core, zap.AddCaller())
    
    // 当我们不是直接使用初始化好的logger实例记录日志，而是将其包装成一个函数等，此时日录日志的函数调用链会增加，想要获得准确的调用信息就需要通过AddCallerSkip函数来跳过
    logger := zap.New(core, zap.AddCaller(), zap.AddCallerSkip(1))
	sugarLogger = logger.Sugar()
}

func getEncoder() zapcore.Encoder {
	// JSON Encoder：return zapcore.NewJSONEncoder(zap.NewProductionEncoderConfig())
    // 普通 Encoder：return zapcore.NewConsoleEncoder(zap.NewProductionEncoderConfig())
    
    // 修改默认时间编码器
    encoderConfig := zap.NewProductionEncoderConfig()
	encoderConfig.EncodeTime = zapcore.ISO8601TimeEncoder
	encoderConfig.EncodeLevel = zapcore.CapitalLevelEncoder
	return zapcore.NewConsoleEncoder(encoderConfig)
}

func getLogWriter() zapcore.WriteSyncer {
	file, _ := os.Create("./test.log")
	// return zapcore.AddSync(file)
    
    // 利用io.MultiWriter支持文件和终端两个输出目标
	ws := io.MultiWriter(file, os.Stdout)
	return zapcore.AddSync(ws)
}
```

有时候我们除了将全量日志输出到`xx.log`文件中之外，还希望将`ERROR`级别的日志单独输出到一个名为`xx.err.log`的日志文件中。我们可以通过以下方式实现。

```go
func InitLogger() {
	encoder := getEncoder()
	// test.log记录全量日志
	logF, _ := os.Create("./test.log")
	c1 := zapcore.NewCore(encoder, zapcore.AddSync(logF), zapcore.DebugLevel)
	// test.err.log记录ERROR级别的日志
	errF, _ := os.Create("./test.err.log")
	c2 := zapcore.NewCore(encoder, zapcore.AddSync(errF), zap.ErrorLevel)
	// 使用NewTee将c1和c2合并到core
	core := zapcore.NewTee(c1, c2)
	logger = zap.New(core, zap.AddCaller())
}
```



#### 使用Lumberjack进行日志切割归档

> *Zap本身不支持切割归档日志文件*

因此用第三方库[Lumberjack](https://github.com/natefinch/lumberjack)来实现。

##### zap logger中加入Lumberjack

修改`WriteSyncer`代码

```go
func getLogWriter() zapcore.WriteSyncer {
	lumberJackLogger := &lumberjack.Logger{
		Filename:   "./test.log", // 日志文件的位置
		MaxSize:    10,			  // 在进行切割之前，日志文件的最大大小（以MB为单位）
		MaxBackups: 5,			  // 保留旧文件的最大个数
		MaxAge:     30, 		  // 保留旧文件的最大天数
		Compress:   false,		  // 是否压缩/归档旧文件
	}
	return zapcore.AddSync(lumberJackLogger)
}
```



### 使用zap接收gin框架默认的日志并配置日志归档

在gin框架项目中，如何使用zap日志库来接收并记录gin框架默认的日志，并配置日志归档。

**gin框架与日志库**：

- gin框架：常用的Go语言Web框架。
- 日志库：go语言常用的日志库有zap、logrus等。

**gin框架默认中间件**：

- gin.Default()：使用了两个默认中间件Logger()和Recovery()。
- Logger()：将gin框架的日志输出到标准输出。
- Recovery()：在程序panic时恢复现场并写入500响应。

**基于zap的中间件实现**：

- GinLogger：自定义中间件，使用zap记录gin框架的默认日志，包括请求路径、查询参数、请求方法、客户端IP、User-Agent、错误信息、请求耗时等。
- GinRecovery：自定义中间件，用于捕获并处理panic，记录错误信息。

**日志归档配置**（未详细展开）：

- 暗示了配置日志归档是文章的一部分内容，但具体实现细节未给出。



### Go语言配置管理神器——Viper中文教程

[Viper官方文档 — 中文](https://www.liwenzhou.com/posts/Go/viper/)

[Viper](https://github.com/spf13/viper)是 Go 语言中一款功能强大的配置管理库，旨在简化应用程序的配置处理。它支持多种配置文件格式，包括 JSON、TOML、YAML、HCL、envfile 和 Java properties 等。此外，Viper 还提供了从环境变量、命令行标志、远程配置系统（如 etcd 或 Consul）以及直接在代码中设置配置值的功能。`go get github.com/spf13/viper`

**主要特性：**

- **多种配置格式支持：** Viper 能够读取并解析多种格式的配置文件，如 JSON、TOML、YAML、HCL、envfile 和 Java properties。
- **优先级管理：** Viper 允许开发者设置配置项的优先级，确保高优先级的配置覆盖低优先级的配置。其优先级顺序如下：
  1. 通过 `viper.Set` 显式设置的值。
  2. 命令行参数（flags）。
  3. 环境变量。
  4. 配置文件。
  5. key/value 存储。
  6. 默认值。
- **实时监控配置文件：** Viper 支持监控配置文件的变化，并在文件更改时自动重新加载配置。
- **环境变量支持：** Viper 可以从环境变量中读取配置，方便在不同环境下的配置管理。
- **远程配置系统支持：** Viper 能够从远程配置系统（如 etcd 或 Consul）读取配置，并监控其变化。
- **命令行标志支持：** Viper 可以绑定命令行标志，方便在启动时传递配置参数。
- **默认值设置：** Viper 允许为配置项设置默认值，确保在未提供配置时使用预设值。



### 优雅关机和优雅重启

> 分别究竟是什么机制。

[优雅地关机或重启 | 李文周的博客](https://www.liwenzhou.com/posts/Go/graceful-shutdown/)



### MVC模式

[MVC 模式 | 菜鸟教程](https://www.runoob.com/design-pattern/mvc-pattern.html)

![1](images/1.png)
