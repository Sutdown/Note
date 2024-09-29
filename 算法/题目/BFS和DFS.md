# DFS和BFS ， 回溯和分支定界



参考：ppt，力扣，算法总结ppt，作业参考答案

标签：算法，算法习题，回溯，分支定界，BFS，DFS



[TOC]

# Leetcode

## Breadth First Search

### [101. 对称二叉树 - 力扣（LeetCode）](https://leetcode.cn/problems/symmetric-tree/)

> 给你一个二叉树的根节点 `root` ， 检查它是否轴对称。

题目难度为easy，拿来作为宽搜的练习题目还是可以的。

观察轴对称有点类似分治的思路，就是左右两边，左边的结点等于右边，这样子看着可以用递归的思路做。

另一种是作为队列的形式，将树的结点的两边从上到下，一边放入，再接着取出，同时是宽度优先搜索。

```cpp
//BFS
bool isSymmetric(TreeNode* root) {
	bool res = true;
	queue<TreeNode*> q;

	q.push(root->left);
	q.push(root->right);
	while (!q.empty()) {
		TreeNode* t1, *t2;
		t1 = q.front(), q.pop();
		t2 = q.front(), q.pop();

		if (!t1 && !t2)continue;
		if ((!t1 || !t2) || (t1->val != t2->val))return false;

		q.push(t1->left);
		q.push(t2->right);

		q.push(t1->right);
		q.push(t2->left);
	}

	return res;
}

//递归recuision
bool check(TreeNode *p, TreeNode *q) {
    if (!p && !q) return true;
    if (!p || !q) return false;
        return p->val == q->val && check(p->left, q->right) && check(p->right, q->left);
}

bool isSymmetric(TreeNode* root) {
    return check(root, root);
}
```



### [102. 二叉树的层序遍历 - 力扣（LeetCode）](https://leetcode.cn/problems/binary-tree-level-order-traversal/description/)

> 给你二叉树的根节点 `root` ，返回其节点值的 **层序遍历** 。 （即逐层地，从左到右访问所有节点）。

看着难度不大，就一层层遍历下来就行。

层数的话可以在队列里面取个特殊值标记。

好的，标记这个思路并不可行，其实可以直接记录每一行的值，因为宽度搜索每次放入一行，记录当时的队列大小就可以了。

```cpp
vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> res;
        if(!root)return res;

        queue<TreeNode*> q;
        q.push(root);

        while(!q.empty()){
           int cz=q.size();
           res.push_back(vector<int>());//为res中增加一个int类型的数组
           for(int i=1;i<=cz;i++){
               auto node=q.front();q.pop();
               res.back().push_back(node->val);//先到达数组的最后，再加上相应的元素

               if(node->left)q.push(node->left);//放入下一层的元素
               if(node->right)q.push(node->right);
           }
        }

        return res;
    }
```



### [107. 二叉树的层序遍历 II - 力扣（LeetCode）](https://leetcode.cn/problems/binary-tree-level-order-traversal-ii/description/)

> 给你二叉树的根节点 `root` ，返回其节点值 **自底向上的层序遍历** 。 （即按从叶子节点所在层到根节点所在的层，逐层从左向右遍历）

给的是根节点，自底而上，可以采用栈的形式。

栈的话可以是可以，但是题目中所需要返回的每一层内部的顺序还是从左到右的，只是层层之间的顺序调换了。但是如果栈的话会改变所有的顺序，不能达到题干的要求。

单独的调换层和层之间的话，使用reverse就可以达到效果，在上一题的最后加上就行。



## Depth First Search

### [111. 二叉树的最小深度 - 力扣（LeetCode）](https://leetcode.cn/problems/minimum-depth-of-binary-tree/description/)

> 给定一个二叉树，找出其最小深度。
>
> 最小深度是从根节点到最近叶子节点的最短路径上的节点数量。
>
> **说明：**叶子节点是指没有子节点的节点。

简单题，练手。深搜。

```cpp
int res = 1e5 + 10;
void dfs(TreeNode*node, int i) {
	if (node->left == nullptr && node->right == nullptr) {
		res = min(res, i);
		return;//递归出口
	}

	if (node->left)dfs(node->left, i + 1);
	if (node->right)dfs(node->right, i + 1);
}

int minDepth(TreeNode* root) {
	if (!root)return 0;
	dfs(root, 0);
	return res + 1;
}
```



### [112. 路径总和 - 力扣（LeetCode）](https://leetcode.cn/problems/path-sum/description/)

> 给你二叉树的根节点 `root` 和一个表示目标和的整数 `targetSum` 。判断该树中是否存在 **根节点到叶子节点** 的路径，这条路径上所有节点值相加等于目标和 `targetSum` 。如果存在，返回 `true` ；否则，返回 `false` 。
>
> **叶子节点** 是指没有子节点的节点。

最短路径的简单版本。最短路径是位于路劲上的，所以可以用cw记录，但是结点上的值在每条路径结束后还会剩下一个，因此代码如下：

```cpp
int cw = 0;
bool res = false;
void dfs(TreeNode* node, int targetSum) {
	if (node->left == nullptr && node->right == nullptr) {
		if (targetSum == node->val)res = true;//剩下的一个未加入的叶子节点
		return;
	}

	if (node->left)dfs(node->left, targetSum - node->val);//深度优先搜索
	if (node->right)dfs(node->right, targetSum - node->val);
}

bool hasPathSum(TreeNode* root, int targetSum) {
	if (!root)return false;
	dfs(root, targetSum);
	return res;
}
```

这第二个是根据113写出来的，也就是改变了些许位置，从而能够顾及到所有节点，跟最短路径也就相差了一点。

```cpp
bool res = false;
void dfs(TreeNode* node, int targetSum) {
	targetSum -= node->val;//

	if (node->left == nullptr && node->right == nullptr)
		if (targetSum == 0) res = true; //剩下的一个未加入的叶子节点

	if (node->left)dfs(node->left, targetSum);//深度优先搜索
	if (node->right)dfs(node->right, targetSum);
}

bool hasPathSum(TreeNode* root, int targetSum) {
	if (!root)return false;
	dfs(root, targetSum);
	return res;
}
```



### [113. 路径总和 II - 力扣（LeetCode）](https://leetcode.cn/problems/path-sum-ii/)

> 给你二叉树的根节点 `root` 和一个整数目标和 `targetSum` ，找出所有 **从根节点到叶子节点** 路径总和等于给定目标和的路径。
>
> **叶子节点** 是指没有子节点的节点。

和上面的不同在于找出所有路径并输出。

更改最短路径的思想，挨个处理每一个结点的同时记录路径即可，同时对数组进行增删。

```cpp
vector<vector<int>> res;
vector<int> tmp;

void dfs(TreeNode* node, int targetSum) {
	tmp.emplace_back(node->val);
	targetSum -= node->val;

	if (node->left == nullptr && node->right == nullptr)
		if (targetSum == 0) res.emplace_back(tmp);//剩下的一个未加入的叶子节点

	if (node->left)dfs(node->left, targetSum);//深度优先搜索
	if (node->right)dfs(node->right, targetSum);
	tmp.pop_back();//
}

vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
	if (!root)return res;
	dfs(root, targetSum);
	return res;
}
```



注：暂时这些题目，以后碰到类似的思想再更新。
