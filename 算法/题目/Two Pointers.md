# Two Pointers



来源：Leetcode

做题时间：2023.12.14—2023.12.

https://algo.itcharge.cn/01.Array/04.Array-Two-Pointers/01.Array-Two-Pointers/

好文章，讲解双指针的，推荐先看看文章。

- **对撞指针**：两个指针方向相反。适合解决查找有序数组中满足某些约束条件的一组元素问题、字符串反转问题。
- **快慢指针**：两个指针方向相同。适合解决数组中的移动、删除元素问题，或者链表中的判断是否有环、长度问题。
- **分离双指针**：两个指针分别属于不同的数组 / 链表。适合解决有序数组合并，求交集、并集问题。

（摘自上面链接中的文章）



### [141. 环形链表 - 力扣（LeetCode）](https://leetcode.cn/problems/linked-list-cycle/description/)

> 给你一个链表的头节点 `head` ，判断链表中是否有环。
>
> 如果链表中有某个节点，可以通过连续跟踪 `next` 指针再次到达，则链表中存在环。 为了表示给定链表中的环，评测系统内部使用整数 `pos` 来表示链表尾连接到链表中的位置（索引从 0 开始）。**注意：`pos` 不作为参数进行传递** 。仅仅是为了标识链表的实际情况。
>
> *如果链表中存在环* ，则返回 `true` 。 否则，返回 `false` 。

。。乍一看，这跟双指针有什么关系。。

**快慢指针**

但是如果判断环的话，假设存在两个指针，那么在环内一定会相遇。

```cpp
bool hasCycle(ListNode *head) {
	if (head == nullptr || head->next == nullptr) return false;
	ListNode*t1 = head, *t2 = head->next;
	while (t1 && t2) {
		if (t1 == t2)return true;
		t1 = t1->next;
		t2 = t2->next;
		if (t2) t2 = t2->next;
	}
	return false;
}
```



### [142. 环形链表 II - 力扣（LeetCode）](https://leetcode.cn/problems/linked-list-cycle-ii/description/)

> 给定一个链表的头节点  `head` ，返回链表开始入环的第一个节点。 *如果链表无环，则返回 `null`。*
>
> 如果链表中有某个节点，可以通过连续跟踪 `next` 指针再次到达，则链表中存在环。 为了表示给定链表中的环，评测系统内部使用整数 `pos` 来表示链表尾连接到链表中的位置（**索引从 0 开始**）。如果 `pos` 是 `-1`，则在该链表中没有环。**注意：`pos` 不作为参数进行传递**，仅仅是为了标识链表的实际情况。
>
> **不允许修改** 链表。

如何返回进入环时的第一个结点。

好像是追击问题，如果数学推导的话应该能得到一个公式，不过今天不想推导，下回再推。



### [88. 合并两个有序数组 - 力扣（LeetCode）](https://leetcode.cn/problems/merge-sorted-array/description/)

> 给你两个按 **非递减顺序** 排列的整数数组 `nums1` 和 `nums2`，另有两个整数 `m` 和 `n` ，分别表示 `nums1` 和 `nums2` 中的元素数目。
>
> 请你 **合并** `nums2` 到 `nums1` 中，使合并后的数组同样按 **非递减顺序** 排列。
>
> **注意：**最终，合并后数组不应由函数返回，而是存储在数组 `nums1` 中。为了应对这种情况，`nums1` 的初始长度为 `m + n`，其中前 `m` 个元素表示应合并的元素，后 `n` 个元素为 `0` ，应忽略。`nums2` 的长度为 `n` 。

真好，快乐练手的双指针，注意一下数组的边界就行。

**分离双指针**

```cpp
void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
	vector<int> res(m + n);
	int n0 = 0;
	int i = 0, j = 0;
	while (i < m && j < n) {
		if (nums1[i] >= nums2[j]) res[n0++] = nums2[j++];
		else res[n0++] = nums1[i++];
	}
    
	while (i < m)res[n0++] = nums1[i++];
	while (j < n)res[n0++] = nums2[j++];
    
	nums1 = res;
}
```

