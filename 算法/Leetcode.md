# Leetcode



[TOC]

### [852. 山脉数组的峰顶索引 - 力扣（LeetCode）](https://leetcode.cn/problems/peak-index-in-a-mountain-array/description/)（1182）

> 符合下列属性的数组 `arr` 称为 **山脉数组** ：
>
> - `arr.length >= 3`
> - 存在i(0 < i < arr.length - 1)使得：
>   - `arr[0] < arr[1] < ... arr[i-1] < arr[i] `
>   - `arr[i] > arr[i+1] > ... > arr[arr.length - 1]`
>
> 给你由整数组成的山脉数组 `arr` ，返回满足 `arr[0] < arr[1] < ... arr[i - 1] < arr[i] > arr[i + 1] > ... > arr[arr.length - 1]` 的下标 `i` 。
>
> 你必须设计并实现时间复杂度为 `O(log(n))` 的解决方案。

由于要求山脉的特点，最容易想到的是**双指针**，从两边向内，直到两指针重合便说明是一个山脉数组。

不过这个的时间复杂度，怕是不满足`O(log(n))`吧，但是过了。。

```cpp
int peakIndexInMountainArray(vector<int>& arr) {
	int i = 0, j = arr.size() - 1;
	while (i < j) {
		if (arr[i] < arr[i + 1])i++;
		if (arr[j - 1] > arr[j])j--;
	}
	return i;
}
```

如果要实现`O(log(n))`的时间复杂度的话，正确的思路应该是**二分查找**。

```cpp
int peakIndexInMountainArray(vector<int>& arr) {
	int i = 0, j = arr.size() - 1, ans = 0;
	while (i < j) {
		int mid = (i + j) / 2;
		if (arr[mid] < arr[mid + 1])i = mid + 1, ans = mid + 1;
		else j = mid;
	}
	return ans;
}
```

——leetcode竞赛参考分数(1182)（这个属于简单的，可以适当做的高一点）



### [921. 使括号有效的最少添加 - 力扣（LeetCode）](https://leetcode.cn/problems/minimum-add-to-make-parentheses-valid/)（1242）

> 只有满足下面几点之一，括号字符串才是有效的：
>
> - 它是一个空字符串，或者
> - 它可以被写成 `AB` （`A` 与 `B` 连接）, 其中 `A` 和 `B` 都是有效字符串，或者
> - 它可以被写作 `(A)`，其中 `A` 是有效字符串。
>
> 给定一个括号字符串 `s` ，在每一次操作中，你都可以在字符串的任何位置插入一个括号
>
> - 例如，如果 `s = "()))"` ，你可以插入一个开始括号为 `"(()))"` 或结束括号为 `"())))"` 。
>
> 返回 *为使结果字符串 `s` 有效而必须添加的最少括号数*。

括号匹配问题？貌似用栈就能做。

就只要发现有匹配的出栈就行，最后得到栈中的个数就是最终答案。

```cpp
int minAddToMakeValid(string s) {
	if (s.size() == 0)return 0;
	stack<char> st;

	for (int i = 0; i < (int)s.size(); i++) {
		if (s[i] == '(')st.push(s[i]);
		else {
			if (!st.empty() && st.top() == '(')st.pop();
			else st.push(s[i]);
		}
	}
	return st.size();
}
```

当然这个从头到尾一个个看也可以当成贪心的思想，记录左括号的个数也可以，可以不用栈实现。

——leetcode竞赛参考分数(1242)（好耶，秒了）



### [888. 公平的糖果交换 - 力扣（LeetCode）](https://leetcode.cn/problems/fair-candy-swap/description/)（1334）

> 爱丽丝和鲍勃拥有不同总数量的糖果。给你两个数组 aliceSizes 和 bobSizes ，aliceSizes[i] 是爱丽丝拥有的第 i 盒糖果中的糖果数量，bobSizes[j] 是鲍勃拥有的第 j 盒糖果中的糖果数量。
>
> 两人想要互相交换一盒糖果，这样在交换之后，他们就可以拥有相同总数量的糖果。一个人拥有的糖果总数量是他们每盒糖果数量的总和。
>
> 返回一个整数数组 answer，其中 answer[0] 是爱丽丝必须交换的糖果盒中的糖果的数目，answer[1] 是鲍勃必须交换的糖果盒中的糖果的数目。如果存在多个答案，你可以返回其中 任何一个 。题目测试用例保证存在与输入对应的答案。

思路倒有，不知道会不会超时。找到两人相差糖果的差值，然后找对应的满足答案的数据就好。good，过了。

```cpp
vector<int> fairCandySwap(vector<int>& aliceSizes, vector<int>& bobSizes) {
	int a = accumulate(aliceSizes.begin(), aliceSizes.end(), 0);
	int b = accumulate(bobSizes.begin(), bobSizes.end(), 0);
	int c = (a - b) / 2;//找到它们的差值

	sort(aliceSizes.begin(), aliceSizes.end());
	sort(bobSizes.begin(), bobSizes.end());//排序

	int i = 0, j = 0;//分离双指针
	while (aliceSizes[i] != bobSizes[j] + c) {
		if (aliceSizes[i] > bobSizes[j] + c)j++;
		else i++;
	}

	return {aliceSizes[i], bobSizes[j]};
}
```

看了眼答案，后面那个比较的思路，可以用**双指针**，**哈希表**，**二分查找**。

哈希表的话，就把第一个当做哈希表，再把第二个加上c找有没有和第一个数组相同的就好。二分变化多点，这里就不讨论了。

——leetcode竞赛参考分数(1334)（原来这就一千三的水平，明天再往高点的分数试试，今天到此为止了）



### [890. 查找和替换模式 - 力扣（LeetCode）](https://leetcode.cn/problems/find-and-replace-pattern/description/)（1414）

> 你有一个单词列表 `words` 和一个模式 `pattern`，你想知道 `words` 中的哪些单词与模式匹配。
>
> 如果存在字母的排列 `p` ，使得将模式中的每个字母 `x` 替换为 `p(x)` 之后，我们就得到了所需的单词，那么单词与模式是匹配的。
>
> *（回想一下，字母的排列是从字母到字母的双射：每个字母映射到另一个字母，没有两个字母映射到同一个字母。）*
>
> 返回 `words` 中与给定模式匹配的单词列表。
>
> 你可以按任何顺序返回答案。

本来想一通暴力做的，然后觉得写的可能太多了，如果是判断words和pattern的匹配程度的话，可以采用**哈希表**。

注意熟悉哈希表的用法。

```cpp
bool match(string &word, string &pattern) {
	unordered_map<char, char>mp; //哈希表创造映射
	for (int i = 0; i < word.length(); i++) {
		char x = word[i], y = pattern[i];
		//如果在哈希表中没有出现该映射，就给加上。
		//如果出现了但是映射不对，返回错误。
		if (!mp.count(x))mp[x] = y;
		else if (mp[x] != y)return false;
	}
	return true;
}

vector<string> findAndReplacePattern(vector<string>& words, string pattern) {
	vector<string> ans;
	for (auto &word : words) {
		if (match(word, pattern) && match(pattern, word))ans.emplace_back(word);
	}
	return ans;
}
```

——leetcode竞赛参考分数(1414)



### [1985. 找出数组中的第 K 大整数 - 力扣（LeetCode）](https://leetcode.cn/problems/find-the-kth-largest-integer-in-the-array/description/)（1414）

> 给你一个字符串数组 `nums` 和一个整数 `k` 。`nums` 中的每个字符串都表示一个不含前导零的整数。
>
> 返回 `nums` 中表示第 `k` 大整数的字符串。

```c++
class Solution {
public:
    string kthLargestNumber(vector<string>& nums, int k) {
        int n=nums.size();
        k=n-k;
        vector<long> numsInt(n);
        for(int i=0;i<n;i++)numsInt[i]=atoi(nums[i].c_str());
        sort(numsInt.begin(),numsInt.end());
        return to_string(numsInt[k]);
    }
};
```

首先，注意字符串转整数和整数转字符串的函数atoi(s.c_str())，to_string(i)

再者，这样做最大的问题在于字符串里的数字是可以超过int甚至是long的最大精度的，所以要想百分百过，sort绝对是不能用的。

这个时候应该想到的是**快速排序**或者**堆排序**.

不过更换比较大小的方式也可以，用**模拟**。

```c++
class Solution {
public:
    string kthLargestNumber(vector<string>& nums, int k) {
        sort(nums.begin(),nums.end(),
        [](string s1,string s2)->bool{
            if(s1.size()!=s2.size())return s1.size()>s2.size();
            else return s1>s2;
        });
        return nums[k-1];
    }
};
```



### [945. 使数组唯一的最小增量 - 力扣（LeetCode）](https://leetcode.cn/problems/minimum-increment-to-make-array-unique/)（1448）

> 给你一个整数数组 `nums` 。每次 move 操作将会选择任意一个满足 `0 <= i < nums.length` 的下标 `i`，并将 `nums[i]` 递增 `1`。
>
> 返回使 `nums` 中的每个值都变成唯一的所需要的最少操作次数。

秒了。

```c++
int minIncrementForUnique(vector<int>& nums) {
        sort(nums.begin(),nums.end());
        int n=nums.size();
        int res=0;
        for(int i=1;i<n;i++){
            while(nums[i]<=nums[i-1]){
                nums[i]++;
                res++;
            }
        }
        return res;
    }
```



### [2645. 构造有效字符串的最少插入数 - 力扣（LeetCode）](https://leetcode.cn/problems/minimum-additions-to-make-valid-string/description/?envType=daily-question&envId=2024-03-06)（1478）

> 给你一个字符串 `word` ，你可以向其中任何位置插入 "a"、"b" 或 "c" 任意次，返回使 `word` **有效** 需要插入的最少字母数。
>
> 如果字符串可以由 "abc" 串联多次得到，则认为该字符串 **有效** 。

```c++
class Solution {
public:
    int addMinimum(string word) {
        int cnt=1;
        int n=word.size();
        for(int i=1;i<word.size();i++)
            if(word[i]<=word[i-1])cnt++;
        return cnt*3-n;
    }
};
```



### [869. 重新排序得到 2 的幂 - 力扣（LeetCode）](https://leetcode.cn/problems/reordered-power-of-2/)（1505）

——leetcode竞赛参考分数(1505)



### [877. 石子游戏 - 力扣（LeetCode）](https://leetcode.cn/problems/stone-game/)（1590）

——leetcode竞赛参考分数(1590)



### [916. 单词子集 - 力扣（LeetCode）](https://leetcode.cn/problems/word-subsets/)（1624）

——leetcode竞赛参考分数(1624)



### [923. 三数之和的多种可能 - 力扣（LeetCode）](https://leetcode.cn/problems/3sum-with-multiplicity/)（1711）

——leetcode竞赛参考分数(1711)



当前区间：

目标：1800