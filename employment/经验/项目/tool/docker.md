# Docker

（宿主机上的一个进程）

docker使用**namespace**去提供称为容器的工作空间

docker引擎在linux中会使用以下命名空间：

- pid 进程id，进程隔离 (process id)
- net 管理网络接口 (networking)
- ipc 管理进程间通信资源 (interprocess communication)
- mnt 管理文件系统挂载(mount)
- uts 隔离内核和版本证明 （unix timesharing system)



cd /proc/

cd 1

ll ns/

容器的命名空间和宿主机的命名空间是隔离的

man手册



**control groups**

限制应用的资源集合

允许docker引擎共享容器的硬件资源



限制，控制，分离一个进程组的资源



**union file sysytem**

通过创建层使得它能轻量级和快

**container format**

docker引擎包含namespace,control groups和union file system将这三者进行一个封装，称为container format，这个默认的容器格式是libcontainer.



dockerfile

docker build -t my-node-app . 构建镜像

docker pull

docker run -p 3000:3000 my-node-app 允许基于镜像的容器

