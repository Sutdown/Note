1.

介绍一下tcp项目。

模拟基本的linux内核操作是怎么做到的

virtualbox，docker，vmware有什么区别

连接管理：connect，listen，accept

可靠数据传输：

1）序列号和确认应答 2）快速重传  超时重传 SACK D-SACK 3）滑动窗口

流量控制：

1）缓冲区和滑动窗口关系 2）窗口关闭 3）不考虑糊涂窗口综合症，nagle算法

拥塞控制

慢启动 拥塞避免 拥塞发生 快速恢复



##### start simulation()

1.初始化：初始化监听socket哈希表和建立连接哈希表

2.获取主机名，设置了一个UDP socket在某个端口监听，TCP就是对UDP增加了一些机制

3.pthread_creat创建线程，线程中存在一个数据接收receive_thread。

   仿真不断接收数据的线程，其中利用的是UDP绑定的端口。接收完之后释放。

​      通知内核收到一个完成的TCP报文，再释放。收到TCP报文时，从报文中获取远程IP和端口以及本地ID和端口，再根据这个四元组查找建立连接的socket哈希表，然后开始握手；没有的话查找监听中的哈希表，再经历tju_handle_packet.。

