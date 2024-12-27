#### 共享锁和独占锁的互斥

> terminate called after throwing an instance of 'std::system_error'
>   what():  Resource deadlock avoided
> Aborted (core dumped)（死锁导致中止）

解决：

`gdb`定位 idle中发生死锁

```debug
IOManager::idle(),run in thread: 27027
terminate called after throwing an instance of 'std::system_error'
  what():  Resource deadlock avoided
[1] + Done                       "/usr/bin/gdb" --interpreter=mi --tty=${DbgTerm} 0<"/tmp/Microsoft-MIEngine-In-5lhsknfi.0tc" 1>"/tmp/Microsoft-MIEngine-Out-hcmtf4bw.gm1"
```

然而idle唯一的锁只有，经排查，应该跟idle无关，可能是其它部分

```cpp
std::lock_guard<std::mutex> lock(fd_ctx->mutex);
```

单步调试发现是在构造函数时便出现了core dump,然而不是

查看gdb的堆栈信息之后，迅速确认**问题在于1.死锁可能在多个线程共享锁上冲突 2.异常发生在`IOManager::addEvent`方法中**

可能的死锁模式是：线程 A 持有共享锁，线程 B 持有共享锁，且线程 A 试图请求一个被线程 B 持有的锁，或者线程 B 试图请求线程 A 持有的锁。

最终原因：

本来为访问fdcontext数组的访问设置了一个共享（读）锁，由于fd不在数组中，之后又设置了一个（独占）写锁去为fdcontext数组扩容，由于共享锁和独占锁是互斥的，会出现core dump。



#### git:GnuTLS recv error (-110)

这个指的是在于Github建立TLS连接时发生的问题。原因是TLS连接关闭不正常，应该是我用代理vpn在网速比较满的时候git push太慢我就直接ctrl+z将它中止然后出了问题。



其实也之前用https连接断断续续出现过其它错误，

比如连接不上端口，但是此时配置代理就没问题了

```bash
Failed to connect to github.com port 443 

配置代理
git config --global http.proxy 127.0.0.1:7890
git config --global https.proxy 127.0.0.1:7890
```



但是当代理的网速太慢时，会发生超时

```bash
fatal: unable to access 'https://github.com/name/xxx.git/': Failed to connect to 127.0.0.1 port 7890 after 0 ms: Couldn't connect to server

git config --global -l 查看代理

git config --global --unset http.proxy 清除代理
git config --global --unset https.proxy
```



可我清除代理之后会出现

```bash
GnuTLS recv error (-110): The TLS connection was non-properly terminated.
```



第一个回答：给了个比较粗暴的方法，直接禁用TLS验证`git config --global http.sslVerify "false"`亲测，对我没用。



第二个回答：尝试更新Git和GnuTLS版本，可能版本太旧了，下面的命令，亲测，再次没有效果。

```bash
sudo apt-get update
sudo apt-get install git
sudo apt-get install --only-upgrade gnutls-bin
```



最后一次，gpt万岁，它推荐我用SSH协议代替HTTPS进行git操作，成功，具体流程如下：（我是ubuntu 24.04)

查看是否有SSH密钥

```
ls -al ~/.ssh 
```

生成新的密钥，会让你输入一些路径之类的信息，最好直接enter默认即可

```bash
ssh-keygen -t ed25519 -C "your_email@example.com"
```

将SSH添加到SSH代理

```bash
eval "$(ssh-agent -s)"
ssh-add ~/.ssh/id_ed25519
```

查看公钥，添加到github账户中，[Github SSH 密钥设置页面](https://github.com/settings/keys)

```
cat ~/.ssh/id_ed25519.pub
```

测试SSH连接

```
ssh -T git@github.com

Hi your-username! You've successfully authenticated, but GitHub does not provide shell access.
```

更新Git远程连接的URL

```
git remote -v 检查git的远程URL
git remote set-url origin git@github.com:your-username/repository-name.git 更新
```

之后便好了。



推荐链接：

[VScode中git push代码报错“Missing or invalid credentials.”问题解决记录 - 知乎](https://zhuanlan.zhihu.com/p/653213652)

[Git报错： Failed to connect to github.com port 443 解决方案-CSDN博客](https://blog.csdn.net/zpf1813763637/article/details/128340109)

[Git Clone错误解决：GnuTLS recv error (-110): The TLS connection was non-properly terminated.-CSDN博客](https://blog.csdn.net/qq_36393978/article/details/127531104)

