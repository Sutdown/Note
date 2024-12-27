### MySQL必知必会



> 最近在面试，连续两次关于`mysql`的基础题都没答上来，当初学的一般，也没有复习，太伤心了，但凡复习一下应该就没问题了/(ㄒoㄒ)/~~。痛定思痛，亡羊补牢。



MySQL是基于客户机服务器的DBMS。

**主键**是对于表中每一行的唯一标识，同一个表中某列代表的主键不能为空，不能重复。



##### 常见sql语法：

```sql
SHOW Datebase;
SHOW COLUMNS FROM customers;

SELECT prod_name
FROM products
ORDER BY prod_price, prod_name DESC # 如果有这两个键的联合索引，应该就能运行快点
LIMIT 1 OFFSET 2 # 从2开始的第一行，也就是第三行
```

```sql
SELECT prod_name, prod_price
FROM products
WHERE (vend_id = 1002 OR vend_id = 1003) AND prod_price >= 10 # AND的优先级比OR高
WHERE (vend_id IN (1002, 1003)) AND prod_price # IN和OR类似，但是比OR要快
```



##### 通配符

> like匹配整个列，如果被匹配的文本在列值中出现like不会找到它，相应的行也不被返回，除非使用通配符。
`%` 可以匹配0或者多个任意字符
`_` 只能匹配单个字符

```sql
SELECT prod_name
FROM products
WHERE prod_name LIKE 's%e' # s开头，e结尾的任意字符
# like不能匹配空

WHERE prod_name LIKE '_ ton anvil'
```



##### 正则表达式

> 可以解决更加复杂的过滤，MySQL仅仅支持多数正则表达式实现的一个很小的子集。
正则表达式不区分大小写，如果需要大小写，则在`REGEXP`之后加上`BINARY`。

```sql
SELECT prod_name
FROM products
WHERE prod_name REGEXP '1000' # 检索prod_name中包含文本1000的所有行，替代了like
ORDER BY prod_name

WHERE prod_name REGEXP '.000' # 检索prod_name中包含文本1000,2000,...,9000的所有行，替代了like

WHERE prod_name REGEXP '1000|2000' # OR操作

WHERE prod_name REGEXP '[123] Ton' # 匹配 1 Ton或者2 Ton或者3 Ton
WHERE prod_name REGEXP '[1-3] Ton'

WHERE vend_name REGEXP '\\.'  			 # 匹配'.'
WHERE prod_name REGEXP '[[:digit:]]{4}'  # 匹配连在一起的四个字符
```

| 元字符            | 说明                         |
| ----------------- | ---------------------------- |
| *                 | 0个或者多个                  |
| +                 | 1个或者多个                  |
| ？                | 0个或者1个                   |
| {n}               | 指定数目的匹配               |
| {n, }             | 不少于指定数目的匹配         |
| {n, m}            | 匹配数目的范围（m不超过255） |
| ^ / $             | 文本开始 / 文本结束          |
| [[:<:]] / [[:>:]] | 词的开始 / 词的结尾          |



##### 创建计算字段

> 通俗点可以理解成列。

拼接字段，使用别名
```sql
SELECT Concat(vend_name, ' (', vend_country), ')') AS tmp
FROM vendors
ORDER BY vend_name
```



##### 使用数据处理函数

> 处理文本串的文本函数
> 在数值数据上进行算术操作的数值函数
> 处理日期和时间值并从这些值中提取特定成分的日期和时间函数
> 返回DBMS正使用的特殊信息的系统函数

```sql
SELECT Upper(vend_name) AS vend_name_upcase
```



##### 汇总数据

> 确定表中行数
> 获得表中行组的和
> 找出表列的最大值，最小值和平均值

`AVG, COUNT, MAX, MIN, SUM`



##### 分组数据

```sql
SELECT vend_id, COUNT(*) AS num_prods
FROM products
WHERE 
GROUP BY vend_id
HAVING COUNT(*) >= 2
ORDER BY

SELETC order_sum, SUM(quantity*item_price) AS ordertotal
FROM orderitems
GROUP BY order_num
HAVING SUM(quantity*item_price) >= 50
```



```sql
SELECT 要返回的列或表达式
FROM 从中检索数据的表
WHERE 行级过滤
GROUP BY 分组说明
HAVING 组级过滤
ORDER BY 输出排序顺序
LIMIT 要检索的行数
```



##### 使用子查询

```sql
# 利用子查询过滤
SELECT cust_id
FROM orders
WHERE order_num IN (SELECT order_num
					FROM orderitems
					WHERE prod_id = 'TNT2')
					
# 从customer中检索客户列表，统计其在orders表中的订单数目
# 相关子查询
SELECT cust_name, 
	   cust_state, 
	   (SELECT COUNT(*)
		FROM orders
		WHERE orders.cust_id = customers.cust_id) AS orders
FROM customers
ORDER BY cust_name
```



##### 联结表

**外键** ：外键为某个表中的一列，它包含另一个表的主键值。

```sql
# 创建联结（默认内连接）
SELECT vend_name, prod_name, prod_price
FROM vendors, products
WHERE vendors.vend_id = products.vend_id
ORDER BY vend_name, prod_name

# 笛卡尔积
SELECT vend_name, prod_name, prod_price
FROM vendors, products
ORDER BY vend_name, prod_name

# 内部联结
SELECT vend_name, prod_name, prod_price
FROM vendors INNER JOIN products
			ON vendors.vend_id = products.vend_id;
```



##### 创建高级联结

> 用自联结而不用子查询自联结通常作为外部语句用来替代从相同表中检索数据时使用的子查询语句。虽然最终的结果是相同的，但有时候处理联结远比处理子查询快得多。应该试一下两种方法，以确定哪一种的性能更好。

```sql
# 自连结一定程度上可以代替子查询，比如以上两种等价
SELECT prod_id, prod_name
FROM products
WHERE vend_id = (SELECT vend_id
				 FROM products
				 WHERE prod_id = 'DTNTR')

SELECT p1.prod_id, p1.prod_name
FROM product AS p1, product AS p2
WHERE p1.vend_id = p2.vend_id
		AND p2.prod_id = 'DTNTR'
```

> 无论何时对表进行联结，应该至少有一个列出现在不止一个表中。
> 迄今为止我们的每个内联结都是自然联结

**外部联结**

```sql
SELECT customers.cust_id, orders.order_num
FROM customers INNER JOIN orders
		ON customers.cust_id = orders.cust_id

SELECT customers.cust_id, orders.order_num
FROM customers LEFT OUTER JOIN orders
		ON customers.cust_id = orders.cust_id
		
SELECT customers.cust_id, orders.order_num
FROM customers LEFT RIGHT JOIN orders
		ON customers.cust_id = orders.cust_id
```



##### 组合查询

> 多数sql查询都只包含从一个或者多个表返回单挑SELECT语句。MySQL也允许执行多个查询，将结果作为单个查询结果集返回，适用情况：
> 在单个查询中，从不同的表返回类似结构的数据
> 对单个表执行多个查询，按照单个查询返回数据

```sql
SELECT vend_id, prod_id, prod_price
FROM products
WHERE prod_price <= 5
UNION
SELECT vend_id, prod_id, prod_price
FROM products
WHERE vend_id IN (1001, 1002)
ORDER BY vend_id, prod_price
```
UNION规则：

- 必须由两条或两条以上的SELECT语句组成，语句之间用UNION分割
- 每个查询中必须包含相同的列，表达式，聚集函数
- 列数据类型必须兼容
- 自动去重，UNION ALL不会自动去重



##### 全文本搜索

MyISAM支持全文本搜索，InnoDB不支持。

全文本索引需要在创建表时指定FULLTEXT或者稍后指定。（会对结果自动排序）

```sql
SELECT note_text
FROM productnotes
WHERE Match(note_text) Against('rabbit')
# 可以和like子句等同
```



##### 插入数据

```sql
INSERT INTO Customers(cust_name, cust_contact, cust_email) VALUES(NULL, NULL, NULL), ('Amy', '12345', 'xxx@gmail.com')
# 最好提供插入时需要匹配的列名，同时和相应的值对应
# 一般不要使用没有明确给出列的列表的INSERT语句，使用列的列表能使SQL代码继续发挥作用，即使表结构发生变化

# 插入检索中的数据
INSERT INTO customers(cust_id, cust_contact, cust_email) 
		SELECT cust_id, cust_contact, cust_email FROM custnew
```



##### 更新和删除数据

```sql
# 更新表的内容
UPDATE customers
SET cust_email = 'xxx@fudd.com'
WHERE cust_id = 10002

# 删除表的内容
DELETE FROM customers
WHERE cust_id = 10002
```



##### 创建和操纵表

```sql
CREATE TABLE customers
(
    cust_id int NOT NULL AUTO_INCREMENT,
    cust_name char(50) NOT NULL,
    PRIMARY KEY (cust_id)
)ENGINE=InnoDB

# 更新表
ALTER TABLE vendors
ADD vend_phone CHAR(20)

ALTER TABLE orders
ADD CONSTRAINT fk_orders_customers FOREIGN KEY (cust_id)
REFERENCES customers (cust_id)

# 删除表
DROP TABLE customers

# 重命名表
RENAME TABLE customers2 TO customers
```

- 表中的每个行必须具有唯一的主键值，如果主键使用单个列或多个列，单个列的值或者多个列的组合值必须唯一。
- 引擎：
  - InnoDB 可靠的事务处理引擎，不支持全文本搜索
  - MEMORY 功能上等同于MyISAM，但由于数据存储在内存中，速度很快，适用于临时表
  - MyISAM 性能很高的引擎，支持全文本搜索



##### 使用视图

视图本身并不包含数据，仅仅用来查看存储在别处的数据的一种设施。如果用多个连结和过滤创建了复杂的视图或者嵌套了视图，性能会下降的很厉害。因此在部署使用了大量视图的应用前，应该先进行测试。

```sql
CREATE VIEW productcustomers AS 
SELECT cust_name, cust_contact, prod_id
FROM customers, orders, orderitems
WHERE customers.cust_id = orders.cust_id
	AND orderitems.order_num = orders.order_num
	
SELECT * 
FROM productcustomers
```

大多视图都是不可更新的。

如果视图定义中有以下操作，则不能进行更新：

- 分组，联结，子查询，并，聚集函数，DISTINCT，导出计算列



##### 创建存储过程

```sql
# 基础操作
CREATE PROCEDURE productpricing(OUT pa DECIMAL(8,2))
BEGIN
	SELECT AVG(prod_price) AS priceaverage
	FROM products
END

CALL productpricing(@priceaverage)
SELECT @priceaverage

DROP PROCEDURE productpricing

SHOW CREATE PROCEDURE ordertotal

# 创建游标
DECLARE ordernumbers CURSOR
FOR SELECT order_num FROM orders
```

