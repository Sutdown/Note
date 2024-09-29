#  python全栈开发教程5



## 网络编程

### 网络编程的基本概念

网络编程：在程序中实现网络计算机之间的数据通信

通信协议：接入网络所遵守的’‘规则’‘，目前全球通用的通信协议即Internet协议



##### TCP/IP协议：

###### IP协议

IP协议是整个TCP/IP协议族的核心

IP地址就是互联网上计算机的唯一标识

目前的IP地址有两种表示方式：IPv4和IPv6

在命令行下使用ipconfig命令可以查看本机的IP地址



###### TCP协议（Transmission Control Protocol）

即传输控制协议，建立在IP协议基础之上。

TCP协议负责两台计算机之间建立可靠连接，保证数据包按顺序发送到。

它是一种可靠的，一对一的，面向有连接的通信协议。



###### UDP协议（User Datagram Protocol）

即用户数据包协议，它是面向无连接的协议，只要知道对方的IP地址和端口，就可以直接发送数据包，由于面向无连接的，所以无法保证数据一定会到达接收方。



###### 端口号

区分计算机中的运行的应用程序的整数。

端口号的取值范围是0到65535，一共655356个，其中80这个端口号分配给了HTTP服务，21这个端口分配给了FTP服务。



### 了解七层协议与四层协议

四层协议和七层协议：

应用层（应用层，表示层），传输层（会话层，传输层），网际层（网络层），网络接口层（数据链路层，物理层）



### 掌握Socket套接字

Socket：描述IP地址和端口号 

```python
# bind((ip,port))绑定ip地址和端口
# listen(N)开始TCP监听，N表示操作系统挂起的最大连接数量，取值1-5
# accept()被动接收TCP客户端连接，阻塞式
# connect((ip,port))主动初始化TCP服务器连接
# recv(size)接受TCP数据，返回值为字符串类型，size表示要接收的最大数据量
```



### 熟练应用TCP编程

#### # TCP服务器端流程：

```python
# TCP服务器端流程：
from socket import socket,AF_INET,SOCK_STREAM

# 1.创建socket对象
sever_socket=socket(AF_INET,SOCK_STREAM)
# 2.绑定IP地址和端口
ip='127.0.0.1'# local
port=8888 # 端口范围
sever_socket.bind((ip,port))

# 3.使用listen监听
sever_socket.listen(5)
print('服务器已启动')

# 4.等待客户端连接
client_socket,client_addr=sever_socket.accept() # 系列解包赋值

# 5.接收来自客户端的连接
data=client_socket.recv(1024)
print('客户端发送过来的数据为：',data.decode('utf-8')) # 要求客户端发送的数据采用utf-8解码

# 6.关闭socket
sever_socket.close()
```

#### # TCP客服端代码流程：

```python
# TCP客服端代码流程：
import socket

# 1.创建Socket对象
client_socket=socket.socket()
# 2.IP地址和主机端口，向服务器发送连接请求
ip='127.0.0.1'
port=8888
client_socket.connect((ip,port))
print('-----与服务器连接成功------')

# 3.发送数据
client_socket.send('Welcome to python world'.encode('utf-8'))

# 4.关闭
client_socket.close()
print('发送完毕')
```



### 熟练应用UDP编程

#### # UDP发送

```python
# UDP发送
from socket import socket,AF_INET,SOCK_STREAM

# 1.创建socket对象
send_socket=socket(AF_INET,SOCK_STREAM)

# 2.准备发送数据
data=input('请输入要发送的数据')
# 3.指定接收方的ip和端口
ip_port=('127.0.0.1',8888)
# 4.发送数据
send_socket.sendto(data.encode('utf-8'),ip_port)

# 5.关闭socket对象
send_socket.close()
```

```python
# UDP接收
from socket import socket,AF_INET,SOCK_STREAM

# 1.创建socket对象
recv_socket=socket(AF_INET,SOCK_STREAM)

# 2.绑定ip地址和端口
recv_socket.bind('127.0.0.1',8888)
# 3.接受来自发送方的数据
recv_data,addr=recv_socket.recvfrom(1024)
print('接收到的数据为：',recv_data.decode('utf-8'))

# 4.准备回复对方的数据
data=input('请输入回复数据')
# 5.回复
recv_socket.sendto(data.encode('utf-8'),addr)

# 6.关闭
recv_socket.close()
```
