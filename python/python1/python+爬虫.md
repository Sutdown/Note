# Python+爬虫

参考视频链接：

【Python+爬虫】爆肝两个月！拜托三连了！这绝对是全B站最用心（没有之一）的Python+爬虫公开课程，从入门到（不）入狱 ！】https://www.bilibili.com/video/BV1d54y1g7db?vd_source=3fb5d6e30320f23cfaa7814e883f9b2f



### 爬虫流程

1. 获取网页内容
2. 解析网页内容
3. 储存或者分析数据



###### HTTP请求

```py
# 请求行（方法类型，资源类型，资源路径）
# 请求头（给服务器的信息）
# 请求体（状态行，状态消息）
```



###### HTML：定义网页结构和信息

###### css：定义网页样式

###### JavaScript：定义用户和网页的交互逻辑



```python
import requests
# headers = {
#     "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/121.0.0.0 Safari/537.36 Edg/121.0.0.0"
# } # 冒充用户请求，但是网站只有临时的user-agent，不提供完整
res = requests.get("https://feyxiang.top/")
print(res.status_code) # 输出状态码
```

```python
from bs4 import BeautifulSoup
import requests
content = requests.get("https://feyxiang.top/").text
soup = BeautifulSoup(content,"html.parser")
all = soup.findAll("a",attrs={"class":"link"})
for a in all:
    print(a)
```



Next:

正则表达式

多线程

数据分析

数据库
