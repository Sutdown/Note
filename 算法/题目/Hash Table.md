# Hash Table



前两道题为简单题，先上手，然后再做几道中等题，大致理解哈希表的思想即可。



### [217. 存在重复元素 - 力扣（LeetCode）](https://leetcode.cn/problems/contains-duplicate/description/)

> 给你一个整数数组 `nums` 。如果任一值在数组中出现 **至少两次** ，返回 `true` ；如果数组中每个元素互不相同，返回 `false` 。

1.简单思路，排序，每个数的相邻两个不同，则一定互不相同。

2.利用unordered_set无序容器s，不含重复元素，不映射，不自动排序。

```cpp
bool containsDuplicate(vector<int>& nums) {
	unordered_set<int> s;
	for (auto x : nums) {
		if (s.find(x) != s.end())return true;
		s.insert(x);
	}
	return false;
}
```



### [219. 存在重复元素 II - 力扣（LeetCode）](https://leetcode.cn/problems/contains-duplicate-ii/description/)

> 给你一个整数数组 `nums` 和一个整数 `k` ，判断数组中是否存在两个 **不同的索引** `i` 和 `j` ，满足 `nums[i] == nums[j]` 且 `abs(i - j) <= k` 。如果存在，返回 `true` ；否则，返回 `false` 。

