# python全栈开发教程2

### 字符串和正则表达式

```python
### 字符串和正则表达式
import re

## 字符串的常用操作
# lower(),upper()
# split(sep=None)
# count(),find(),index()
# startswith(s),endswith(s)
# replace(old,new),center(width,fillchar)
# join(iter)
# strip(chars),lstrip(chars),rstrip(chars)

name = 'Amy'
score = 98
## 格式化字符串的方式
# 1）%s,%d,%f
print('name: %s,score: %d' % (name,score))
# 2)f-string
print(f'name: {name},score: {score}')
# 3)str.format()
print('name: {0},score: {1}'.format(name,score))

## 字符串的编码和解码

## 数据的验证
# isdigit(),isnumeric()
# isalpha(),isalnum()
# islower(),isupper()
# istitle(),isspace()

## 字符串的处理
# 字符串拼接
# 1）+
# 2)' '.join()
# 3)直接拼接
# 4）格式化字符串拼接
s='pythhhhonnn'
new_s=''
# 字符串去重
# 1）字符串拼接/索引+not in
for item in s:
    if item not in new_s:
        new_s+=item
print(new_s)
# 2)集合去重+列表排序
new_s1=list(set(s))
new_s1.sort(key=s.index)
print(''.join(new_s1))

## 正则表达式
# 元字符：具有特殊意义的专用字符。
# 限定符：用于限定匹配的次数
## re模块
# re.match(pattern,string,flags=0)(从开始位置匹配）
# re.search(pattern,string,flags=0)（搜索第一个匹配的值）
# re.findall(pattern,string,flags=0)（查找所有）
# re.sub(pattern,repl,string,count,flags=0)
# re.split(pattern,string,maxsplit,flags=0)
```

### 异常处理

```python
## 异常处理
# raise
# 抛出一个异常，从而提醒程序出现了异常情况，程序能够正确的处理这些异常情况
# 语法结构：raise [Exception类型(异常描述信息)]
# 常见异常类型：
# ZeroDivisionError,IndexError,KeyError,NameError,
# SyntaxError,ValueError,AttributeError,TypeError,IndentationError
```

### 函数

```python
## 异常处理
# raise
# 抛出一个异常，从而提醒程序出现了异常情况，程序能够正确的处理这些异常情况
# 语法结构：raise [Exception类型(异常描述信息)]
# 常见异常类型：
# ZeroDivisionError,IndexError,KeyError,NameError,
# SyntaxError,ValueError,AttributeError,TypeError,IndentationError

## 函数
# 函数的参数传递
# 位置参数，关键字参数，默认值参数，
# 可变参数（*para个数可变的位置参数，**para个数可变的关键字参数）

# 函数的返回值
# 变量的作用域
# 函数的递归操作

# 匿名函数lambda的使用
s=lambda a,b:a+b
print(type(s))
print(s(10,20))

print('-'*15)

lst=[10,20,30,40]
for i in range(len(lst)):
    result=lambda x:x[i]
    print(result(lst),end=' ')
print()

# 内置函数
# 类型转换函数，数组函数，迭代器操作函数，其它函数
```
