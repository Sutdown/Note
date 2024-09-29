# 全排列问题（backtracking）



## [46. 全排列 - 力扣（LeetCode）](https://leetcode.cn/problems/permutations/)

> 给定一个不含重复数字的数组 `nums` ，返回其 *所有可能的全排列* 。你可以 **按任意顺序** 返回答案。

思路：可以采用一个树从上而下列出所有的情况，但是要如何实现代码，每条路径上每个数只能出现一次。

**限界函数：**该路径不与上面层数出现过的数组重叠。

这里有个点在于如何在每一种情况中每个数据只能出现一次，要如何实现。

大体存在两个思路：一是再进行一个数组标记，这样凭空增加了空间复杂度，二是在原数组上变化。

代码见下：

```cpp
class Solution {
	public:
		void backtrack(vector<vector<int>> &res, vector<int> &num, int first, int end) {
			if (first == end)res.emplace_back(num); //递归出口
            
			//0-first-1是已经遍历的，first-end是后面需要遍历的
			for (int i = first; i < end; i++) {
				swap(num[i], num[first]);
				backtrack(res, num, first + 1, end);
                //回溯时需要恢复现场
				swap(num[i], num[first]);
			}
		}

		vector<vector<int>> permute(vector<int>& nums) {
			vector<vector<int>> res;
			backtrack(res, nums, 0, (int)nums.size());
			return res;
		}
};
```



## [47. 全排列 II - 力扣（LeetCode）](https://leetcode.cn/problems/permutations-ii/description/)

> 给定一个可包含重复数字的序列 `nums` ，***按任意顺序*** 返回所有不重复的全排列。

这个和上面最大的问题在于如何去除重复的序列。

每个节点的分支不能有相同的元素。

**限界函数：**1.不与之前层的数据重复。2.不能与当层出现的元素重复。

思路一：可以在上述的基础上，对于重复元素不处理（nums[i]!=nums[i-1]类似于之前的字符串从1开始为预留了一个空间)，但是会造成数组的访问问题。

[c++ - 模板中的错误 std::bad_array_new_length - IT工具网 (coder.work)](https://www.coder.work/article/7657208)

思路二：对重复元素的出现采用数组vis记录当时的状态，然后在状态空间树扩展结点的时候要注意不能与之前的重合也不能与该层出现过的一样。也就是递归树深搜和回溯回来两种状态的限制。

代码如下：

```cpp
class Solution {
	public:
		vector<int> vis;//标记选过与未选的状态
		//重复数字在每个树的分支中只能
		void backtrack(vector<int>& nums, vector<vector<int>>& res, int idx, vector<int>&perm) {
			if (idx == nums.size()) {
				res.emplace_back(perm);    //叶子节点时输出当前结果
				return;
			}

			for (int i = 0; i < (int)nums.size(); i++) {
                //这个if语句包含了限界函数的两层含义，满足其中任意一层可跳过这个
                //"||"左边指的是在上面几层已经出现过。
                //"||"右边指的是在回溯回来时，i-1个变成0然后到达第i个，第i-1个为0第i个和前者相同可在这一层不出现
				if (vis[i] || (i > 0 && nums[i] == nums[i - 1] && !vis[i - 1]))continue; //去重
				perm.emplace_back(nums[i]);
				vis[i] = 1;
				backtrack(nums, res, idx + 1, perm);
				vis[i] = 0;
				perm.pop_back();
			}
		}

		vector<vector<int>> permuteUnique(vector<int>& nums) {
			vector<vector<int>> res;
			vector<int> perm;
			vis.resize((int)nums.size());//分配空间
			sort(nums.begin(), nums.end());//排序
			backtrack(nums, res, 0, perm);
			return res;
		}
};
```

