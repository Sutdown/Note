

# python全栈开发教程1



目测第一阶段主要在于基础知识，路线先是从b站视频python[全栈开发教程](https://b23.tv/JxBsQTg)，算是入门学习。

第二阶段会重开文章，这篇主要针对一阶段的学习历程，二阶段[北理-Python数据分析与展示-Numpy、Matplotlib、Pandas-哔哩哔哩](https://b23.tv/jUfXCvD) 。

其它的内容就再看具体情况。

——2024.01.11



## python全栈开发教程P1-P58

### 基础语法

（存在c++基础，只记录python）

python是一种**解释性语言**。

python**语言特点**：语法简洁，平台无关，粘性扩展，开源理念，通用灵活，强制刻度，支持中文，模式多样，类库丰富。

python**应用领域**：Web开发，数据分析与科学计算，人工智能和机器学习，自动化测试和运维，网络爬虫，游戏开发。



```python
# 1.print
a=100
print(a)
print('hello,python')
print(chr(98)) # ASCII码表
print(ord('北')) # 中文编码表

fp=open('node.txt','w')
print('北京欢迎你',file=fp)
fp.close()

print(a,end='--->')
print('hi')

print('hello='+'hi') # +只能加两个字符串

# 2.input
name=input('put your name:')
print('我的名字是：'+name)

# 3.代码缩进
```

```python
# 1.保留字
# 查询关键字
import keyword
print(keyword.kwlist)
print(len(keyword.kwlist))

# 2.标识符
# _开头是protected的
# __开头是private的

# 3.变量与常量
# python可以动态修改数据类型
# 规定所有字母大写为常量
luck_number=8
print('luck_number数据类型:',type(luck_number))
luck_number='hello'
print('luck_number数据类型:',type(luck_number))

# 4.数值类型
# 5.字符串类型
# 6.字符串的索引和切片
# 7.布尔类型
# 8.数据类型转换

# 9.
# eval()去调字符串的引号，执行里面的运算
# round()保留几位小数

# 10.算术运算符
# //整除 /除法 **幂运算
# 交换a,b=b,a
# 运算符优先级
```

```python
# 1.顺序结构，选择结构，双分支结构，多分支结构,嵌套
# 多分支：碰见一个正确的执行并退出
score=eval(input('please write your grade:'))
if 0<=score<60:
    score = 'C'
elif 60<=score<80:
    score = 'B'
else:
    score = 'A'

# 2.模式匹配match-case结构
match score:
    case 'A':
        print('优秀')
    case 'B':
        print('良好')
    case 'C':
        print('不及格')

# 3.循环结构(for,while)
# range为[)结构的内置函数
a=0
for i in range(1,11):
    a+=i
# for(while)...else...循环正常执行完后执行else部分

# 4.break continue
# 5.空语句pass:只起占位符作用，使语法结构完整不报错，一般可用在if，for,while,函数定义，类定义中
```

```python
# 1.序列：用于存储多个值的连续空间
# 索引和切片

### 四个组合数据类型
## 2.列表
# 指一系列按照特定顺序排列的元素组成，是python中内置的可变序列。
# 创建方式
# 1）列表名=[ele1,ele2,...,eleN]
# 2)内置函数list()。列表名=list(序列)
lst=['hello','world','!']

# 列表遍历
# 1）for（in）
for i in lst:
    print(i,end=' ')
print()
# 2)索引（for，range，len）
for i in range(0,len(lst)):
    print(i,'--->',lst[i])
# 3）enumerate函数语法结构
for index,item in enumerate(lst):
    print(index,item)

# 列表方法
# append,inset(index,x),clear(),pop(index),remove(x),reverse,copy

# 列表生成式
# lst=[expression for item in range]
# lst=[expression for item in range if condition]

print('-'*25)

## 3.元组
# 是python内置的不可表序列
# 创建方式
# 1）元组名=（ele1,ele2,...,eleN）
# 2)内置函数tuple()
t=('元','组',[1,2,3])
print(t)

# 元组遍历
# 1)for
for item in t:
    print(item,end=' ')
print()
# 2)for+range()+len()
for i in range(len(t)):
    print(i,'--->',t[i])
# 3)
for index,item in enumerate(t,start=1):
    print(index,'--->',item)

print('-'*25)

## 4.字典
# 字典类型是根据一个信息查找另一个信息构成“键值对”，它表示索引用的键和对应的值构成的成对关系。
# 创建方式
# 1）d={key1:value1,key2,value2,...}
# 2)zip(lst1,lst2)
lst1=[1,2,3,4]
lst2=['cat','dog','zoo','car']
d=dict(zip(lst1,lst2))
print(d)

# 语法结构
# dict(key1=value1,key2=value2,...)

# 字典遍历
# 1）for element in d.items():
#         pass
# 2) for key,value in d.items():
#         pass

# 字典生成式
# d={key:value for item in range}
# d={key:value for key,value in zip(lst1,lst2)}

## 5.集合
# 是无序的不重复元素序列，存储不可变数据类型

# 创建方式
# 1)s={ele1,ele2,...eleN}
# 2)s=set(可迭代对象）
```

