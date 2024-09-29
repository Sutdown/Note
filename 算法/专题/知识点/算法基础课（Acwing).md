# 算法设计与分析

## <u>前置：算法基础课（Acwing)</u>

[活动 - AcWing](https://www.acwing.com/activity/content/11/)



## 一.基础算法

### 排序

#### 快速排序---分治思想

1.确立分界点，从最左边，中间，最后面或者随机一个分界点都可以。

2.调整区间，使分界点左边的数都小于等于分界点，右边的数都大于等于边界点。

3.递归处理左右两段。

```c++
//从l到r之间，将l作为临界点，小于l的位于左边，大于l位于右边
void quick_sort(int q[], int l, int r) {
	//需要判断上次的i和j是否为递归出口，因此采用do while
	if (l >= r)return ;//递归出口

	int x = q[l], i = l - 1, j = r + 1;
	while (i < j) {
		//从左侧开始，找到比x大的数
		do i++;
		while (q[i] < x);
		//从右侧开始，找到比x小的数
		do j--;
		while (q[j] > x);
		//交换它们
		if (i < j)swap(q[i], q[j]);
	}
	//递归的思想
	quick_sort(q, l, j);
	quick_sort(q, j + 1, r);
}
```

Summary：调整区间+递归



#### 归并排序---分治思想

1.确立分界点为首末的中点。

2.以中点为界，递归两侧中点使其有序；先递归所有的数到出口，然后再从最后的出口往前回溯，在出口时每个数字都是一组，所以是有序的，再回溯时，就都是有序序列。

3.归并，指回溯时，将两边有序序列合二为一。（双指针算法）

```c++
void merge_sort(int q[], int l, int r) {
	if (l >= r)return; //递归出口
	//拆分过程
	int mid = (l + r) >> 1;
	merge_sort(q, l, mid), merge_sort(q, mid + 1, r);
	//合并两个有序序列
	//双指针算法，同时借用tmp数组保存合并后的序列
	int k = 0, i = l, j = mid + 1;
	while (i <= mid && j <= r)
		if (q[i] <= q[j])tmp[k++] = q[i++];
		else tmp[k++] = q[j++];
    //
	while (i <= mid)tmp[k++] = q[i++];
	while (j <= r)tmp[k++] = q[j++];
	//将tmp数组的值返赋给q数组
	for (int i = l, j = 0; i <= r; i++, j++)q[i] = tmp[j];
}
```

summary：先递归到最后一层，调整最后一层的左右顺序，难点在于合并左右两个有序序列，方法为双指针



注：归并排序和快速排序的时间复杂度都是nlog2n;

（降低原本的O（n^2）的复杂度，以归并为例，需要递归log2n层到达出口，每层时间复杂度为O（n)）；



### 二分

当题目具有二段性或者单调性时，可以采用二分进行枚举查找。

此方法一定有解，无解则是属于题目具体讨论。

#### 整数

版本一：边界点位于左半边，从而将[l,r]拆分为[l,mid]和[mid+1,r];

```c++
int bsearch_1(int l, int r) {
	while (l < r) {
		int mid = (l + r) >> 1;
		if (check(mid))r = mid;
		else l = mid + 1;
	}
	return l;
}
```

版本二：边界点位于右半边，从而将[l,r]拆分为[l,mid-1]和[mid,r];

```c++
int bsearch_2(int l, int r) {
	while (l < r) {
        //当l为r-1时，mid为r-1，会陷入死循环
        //因此，如果在条件成立时，l取mid，mid就要取l+r+1；r取mid，mid取l+r即可；
		int mid = (l + r + 1) >> 1;
        //l+1==r时，如果mid= l+r >>1就会陷入死循环
		if (check(mid))l = mid;
		else r = mid - 1;
	}
	return l;
}
```



#### 浮点数

相较于整数不用考虑边界的加一减一问题

保留四位小数，则保留精度到1e-6；保留六位小数，则保留精度到1e-8；

```c++
double l = 0, r = x;
while(r - l > 1e-8) {
    double mid = (l+r) / 2;
    if (mid * mid >= x) r = mid;
    else l = mid;
}
```



### 高精度

当用基本数据类型无法表示整数时，此时应该考虑到高精度运算。

大整数的存储：整数的高位存储在数组的低位，整数的低位存储数组的高位。因此由于逆序的缘故，进位时比较容易，但读入读出时需要注意其中的反转。

由于C++中没有处理大整数的类（python和java中有），因此我们只能用string处理大整数的加减乘除。

#### 高精度加法

模拟人工计算即可。

```c++
#include <iostream>
#include <vector>

using namespace std;

vector<int> add(vector<int>&A, vector<int>&B) {
	vector<int>C;

	int t = 0; //t表示进位
	for (int i = 0; i < (int)A.size() || i < (int)B.size(); i++) { //以较长的为界限
		if (i < (int)A.size())t += A[i];
		if (i < (int)B.size())t += B[i];
		C.push_back(t % 10); //个位
		t = t / 10; //十位
	}
    
    if (t) C.push_back(1);//最后一位不为0，直接赋给最高位
	
	return C;
}

int main() {
	string a, b; //字符串处理大整数
	cin >> a >> b;

	vector<int>A, B; //字符串存入数组
	for (int i = a.size()-1; i >= 0; i--) {
		A.push_back(a[i] - '0');
	}
	for (int i = b.size()-1; i >= 0; i--) {
		B.push_back(b[i] - '0');
	}

	auto C = add(A, B);
	for (int i = C.size()-1; i >= 0; i--) {
		cout << C[i];
	}
	cout << endl;
	return 0;
}
```



#### 高精度减法

1.借位和进位 2.正数和负数

```c++
#include <iostream>
#include <vector>

using namespace std;

bool cmp(vector<int>&A, vector<int>&B) {
	if (A.size() != B.size())return A.size() > B.size();

	for (int i = A.size() - 1; i >= 0; i--) {
		if (A[i] != B[i])
			return A[i] > B[i];
	}
	return true;
}

//前提:A>=B
vector<int> sub(vector<int>&A, vector<int>&B) {
	vector<int>C;

	int t = 0; //t表示进位
	for (int i = 0; i < (int)A.size(); i++) {
		t = A[i] - t; //借位
		if (i < (int)B.size()) t -= B[i];
		C.push_back((t + 10) % 10);
		if (t < 0)t = 1;//是否借位
		else t = 0;
	}
	
	//去掉前导零，即最高位的0
	while(C.size()>1&&C.back()==0)C.pop_back();

	return C;
}

int main() {
	string a, b; //字符串处理大整数
	cin >> a >> b;

	vector<int>A, B; //字符串存入数组
	for (int i = a.size() - 1; i >= 0; i--) {
		A.push_back(a[i] - '0');
	}
	for (int i = b.size() - 1; i >= 0; i--) {
		B.push_back(b[i] - '0');
	}

	if (cmp(A, B)) {
		auto C = sub(A, B);
		for (int i = C.size() - 1; i >= 0; i--) {
			cout << C[i];
		}
	} else {
		auto C = sub(B, A);
		cout << '-';
		for (int i = C.size() - 1; i >= 0; i--) {
			cout << C[i];
		}
	}
	cout << endl;

	return 0;
}
```



#### 高精度乘法

此为高精度整数和普通整数相乘，与高精度加法思想相似。

```c++
#include <iostream>
#include <vector>

using namespace std;

vector<int> mul(vector<int>&A, int &B) {
	vector<int>C;
	
	int t = 0; //t表示进位
	for (int i = 0; i < (int)A.size() || t; i++) { //以较长的为界限
		if (i < (int)A.size())t += A[i] *B;
		C.push_back(t % 10); //个位
		t = t / 10; //十位
	}
	
	return C;
}

int main() {
	string a;//字符串处理大整数
	int B;
	cin >> a >> B;
	
	vector<int>A; //字符串存入数组
	for (int i = a.size()-1; i >= 0; i--) {
		A.push_back(a[i] - '0');
	}
	
	auto C = mul(A,B );
	for (int i = C.size()-1; i >= 0; i--) {
		cout << C[i];
	}
	cout << endl;
	return 0;
}
```



#### 高精度除法

1.引入余数。

2.C最后要进行一次倒置，保证和其它四则运算兼容。

3.另外三种从最低位计算，除法是从最高位开始。

```c++
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

//A/B，C为商，r为余数
vector<int> div(vector<int>&A, int B, int &r) {
	vector<int>C;

	r = 0;
	for (int i = A.size() - 1; i >= 0; i--) {
		r = r * 10 + A[i];
		C.push_back(r / B);
		r %= B;
	}
	//格式一致
	reverse(C.begin(), C.end());
	//去掉前导零
	while (C.size() > 1 && C.back() == 0)C.pop_back();

	return C;
}

int main() {
	string a;//字符串处理大整数
	int B, r;
	cin >> a >> B;

	vector<int>A; //字符串存入数组
	for (int i = a.size() - 1; i >= 0; i--) {
		A.push_back(a[i] - '0');


	auto C = div(A, B, r);
	for (int i = C.size() - 1; i >= 0; i--) {
		cout << C[i];
	}
	cout << endl << r << endl;
	return 0;
}
```



### 前缀和

更多偏向一种思想，利用计算公式。

#### 一维

前缀和指数列中前n个数的和，而前缀和可以求出数列内任意一个区间数的和。

高中数列：
$$
a[j]=s[j]-s[j-1];

s[i]=a[1]+a[2]+a[3]+...+a[i];
$$

```c++
#include <iostream>
using namespace std;
const int N = 100010;
int n, m;
//a[N]为数组，s[N]为数组中前n个数的和
int a[N], s[N];
int main() {
	//n为数组大小，m为计算次数
	scanf("%d%d", &n, &m); //scanf比cin速度快一倍

	for (int i = 1; i <= n; i++)scanf("%d", &a[i]);
	for (int i = 1; i <= n; i++)s[i] = s[i - 1] + a[i]; //初始化前缀和

	while (m--) {
		int l, r;
		scanf("%d%d", &l, &r);
		printf("%d\n", s[r] - s[l - 1]);
	}
	return 0;
}
```



#### 二维

由一维前缀和扩展，二维平面上每一个点s【i】【j】代表其左上方的值的和。

求前缀和的公式：
$$
s[i][j]=s[i-1][j]+s[i][j-1]-s[i-1][j-1]+a[i][j];
$$
由此，任意一面积内的点数和：
$$
a[i][j]=s[i][j]-s[i-1][j]-s[i][j-1]+s[i-1][j-1];
$$
 注：S[i, j] = 第i行j列格子左上部分所有元素的和
以(x1, y1)为左上角，(x2, y2)为右下角的子矩阵的和为 
$$
S[x2, y2] - S[x1 - 1, y2] - S[x2, y1 - 1] + S[x1 - 1, y1 - 1]；
$$

```c++
#include<iostream>
using namespace std;
const int N = 1010;

int n, m, q;
int a[N][N], s[N][N];

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	cin >> n >> m >> q;

	//矩阵
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++)
			cin >> a[i][j];
	}

	//前缀和
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++)
			s[i][j] = s[i - 1][j] + s[i][j - 1] + a[i][j] - s[i - 1][j - 1];
	}

	int x1, y1, x2, y2;
	while (q--) {
		cin >> x1 >> y1 >> x2 >> y2;
		cout << s[x2][y2] - s[x1 - 1][y2] - s[x2][y1 - 1] + s[x1 - 1][y1 - 1] << endl;
	}

	return 0;
}
```

//b数组是a数组的前缀和数组



### 差分

#### 一维差分

a数组是原数组，b数组是a数组的差分，等价b数组的前缀和为a数组。
$$
b[i] = a[i] - a[i - 1]
$$
给区间[l, r]中的每个数加上c：
$$
b[l] += c, b[r + 1] -= c
$$

```c++
#include<iostream>
using namespace std;
const int N = 100010;
int n, m;
int a[N], b[N];

//核心代码
void insert(int l, int r, int c) {
	b[l] += c;
	b[r + 1] -= c;
}
int main() {
	scanf("%d%d", &n, &m); //n为数组长度，m为在某区间改变值的次数
	for (int i = 0; i <= n; i++)scanf("%d", &a[i]); //原数组
	//b数组记录差分
	for (int i = 0; i <= n; i++)insert(i, i, a[i]);

	while (m--) {
		int l, r, c;
		scanf("%d%d%d", &l, &r, &c);
		//在[l,r]区间内加c；
		insert(l, r, c);
	}
	//计算前缀和，将数组b还原为数组a
	for (int i = 1; i <= n; i++)b[i] += b[i - 1];
	//输出
	for (int i = 0; i <= n; i++)printf("%d", b[i]);

	return 0;
}
```

####  二维差分

给以(x1, y1)为左上角，(x2, y2)为右下角的子矩阵中的所有元素加上c：
$$
S[x1, y1] += c, S[x2 + 1, y1] -= c, S[x1, y2 + 1] -= c, S[x2 + 1, y2 + 1] += c
$$

```c++
#include<iostream>
using namespace std;
const int N = 10010;

//a数组可要可不要，把a数组当作某两个相同的点的差分也就是那个点位的值
int n, m, q;
int a[N][N], b[N][N];

void insert(int x1, int y1, int x2, int y2, int c) {
	b[x1][y1] += c;
	b[x2+1][y1] -= c;
	b[x1][y2+1] -= c;
	b[x2 + 1][y2 + 1] += c;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	
	cin >> n >> m >> q;
	
	//矩阵
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++)
			cin >> a[i][j];
	}
	
	//前缀和
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++)
			//i,j,i,j的变换使在b[i][j]的位置就是a[i][j];
			insert(i, j, i, j, a[i][j]);
	}
	
	int x1, y1, x2, y2, c;
	while (q--) {
		cin >> x1 >> y1 >> x2 >> y2 >> c;
		insert(x1, y1, x2, y2, c);
	}
	
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			b[i][j] += b[i - 1][j] + b[i][j - 1] - b[i - 1][j - 1];  //二维前缀和
		}
	}
	
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++)
			cout << b[i][j] << ' ';
		cout << endl;
	}
	return 0;
}
```

//a数组是b数组的前缀和数组，b数组是a数组的差分数组



### 双指针算法

常用问题：

1.两个指针指向同一数组，

遍历方向相同且不相交，称为滑动窗口，用来解决区间搜索问题。

遍历方向相反，则可以用来搜索有序数组（需要提前进行排序）。

2.快慢指针，用来判断链表环路问题。

双指针算法的核心思想是将朴素算法O（n*n）优化到O（n）；

```C++
//朴素算法
for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			//)O(n^2);
         
//双指针算法
for (i = 0, j = 0; i < n; i++) {
		while (j < i && check(i, j))j++;

		//每道题的具体逻辑
	}
```



### 位运算

#### n的二进制表示中的第k位是多少

1.进行移位，将第k位移到最后一位。

2.看个位，&1表示取出当前最后一位。



#### lowbit（x）返回x的最后一位1

```c++
#include <iostream>
using namespace std;

//lowbit可以截取一个二进制数的最后一个1开始后的所有数
int lowbit(int x) {
	return x & (-x);
}

int main() {
	int num;
	int res = 0;
	cin >> num;
	while (num) {
		num -= lowbit(num);
		res++;
	}
		cout << res << endl;

	return 0;
}

```



### 离散化

#### 背景：

当存在一个序列时，序列的容量很大，但是只存储了很少的数（即大小），此时就需要离散化对数据进行处理。       

 数的值域跨度范围很大，但是数的个数很少，通常会差几个数量级。

1）序列中可能存在重复元素。

2）如何求出x离散化后的值---二分。

#### 关于unique（）

~使用前需要排序；

~所有不重复的元素排在数组前面，数组末尾未占用的位置保留原来的值；

~返回值是不重复的元素个数（标准说法是去重之后的尾地址），即重复元素的第一位，再用earse进行删除。



```c++
vector<int> alls;//存储所有待离散化的值
	sort(alls.begin(), alls.end()); //由小到大排序
	alls.erase(unique(alls.begin(), alls.end()), alls.end()); //序列去重

	//二分求出x对应离散化后的值
	int find(int x) { //找出第一个大于等于x的位置
		int l = 0, r = alls.size() - 1;
		while (l < r) {
			int mid = (l + r) >> 1;

			if (alls[mid] >= x)r = mid;
			else l = mid + 1;
		}
	    return r + 1;
	}
```



#### 区间和（acwing802）

假定有一个无限长的数轴，数轴上每个坐标上的数都是0。

现在，我们首先进行n次操作，每次操作将某一位置 x上的数加c。

接下来，进行 m 次询问，每个询问包含两个整数 l 和 r，你需要求出在区间 [l,r] 之间的所有数的和。

输入格式:

第一行包含两个整数 n和 m。

接下来 n 行，每行包含两个整数 x和 c。

再接下来m行，每行包含两个整数l和 r。

输出格式:

共 m行，每行输出一个询问中所求的区间内数字和。

```c++
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int N = 10010;
typedef pair<int, int> PII;

int n, m; //n次操作，m次询问
int a[N], s[N]; //前缀和

vector<int> alls;//存放离散之后的所有数的顺序，完成排序去重
vector<PII> add, query; //添加，询问

//二分查找
int find(int x) {
	int l = 0, r = alls.size() - 1;
	while (l < r) {
		int mid = (l + r) >> 1;
		if (alls[mid] >= x)r = mid;
		else l = mid + 1;
	}
	return r + 1;
}

int main() {
	cin >> n >> m;

	//添加
	while (n--) {
		int x, c;
		cin >> x >> c;
		add.push_back({x, c});
		alls.push_back(x);
	}

	//询问
	while (m--) {
		int l, r;
		cin >> l >> r;
		query.push_back({l, r});
		alls.push_back(l);
		alls.push_back(r);
	}

	//去重
	sort(alls.begin(), alls.end());
	alls.erase(unique(alls.begin(), alls.end()), alls.end());

	//处理添加
	for (auto item : add) {
		a[find(item.first)] += item.second;
	}

	//前缀和
	//alls重新建立一个数组，alls存储数组下标，从小到大
	//a[N]存储连续的下标对应的值
	for (int i = 1; i <= (int)alls.size(); i++)
		s[i] = s[i - 1] + a[i];

	//处理询问结果
	for (auto t : query) {
		int l = find(t.first), r = find(t.second);
		cout << s[r] - s[l - 1] << endl;
	}
	return 0;
}
```



### 区间合并

如果两个区间有交集，则将两个区间合并为一个。

区间和区间之间的关系分为三类：

1.彼此互不相交；

2.后一个区间被前一个区间包含；

3.后一个区间与前一个区间有相交的部分。

```c++
void merge(vector<PII>& segs) {
    //此处的PII指的是pair，pair类似于一种元素用来存储一组key和value值
    //而map是一种关联式容器，可以保存多个pair
	vector<PII> res;

	sort(segs.begin(), segs.end());

	int st = -2e9, ed = -2e9; //维护一个区间
	for (auto seg : segs) { //C++11的新特性，vector遍历的简写
		//处理情况一
		//新区间不在维护区间内，说明是一个全新的区间
		if (ed < seg.first) {
			if (st != -2e9)res.push_back({st, ed});
			st = seg.first, ed = seg.second;
            //处理后不要立刻进入res，而是要在下次时进行比较
		} else//处理情况左侧在区间内，合并右边即可
			ed = max(ed, seg.second);
	}
	if (st != -2e9)res.push_back({st, ed});

	segs = res;
}
```



## 二.数据结构

### cin,cout加速代码句

```c++
cin.tie(0);
ios::sync_with_stdio(false);
```



### 链表与邻接表：树与图的存储（用数组表示）

#### 数组模拟单链表——邻接表

单链表的作用是邻接表，邻接表用来存储树和图

```c++
//数组模拟单链表
const int N = 100010;
//head表示头结点的下标
//e【i】表示结点i的值
//ne【i】表示结点i的下一个结点的值
//idx存储当前用到的指针的个数

int head, e[N], ne[N], idx;

//初始化单链表
void init() {
	head = -1;
	idx = 0;
}

//建立单链表,尾插法
void add_to_head(int x) {
	e[idx] = x, ne[idx] = head, head = idx++;
}

//从第k个数后面插入x
void add(int k, int x) {
	e[idx] = x, ne[idx] = ne[k], ne[k] = idx++;
}

//下标是k的点的后一个点移除
void remove(int k) {
	ne[k] = ne[ne[k]];
}
```



#### 数组模拟双链表

双链表一般用来优化题目

```c++
//数组模拟双链表
const int N = 100010;
int e[N], l[N], r[N], idx;

//将0定义为头结点，1为尾结点
void init() {
	l[1] = 0, r[0] = 1, idx = 2;
}

//在第k个点之后插入x
void add(int k, int x) {
	e[idx] = x;
	l[idx] = k;
	r[idx] = r[k];
	l[r[k]] = idx;
	r[k] = idx++;
}

//移除第k个位置的点
void remove(int k) {
	l[r[k]] = l[k];
	r[l[k]] = r[k];
}

//在最左边插入一个结点
void insertL(int x) {
	e[idx] = x;
	l[idx] = 0;
	r[idx] = r[0];
	r[0] = idx;
	l[r[idx]] = idx;
	idx++;
}

//在最右边插入一个结点
void insertR(int x) {
	e[idx] = x;
	r[idx] = 1;
	l[idx] = l[1];
	r[l[idx]] = idx;
	l[1] = idx;
	idx++;
}

```



### 栈——先进后出（First In last Out)

#### 数组模拟栈

```c++
//数组模拟栈
const int N = 100010;
int stk[N], tt;
//插入元素
stk[++tt] = x;
//弹出元素
tt--;
//判断栈
stk[tt];
//判断栈是否为空
if (tt > 0) {

}
```



#### 单调栈

从当前位置的左边找到一个离它最近并且比它小的数

维护一个栈，实现上述问题

引申：每当倒序寻找一个数时，可以考虑用栈简化问题，降低时间复杂度

```c++
//时间复杂度为o（n），栈里的每个元素只进栈出栈一次，2*n次操作
	int stk[N], tt;

	while (n--) {
		int x;
		cin >> x;
        //边进栈边判断，栈中始终时单调的
		while (tt && stk[N] >= x)tt--;
		···
		stk[++tt] = x;
	}
```



### 队列——先进先出（First In First Out)

#### 数组模拟队列

```c++
//hh队头，tt队尾
	int q[N], hh, tt = -1;
	//入队
	q[++tt] = x;
	//出队
	hh++;
	//队头的值
	q[hh];
	//队列是否为空
	if (hh <= tt) {

	}
```



#### 单调队列

维护一个单调队列

```c++
//维护一个单调递增的队列
//a数组存储数值，q数组存储下标，便于队头元素出队
//滑动窗口：队头出队的情况就是已经超过了滑动窗口的阈值
	int hh = 0, tt = -1;
	for (int i = 0; i < n; i++) {
		//队列不为空并且滑出窗口时，队头出队
		if (hh <= tt && i - k + 1 > q[hh])hh++;
		//队列不为空且进队列元素比队头小时，队头出队
		while (hh <= tt && a[q[tt]] >= a[i])tt--;
		//插入
		q[++tt] = i;
		···
	}
```



### KMP

在子串中，p[1,j]=p[i-j+1,j];

即从当前位置往前数j个，和该串开头的j个相等（相当于回退）。

作用：KMP中next数组的出现，使子串和主串比较时，不用从头开始比较，在字串向后移动的过程中，（假设成功匹配）必然会出现和原有位置重合的部分，而next数组就是为了记录这一重合部分的长度，避免了O（n^2)的时间复杂度。

```c++
	//s[]是长文本，p[]是模式串，m是s的长度，n是p的长度
	//求next数组
	for (int i = 2, j = 0; i <= n; i++) {
		//next[1]=0,所以下标从2开始
        //while循环类似递归的思路，寻找第i个下标前面的ne[i]的值，同时利用ne[i-1]等其它数据
		while (j && p[i] != p[j + 1])j = ne[j];
        //在ne[j]的基础上修正
		if (p[i] == p[j + 1])j++;
        //得到相应位置上ne[i]的值
		ne[i] = j;
	}

	//模式匹配
	for (int i = 0, j = 0; i <= m; i++) {
        //当此次匹配不成功时，对于字串进行next数组的回溯
        //next数组就是前几个和后几个时相同的，无需重复比较
		while (j && s[i] != p[j + 1])j = ne[j];
		if (s[i] == p[j + 1])j++;
		if (j == n) {
			cout << i - n;
			j = ne[j];
		}
	}
```



### Trie

维护一个根节点为0的树，用来处理字符串集合（精确到每一个字符），提供字符串的插入和查询操作。

```c++
const int N = 100010;
//son存储树，26指小写字母的个数，N则类似于链表数组的用法，上一个结点son的值对应下一个结点中N的值
//idx为结点编号，也就是总结点数
//cnt[],存储某个结点结尾的单词的数量
int son[N][26], cnt[N], idx;

void insert(char *str) {
	int p = 0;

	for (int i = 0; str[i]; i++) {
		//映射字母为0-25整数
		int u = str[i] - 'a';
		//判断是否有下一结点，否则创建
		if (!son[p][u]) son[p][u] = ++idx;
		//下一个结点
		p = son[p][u];
	}
	//某个单词出现次数
	//由于p的特定性，所以不会与其它重
	cnt[p]++;
}

int query(char *str) {
	int p = 0;

	for (int i = 0; str[i]; i++) {
		int u = str[i] - 'a';
		if (!son[p][u])return 0;
		p = son[p][u];
	}

	return cnt[p];
}
```



### 并查集

### 堆



### Hash表

[840. 模拟散列表 - AcWing题库](https://www.acwing.com/problem/content/842/)

[841. 字符串哈希 - AcWing题库](https://www.acwing.com/problem/content/843/)

由于I是插入一个数，Q是询问一个数是否存在。要是用数组顺序存放，再每次寻找时遍历，时间复杂度为O(n)，可以。

要是我将每插入的数作为数组的下标，将插入记为1，未插入记为0，可以实现时间复杂度为O(1)，但是对于空间的利用率极低。

有没有能够将两种思路的有点合并的方法？

[一文快速入门哈希表_哈希表入门-CSDN博客](https://blog.csdn.net/raelum/article/details/128793474)(优质文章和优质创作者)





## 三.搜索与图论

### DFS和BFS

#### DFS（深度优先搜索）

本质上为维护一个隐藏的stack，不具有最短性。

空间：O（h）（较为节省空间）

DFS的两个重要概念：

回溯：也就是需要return的地方;

剪枝：提前判断一些不满足条件的地方，并将其回溯。

注意：
回溯时记得恢复现场；

必然有一个标记访问的过程。



例一：全排列问题

```c++
#include <iostream>
using namespace std;
//数字的全排列

const int N = 10;
int n, path[N]; //数字长度和路径
bool st[N];//状态数组

void dfs(int u) {
	if (u == n) {
		for (int i = 0; i < n; i++)cout << path[i] << ' ';
		puts(" ");
	}

	for (int i = 1; i <= n; i++) {
		if (!st[i]) {
			path[u] = i;
			st[i] = true;
			dfs(u + 1); //深度搜索
			st[i] = false; //恢复
		}
	}
}

int main() {
	cin >> n;
	dfs(0);
	return 0;
}
```



例二：n-皇后问题

逐行搜索：

根据每行的搜索，形成一个类似于树从上到下的框架，思路类似于全排列,时间复杂度O（n*n!)。

```c++
#include <iostream>
using namespace std;

const int N = 20;

int n;
char g[N][N];
bool col[N], dg[N], udg[N];//列，对角线，反对角线

void dfs(int u) {
	if (u == n) {
		for (int i = 0; i < n; i++)puts(g[i]);
		puts("");//换行
		return;
	}

	for (int i = 0; i < n; i++) {
		if (!col[i] && !dg[u + i] && !udg[n - u + i]) {//列，对角线，反对角线
			g[u][i] = 'Q';
			col[i] = dg[u + i] = udg[n - u + i] = true;
			dfs(u + 1);
			col[i] = dg[u + i] = udg[n - u + i] = false;
			g[u][i] = '.';
		}
	}
}
int main() {
	cin >> n;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			g[i][j] = '.';
	}

	dfs(0);
	return 0;
}
```

逐格搜索：

每个格子只有放皇后和不放皇后两种选择，时间复杂度为2^n*n;

```c++
const int N = 10;

int n;
bool row[N], col[N], dg[N * 2], udg[N * 2];
char g[N][N];

void dfs(int x, int y, int s)
{
    if (s > n) return;
    if (y == n) y = 0, x ++ ;

    if (x == n)
    {
        if (s == n)
        {
            for (int i = 0; i < n; i ++ ) puts(g[i]);
            puts("");
        }
        return;
    }

    g[x][y] = '.';
    dfs(x, y + 1, s);

    if (!row[x] && !col[y] && !dg[x + y] && !udg[x - y + n])
    {
        row[x] = col[y] = dg[x + y] = udg[x - y + n] = true;
        g[x][y] = 'Q';
        dfs(x, y + 1, s + 1);//递归
        g[x][y] = '.';
        row[x] = col[y] = dg[x + y] = udg[x - y + n] = false;//恢复
    }
}
```



#### BFS（广度优先搜索）

本质上是维护一个队列，queue，能够计算最短路。

空间复杂度为O（2^h）。BFS能够记录路径。

例1：迷宫问题

```c++
#include <iostream>
#include <queue>
#include <cstring>
using namespace std;

typedef pair<int, int> PII;

//BFS解决最短路问题
//配合队列使用
const int N = 110;

int n, m;
int maze[N][N], d[N][N]; //分别存放迷宫和最短距离

int BFS() {
	queue<PII> q;

	memset(d, -1, sizeof(d)); //先初始化-1
	d[0][0] = 0; //从左上开始
	q.push({0, 0});

	//maze中标记1不能走，0能走
	while (q.size()) {
		auto t = q.front();
		q.pop();

		int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};

		for (int i = 0; i < 4; i++) {
			int x = t.first + dx[i], y = t.second + dy[i];

			//BFS搜索，宽度优先搜索，四个方向
			if (x >= 0 && x < n && y >= 0 && y < m && maze[x][y] == 0 && d[x][y] == -1) {
				d[x][y] = d[t.first][t.second] + 1;
				q.push({x, y});
			}
		}

	}

	return d[n - 1][m - 1];
}

int main() {
	//接受迷宫
	cin >> n >> m;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			cin >> maze[i][j];

	cout << BFS();
	return 0;
}
```



### 树和图的遍历

树可以是一种特殊的图（树只有一个根节点），无向图也是一种特殊的有向图。

图有两种存储方式：

邻接矩阵：二维数组记录两个结点之间的权值，没有权值时采用0和1，有权重就记录当死的权重值。

邻接表：数组模拟邻接表如下

图中的每个结点都有一个单链表，用于存储该点可以到达的点

```c++
//h数组表示存储了多个单链表
int h[N], e[M], ne[M], idx;

memset(h, -1, sizeof(h));

void add(int a, int b) {
	//多链表+尾插法
	e[idx] = b, ne[idx] = h[a], h[a] = idx++;
}
```



#### 深度优先遍历

模板DFS树与图的遍历

```c++
bool st[N];//标记访问状态
void dfs(int u) {
	st[u] = true;

	for (int i = h[u]; i != -1; i = ne[i]) {
		int j = e[i];
		if (!st[j])dfs(j);
	}
}
```

例题：树的重心[846. 树的重心 - AcWing题库](https://www.acwing.com/problem/content/description/848/)

重心定义：重心是指树中的一个结点，如果将这个点删除后，剩余各个连通块中点数的最大值，在所有最大值中的最小的那个节点被称为树的重心。

```c++
//树的重心
#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;

const int N = 100010;
const int M = 100010 * 2; //以有向图的形式存储无向图

int n, ans = N;

//h数组表示存储了多个单链表
int h[M], e[M], ne[M], idx;

void add(int a, int b) {
	//a为根，根后插入b
	e[idx] = b, ne[idx] = h[a], h[a] = idx++;
}

bool st[N];//标记访问状态
int dfs(int u) {
	int res=0;//删去某个结点后最大连通子图的结点数
	st[u] = true;
	int sum=1;//以u为根的结点数
	
	//循环之内已经遍历出来了子树结点的最大
	for (int i = h[u]; i != -1; i = ne[i]) {
		int j = e[i];
		if (!st[j]) {
			int s = dfs(j); //未被访问时以该节点为根节点的子树的结点总数
			res = max(res, s); //找到最大联通子图的结点数目
			sum += s; //总结点树，以便求最后一个联通子图的总数
		}
	}
	
	//二次递归时的结点是访问不到上面已经访问的结点，所以需要n-sum
	res = max(res, n - sum); //结点为u时的最大联通子图
	ans = min(res, ans); //重心的最小的最大值
	return sum;
}

int main() {
	memset(h, -1, sizeof(h));
	cin >> n;

	for (int i = 1; i < n; i++) {
		int a, b;
		cin >> a >> b;
		add(a, b), add(b, a);
	}

	dfs(1);//可以从任意一个结点开始，无向图

	cout << ans << endl;
	return 0;
}
```



#### 广度优先遍历

权重为1时，可以用BFS求最短距离。

例题：[847. 图中点的层次 - AcWing题库](https://www.acwing.com/problem/content/849/)

```c++
#include <iostream>
#include <cstring>
using namespace std;

const int N = 1e5 + 10;

int h[N], e[N], idx, ne[N];
int d[N];//路径距离

int n, m; //n个结点m条边

int q[N];//存储层次遍历序列，队列

void add(int a, int b) {
	e[idx] = b, ne[idx] = h[a], h[a] = idx++;
}

int bfs() {
	int hh = 0, tt = 0;
	q[0] = 1; //队列中的第一个元素是编号为1的结点

	memset(d, -1, sizeof(d));

	d[1] = 0; //每个结点到起点的距离

	while (hh <= tt) {
		int t = q[hh++]; //取出队列头结点；

		//遍历结点的每一个邻边
		for (int i = h[t]; i != -1; i = ne[i]) {
			int j = e[i];
			if (d[j] == -1) {
				d[j] = d[t] + 1; //存储距离
				q[++tt] = j; //入队
			}
		}
	}

	return d[n];
}
int main() {
	cin >> n >> m;

	memset(h, -1, sizeof(h));

	for (int i = 0; i < m; i++) {
		int a, b;
		cin >> a >> b;
		add(a, b);
	}

	cout << bfs() << endl;
	return 0;
}
```



### 拓扑排序

拓扑序列：图中的每条边（x,y)都出现在y之前，则称A是该图的一个拓扑序列。

时间复杂度：O（n+e），n点数e边数，搜索入度为0的点所需的时间是O（n），每个顶点入读-1的运算执行力e次。

操作：找到入度为零的点，拉出来，并且删掉它的所有出度。因此拓扑序列不唯一。

只有有向图才存在拓扑序列，思路为BFS。

例题：[848. 有向图的拓扑序列 - AcWing题库](https://www.acwing.com/problem/content/850/)

```c++
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

const int N = 100010;

int e[N], ne[N], h[N], idx; //邻接表存储

int q[N], hh = 0, tt = -1; //队列

int n, m; //图的点数和边数
int d[N];//各个点的入度

void add(int a, int b) {
	e[idx] = b, ne[idx] = h[a], h[a] = idx++;
}

void topSort() {
	for (int i = 1; i <= n; i++) {
		if (d[i] == 0)
			q[++tt] = i; //入读为0时入队
	}

	while (hh <= tt) {
		int a = q[hh++];
		for (int i = h[a]; i != -1; i = ne[i]) {
			int b = e[i];
			d[b]--;//删去入度为0的点，与之相连的点入读-1
			if (d[b] == 0)
				q[++tt] = b;
		}
	}

	if (tt == n - 1) { //所有数均入过队列，可以拓扑排序
		for (int i = 0; i < n; i++)
			cout << q[i] << ' ';
	} else
		cout << -1;

	cout << endl;
}

int main() {
	cin >> n >> m;

	memset(h, -1, sizeof(h));

	for (int i = 0; i < m; i++) {
		int a, b;
		cin >> a >> b;
		d[b]++;//入度的数组
		add(a, b);
	}

	topSort();//拓扑排序
	return 0;
}
```



### 最短路

#### Dijkstra



#### bellman-ford



#### spfa



#### Floyd

思想：动态规划。

时间复杂度：O(n^3)。

状态转移方法：
$$
d[k][i][j]=d[k-1][i][k]+d[k-1][k][j]
$$

```c++
void floyd() {
	for (int k = 1; k <= n; k ++ )
		for (int i = 1; i <= n; i ++ )
			for (int j = 1; j <= n; j ++ )
				d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
}
```



### 最小生成树

#### Prim

首先选取权重最小的边加入集合，接着寻找与其相关联的边，选取其中最小的加入集合，重复这一过程，从已知的点出发寻找最短边，同时避免成环。最后形成最小生成树。

朴素版Prim

联系：Dijkstra是更新到起始点的距离，Prim是更新到集合的距离。

```c++
const int N = 510, Inf = 0x3f3f3f;
int g[N][N];
int dist[N];
bool st[N];
int n, m;

memset(g, 0x3f, sizeof(g));

int Prim() {
	memset(dist, 0x3f, sizeof(dist)); //初始化为负无穷
	int res = 0; //记录权重

	//从第一个点开始循环迭代
	for (int i = 0; i < n; i++) {
		int t = -1;

		//选取与集合最近的点
		for (int j = 1; j <= n; j++)
			if (!st[j] && (t == -1 || dist[j] > dist[t]))
				t = j;

		//跳过第一次循环，从i=1开始
		//遍历为负无穷，说明不完全连通
		if (i && dist[t] == Inf)return Inf;
		if (i)res += dist[t];

		for (int j = 0; j <= n; j++)
			dist[j] = min(dist[j], g[t][j]);
		st[t] = true;
	}
	return res;
}
```



#### Kruskal



### 二分图：染色法，匈牙利算法

#### 染色法判定二分图

#### 匈牙利算法



## 四.数论

### 质数

### 约数

### 欧拉函数

### 快速幂

### 扩展欧几里得算法

### 中国剩余定理

### 高斯消元

### 组合计数

### 容斥原理

### 简单博弈论



## 六.贪心



## 七.事件复杂度分析`





## 正文：算法设计分析

见笔记本，不更新了。