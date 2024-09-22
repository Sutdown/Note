# python全栈开发教程4

## 文件及相关操作

```python
### 文件以及IO操作
## 掌握文件读写的基本操作
# 操作文件的步骤
# 1）打开文件：变量名=open(filename,mode,encoding)
# 2)操作文件：变量名.read();变量名.write(s)
# 3)关闭文件：变量名.close()
def my_write():
    #file=open('a.txt','w',encoding='utf-8')
    #file.write('危楼高白尺，手可摘星辰')
    #file.close()
    lst=['Amy','John','Eric']
    with open('stu.csv','w') as file:
        file.write(','.join(lst))

def my_read():
    #file=open('a.txt','r',encoding='utf-8')
    #s=file.read()
    #print(type(s),s)
    #file.close()
    with open('stu.csv','r') as file:
        s=file.read()
        lst=s.split(',')
        print(lst)

if __name__ == '__main__':
    my_write()
    my_read()

## 掌握with语句的使用
# with语句：上下文管理器。
# 在处理文件时，无论是否产生异常，都能保证with语句执行完毕后关闭已经打开的文件，这个过程是自动的，无需手动操作。
# 语法结构：with open(...) as file: pass
def write_fun():
    with open('aa.txt','w',encoding='utf-8') as file:
        file.write('hello,python.')

def read_fun():
    with open('aa.txt','r',encoding='utf-8') as file:
        print(file.read)

def copy(src_file,target_file):
    with open(src_file,'r',encoding='utf-8') as file:
        with open(target_file,'w',encoding='utf-8') as file2:
            file2.write(file.read())

#if __name__ == '__main__':
#     write_fun()
#     read_fun()
#     copy('./aa.txt','./dd.txt')

## 熟悉数据的组织维度
## 掌握不同维度数据的存储
# 一维数据，二维数据，三维数据
# json模块常用函数
# json.dumps(obj),json.loads(s)python与JSON格式的编码解码
# json.dump(obj,file)将转换结果存储到文件file中
# json.load(file)从file中读入

## 掌握目录与文件的相关操作
# getcwd(),listdir(path)
# mkdir(path),makedirs(path)
# rmdir(path),removedirs(path),chdir(path)
# walk(path),remove(path),rename(old,new)
# stat(path),startfile(path)

## OS模块的使用
# import os
```

