- Ateniese, Giuseppe, et al. "Improved proxy re-encryption schemes with applications to secure distributed storage."

   (2006)

  - 论文链接：Improved Proxy Re-Encryption Schemes with Applications to Secure Distributed Storage
  - 这篇论文介绍了代理重加密的基本概念和第一个显著的方案，适合初学者理解基础。

注意：

1.可以去看Blaze，Bleumer，Strauss1988年提出的原子代理重加密的应用程序



### 标题

> Improved Proxy Re-encryption Schemes with Applications to Secure Distributed Storage
>
> 改进的代理重加密方案及其应用程序以保护分布式存储



本文中的代理重加密，相比1998年BBS原子代理重加密（半可信代理将Alice公钥计算的密文转换为Bob私钥打开可以看到的密文，无需看到底层密文），实现了更强的安全概念



###### 优点

本方案的优点在于

只对代理给予了**有限**的信任。比如，**代理无法解密重新加密的信息**

该方案是**单向**的，并且**不需要委托者向人和人透露它们所有的秘密密钥**，甚至**不需要和受委托者进行交互**。



使用**集中式访问控制服务器**，管理存储在分布式，不可信副本上的加密内容的访问，用代理重加密实现集中管理的发个文控制，无需向访问控制服务器授予完全解密权限。



##### BBC 原子代理重加密方案



单向，秘密共享



#### 理论和实现



##### 2 方案 理论基础

###### 理论基础：

- 双线性映射

- 单项代理重加密



3 改进的代理冲加密方案

![9](F:\论文\Week\image\97.jpg)

![98](F:\论文\Week\image\98.jpg)

![99](F:\论文\Week\image\99.jpg)

![100](F:\论文\Week\image\100.jpg)

![101](F:\论文\Week\image\101.png)









