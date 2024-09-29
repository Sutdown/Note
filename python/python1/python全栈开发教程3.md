# python全栈开发教程3



## 面向对象程序设计

```python
### 面向对象程序设计
## 类的定义和对象的创建
## 属性和方法的调用
# 实例属性：定义在_init_方法中，使用self打点的变量
# 实例方法：定义在类中的函数，而且自带参数self
# 静态方法：使用装饰器@staticmethod修饰的方法
# 类方法：使用装饰器@classmethod修饰的方法
class Student:
    school='北京XX教育'

    def __init__(self,xm,age):
        self.name=xm
        self.__age=age

    @property # 属性
    def age(self):
        return self.__age

    @age.setter
    def age(self,value):
        if value<0:
            print('年龄输入有错')
            self.__age='None'
        else:
            self.__age=value

    def show(self):
        print(f'my name is {self.name}')
        print(f'my age is {self.age}')

    @staticmethod
    def sm():
        print('静态方法，不能调用实例属性和实例方法')

    @classmethod
    def cm(cls):
        print('类方法，不能调用实例属性和实例方法')

stu=Student('Amy','20')
print(stu.name,stu.age)
print(Student.school)
print(type(stu))
stu.show()

Student.cm()
Student.sm()

## 面向对象的三大特征
## 动态语言的特点
# 封装，继承，多态
class Person(Student):
    def __init__(self,name,age,stuno):
        super().__init__(name,age)
        self.stuno=stuno

## Object类的常用方法
# object类中特殊的方法
# _new_()由系统调用，用于创建对象
# _init_()创建对象时手动调用，用于初始化对象属性值
# _str_()对象的描述，返回值是str类型，默认输出对象内存地址

## 深拷贝和浅拷贝
# deepcopy和copy
```



## 模块

```python
### 模块

## 自定义模块的创建

## 模块的导入
# 1)import ...
# 后面的模块中的函数覆盖前面相同的函数
# 2)from ... import ...

## python中包的定义
# 含有_init_.py文件的文件夹(目录）
# 可以避免模块名称相冲突的问题
# 主程序运行：
# if _name_=='_main_':
#        pass

## python中常用的内置模块
# 在安装python解释器时于解释器一起安装进来的模块称为系统内置模块（标准模块/标准库）
# os模块，re模块，random模块，json模块，time模块，datetime模块

## python中常用的第三方模块
# pip install ...
# pip uninstall ...
```

```python
# requests库：
# 处理HTTP请求的第三方库，在爬虫中应用广泛
# 使用其中的get()可以打开网络请求，获取一个response响应对象。响应结果中的字符串数据可以通过响应对象的text属性获取，响应结果中除了有字符串数据也有二进制数据，响应结果中的二进制数据可以通过响应对象的content属性获取。
import requests
url='https://feyxiang.top/'
resp=requests.get(url)
resp.encoding='utf-8'
# print(resp.text)

txt=re.findall('<span class="text">([\u4e00-\u9fa5]*)</span>',resp.text)
print(txt)

# [正则表达式](https://www.runoob.com/regexp/regexp-intro.html)
```

```python
# openpyxl模块
# 用于处理Microsoft Exel文件的第三方库
# 可以对Excel文件中的数据进行写入和读取
# 函数功能：
# load_workbook(filename),workbook.sheetnames,
# sheet.append(lst)
# workbook.save(excelname),Workbook()
```

```python
# pdfplumber可用于从PDF文件中读取内容
# Numpy是python数据分析方向和其它库的依赖库，用于处理数组，矩阵等数据
# Pandas是基于Numpy模块扩展的一个非常重要的数据分析模块，使用Pandas读取Excel数据更加的方便
# Matplotlib是用于数据可视化的模块，使用Matplotlib.pyplot可以非常方便的绘制饼图，柱形图，折线图等。
```

```python
# PyEcharts是由百度开源的数据可视化库，它对流行图的支持度比较高。
# 为用户提供了三十多种图形，如柱形渐变图，K线周期图等
# 帮助文档：https://pyecharts.org/#/zh-cn/

# PIL用于图像处理的第三方库，支持图像存储，处理和显示等操作
# jieba是Python中用于对中文进行分词的模块，它可以将一段中文文本分隔成中文词组的序列。

# PyInstaller第三方库可以在Windows操作系统中将Python源文件打包成.exe的可执行文件。
# 也可以在linux和MacOS操作系统中对源文件进行打包操作
```