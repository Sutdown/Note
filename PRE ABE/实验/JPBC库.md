# JPBC库



pairing-based cryptography library

基于配对的加密库



[JPBC库（基于配对的密码学）入门和避坑指南-CSDN博客](https://blog.csdn.net/weixin_44960315/article/details/107325591)

[基于配对的密码学——基础知识及JPBC库](https://blank-vax.github.io/2021/07/05/%E5%9F%BA%E4%BA%8E%E9%85%8D%E5%AF%B9%E7%9A%84%E5%AF%86%E7%A0%81%E5%AD%A6%E2%80%94%E2%80%94%E5%9F%BA%E7%A1%80%E7%9F%A5%E8%AF%86%E5%8F%8AJPBC%E5%BA%93/)

[属性加密访问结构(一)---访问控制树(超详细) - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/582162993)

[数学、英语对程序员来说重要吗？记线性秘密分享方案（Linear Secret Sharing Scheme，LSSS）的实现 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/21664741)



[TOC]

#### 代码：

##### 1

```java
Field G1 = bp.getG1();
Element y = G1.newElement(4).getImmutable();
```

在该代码中，程序出现`IllegalStateException`异常，这是由于bp.getG1()方法可能返回非G1子群。

另外，如果程序出现`IllegalArgumentException`异常，则是由于G1.newElement(4)创建的元素不在G1子群中。

##### 2

JPBC 库中的四个循环子群是：

1. **G1:** 椭圆曲线群，加法群，元素是椭圆曲线上的点。
2. **G2:** 椭圆曲线群，加法群，元素是椭圆曲线上的点。
3. **GT:** 配对群，乘法群，元素是 G1 和 G2 元素配对的结果。
4. **Zr:** 有限域，乘法群，元素是有限域中的数字。

**以下是四个循环子群的详细介绍：**

**1. G1 子群:**

- G1 子群是椭圆曲线群，其元素是椭圆曲线上的点。
- G1 子群的运算包括加法、减法和标量乘法。
- G1 子群常用于基于身份的加密 (IBE) 和签名方案。

**2. G2 子群:**

- G2 子群是另一个椭圆曲线群，其元素也是椭圆曲线上的点。
- G2 子群的运算与 G1 子群类似，包括加法、减法和标量乘法。
- G2 子群常用于双线性映射签名方案和属性加密方案。

**3. GT 子群:**

- GT 子群是配对群，其元素是 G1 和 G2 元素配对的结果。
- GT 子群的运算包括乘法、除法和幂运算。
- GT 子群常用于双线性映射签名方案和属性加密方案中的加密和解密操作。

**4. Zr 子群:**

- Zr 子群是有限域，其元素是有限域中的数字。
- Zr 子群的运算包括加法、减法、乘法和除法。
- Zr 子群常用于椭圆曲线密码学中的各种计算，例如椭圆曲线积分和密钥生成。

**JPBC 库中的四个循环子群相互关联，并在椭圆曲线密码学中发挥着重要作用。**

**以下是一些关于四个循环子群的额外信息：**

- G1 和 G2 子群的阶通常是素数。
- GT 子群的阶是 G1 和 G2 子群阶的乘积。
- Zr 子群的阶是有限域的大小，通常也是素数。
- JPBC 库提供了各种方法来创建和操作四个循环子群中的元素。

##### 循环群

JPBC库共提供**四个循环群**，其中$G_1,G_2,G_T$均为**阶为p的乘法循环群**，而$Z_P$为**整数域上的加法循环群**。乘法循环群上的点是z值为0的椭圆曲线上的点，而整数循环群上的点是数，二者均可抽象为`Element`数据类型并用于仿真中。生成测试元素并打印，

- $G_1,G_2,G_T$中元素的**模幂运算、倍乘运算以及互相之间的加法运算**，运算结果均为对应**群**上的元素。
- $Z_p$中元素的**加减乘除运算及乘方**运算，运算结果为**整数循环群**上的元素。

注：

JPBC库中将$G_1,G_2$表示成了乘法循环群，因此在加法循环群形式方案的仿真过程中，应特别注意将加法群改写为乘法群的写法再完成进一步仿真。

由于加法群中的加法运算对应乘法群中的乘法运算，减法运算对应除法运算（即求逆元），乘法运算对应幂指数运算，而除法运算对应对数运算。故改写过程需要结合以上运算法则。

##### 函数

###### getG1()

`getG1()` 函数用于获取椭圆曲线群 G1。G1 是一个加法群，由椭圆曲线上的点组成。

###### newElement()

`newElement()` 函数用于创建一个椭圆曲线群 G1 中的新元素。该元素可以是椭圆曲线上的点，也可以是无限远点。

###### newRandomElement()

`newRandomElement()` 函数用于创建一个椭圆曲线群 G1 中的新随机元素。该元素是椭圆曲线上的一个随机点。

```java
// 创建一个特定的元素
Element g = G1.newElement(x, y);

// 创建一个随机元素
Element h = G1.newRandomElement();
```

###### Base64

将非ASCII字符的数据转换成ASCII字符的一种方法。把任意长度的二进制数据变为纯文本。

会将原始数据长度增加1/3

```java
编码
mskProp.setProperty("g_alpha", Base64.getEncoder().withoutPadding().encodeToString(g_alpha.toBytes()));//转为纯文本并且设置到prop中

解码
String gString = pkProp.getProperty("g");
Element g = bp.getG1().newElementFromBytes(Base64.getDecoder().decode(gString)).getImmutable();
```

[加密与安全 - 廖雪峰Java](https://www.liaoxuefeng.com/wiki/1252599548343744/1255943717668160)

##### 哈希算法

- 相同的输入一定得到相同的输出；
- 不同的输入大概率得到不同的输出。

可能发生哈希碰撞

SHA-1 输出160bits

##### 拉格朗日插值算法

![1](E:\0\大创\image\1.png)

利用多项式，使得满足一定量的属性就可以还原出原多项式的思想。

##### Shamir秘密共享

![2](E:\0\大创\image\2.png)

在拉格朗日算法基础上的算法。



#### 代码(IJ)

ABE是IBE的扩展。

##### BLS

##### IBE

##### FIBE

##### CPABE

##### KBABE

###### CPABE和KPABE的整体框架：

setup：初始化参数，生成加密密钥

keygen：生成密钥

encrypt：利用加密算法和密钥对明文加密

decrypt：使用解密算法



###### 将算法的式子思路以代码的形式展现



###### 密码等信息存储

Pairing, Element,

Properties

文件



##### Node

##### accesstree

访问树属性结构,秘密共享和秘密恢复



