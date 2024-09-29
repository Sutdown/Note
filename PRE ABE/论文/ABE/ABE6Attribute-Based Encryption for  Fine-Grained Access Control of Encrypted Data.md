### Attribute-Based Encryption for Fine-Grained Access Control of Encrypted Data

[属性基加密细粒化访问](https://zhuanlan.zhihu.com/p/386078331)



KPABE的介绍文章。KPABE的访问树怎么跟CPABE的访问树一样一样的。



##### Introduction

由于更敏感的数据在第三方网站在互联网上共享和存储，因此需要对存储在这些网站上的数据进行加密。加密数据的一个缺点是仅在粗粒度级别共享（即给另一方我们的私钥），限制了用户以细粒度级别选择性地共享其加密数据的能力。

例如假设Alice想要授予Bob对其所有网络流量日志的解密访问权限，以获取特定日期内来自特定网段的所有条目。Alice要么只能解密所有相关条目，要么必须向Bob提供其解密私钥，从而使其可以访问所有条目。这么做就很不方便了。

•KP-ABE (key-policy ABE) （基于密钥策略的属性加密）是将策略嵌入到密钥中，属性嵌入到密文中。**密钥中实现访问密文中不同的数据。**

•CP-ABE(ciphertext-policy ABE)（基于密文策略的属性加密）是将策略嵌入到密文中，属性嵌入到密钥中。**密钥中实现不同属性用户的访问。**



###### IBE和CPABE

CPABE中，密钥的属性满足访问策略时才能成功解密密文，特定密钥才能解密密文。

从唯一标识符扩展成属性集合，不仅是用户身份表示方式上的改变，属性集合也能很方便的和访问结构相结合，实现对密文和密钥的访问控制，属性集合也可以方便的表示某些用户组的身份，实现了一堆多通信，这也是ABE的优势。



##### 相关工作

###### Fine-grained Access Control

有助于向一组用户授予不同的访问权限，允许灵活的指定单个用户的访问权限。

本文介绍的是，数据以加密的形式存储在服务器上，不同的用户可以允许被解密不同的数据,有效的消除了需要依赖存储服务器来防止未经授权的数据访问的需要。



###### Secret-Sharing Schemes秘密共享方案

在SSS中，指定一种访问树结构，内部节点由AND和OR门组成，叶子由不同参与方向组成，满足该树的任何一方都可以聚在一起重建秘密，因此在SSS中，允许不同用户之间的串通。

本文中，每个用户的密钥和一个访问树结构相关联，叶子是和属性相关联。（策略在密钥中，属性在密文中）如果与密文相关联的属性满足密钥的访问结构，那么用户可以解密密文，与SSS相关，此时用户无法进行串通合谋。



###### ABE

ABE之前是IBE和FIBE，也就是用个人身份作为公钥去加密，FIBE将身份视为一组属性，只要属性大体满足便可以访问，采用的是门限阈值的防止。ABE则是对于属性有了更广泛的一个扩展



##### KP-ABE

###### Access Structure

<img src="F:\论文\Week\image\61.jpg" alt="61" style="zoom:75%;" />

###### 选择性ID模型（CPA安全）

在各个论文出现好多次了。。。

![63](F:\论文\Week\image\63.jpg)

###### Bilinear Maps双线性映射

###### The Decisional Bilinear Diffie-Hellman DBH假设

###### Access tree

之前哪篇文章详细看过这个，就略了。

###### Lagrange coefficient拉格朗日插值

![64](F:\论文\Week\image\64.jpg)

###### KPABE算法

密钥策略表示成一个访问树

密文由简单的属性集合参与生成

如果一个用户能解密密文并且仅当密文的属性集合满足密钥的密钥策略时才能解密。能够增强密钥策略的逻辑表达能力，实现更好的细粒化访问策略。

<img src="F:\论文\Week\image\62.jpg" alt="62" style="zoom:80%;" />

Setup：安全参数；输出公共参数PK和主密钥MK

Encryption：随机算法，输入m，一组属性$\gamma$，公共参数PK；输出密文E

Key Generation：随机算法，输入访问结构A，主密钥MK，公共参数PK；输出解密密钥D

Decryption：输入为属性集合$\gamma $下加密的密文E，访问控制结构A的解密密钥D，公共参数PK；如果$\gamma \in A$，输出消息M。



##### Construction

###### Setup

<img src="F:\论文\Week\image\65.jpg" alt="65" style="zoom: 100%;" />

随机生成PK和MK，根据是否复合属性要求划分出两类公钥。

###### Encryption

![66](F:\论文\Week\image\66.jpg)

如果符合$\gamma $属性，利用公钥中的T进行加密运算，否则利用公钥中的随机数进行加密运算。

###### Key Generation

![67](F:\论文\Week\image\67.jpg)

###### Decryption

<img src="F:\论文\Week\image\68.jpg" alt="68" style="zoom:70%;" />



##### Proof of Security

略



##### Large Universe Construction

之前的构造中，公共参数的大小随着universe中可能的属性数量线性增长。此处构造了另一种$Z^*_p$的所有元素作为属性的方案，公共参数仅在参数n中线性增长。n固定为可加密的最大集合大小。

![69](F:\论文\Week\image\69.jpg)

跟那个CPABE访问树一模一样。。。绝了



##### Delegation or Private Keys

在large Universe构建中，单个用户可以使用私钥生成新的私钥委派给其他用户。具有与访问树T相对应的私钥的用户可以计算比T更严格的任何访问树T’相对应的新的私钥。因此，用户能充当可以生成私钥并非分发给其它本地用户的本地密钥授权机构。

1. adding a new trivia gate to T在现有节点上添加新的节点

2. manipulating an existing(t,n)-gate in T

   2.1 将可变的阈值在一定范围内变化a/b，a可以在小于b的情况下+1

   2.2 a/b，a和b可以同时+1

   2.3 删除x节点的孩子节点。从原始解密密钥中删除所有x节点以下的树的所有节点

3. re-randomizing the obtained key重新随机化





##### KPABE的应用

- 审计日志应用程序

  只提取和目标有关的日志内容，避免日志中全部内容的暴露

- 定向广播

  用户根据个人喜好制定接收的节目。



##### Discussion And Extensions

1. 将委托技术用于实现有效的CCA安全和HIBE（基于分层身份的加密）
2. 允许用复杂策略的CPABE存在很多应用。比如复杂的广播加密中，用属性描述用户，创建一个只有用户属性与策略匹配时才能打开的密文。
3. 当前的结构并未隐藏对数据进行加密的属性集，如果可以隐藏这些属性，那么也就可以将这些属性中视为系统的关键字，这回导致加密数据进行基于关键词的搜索。
