#pragma once

#include "question.h"

using namespace std;

void Question94() {
	InorderTraversalBTree solution;

	TreeNode* root = new TreeNode(1);
	root->right = new TreeNode(2);
	root->right->left = new TreeNode(3);
	solution.inorderTraversal(root);
	for (auto&& element : solution.result) {
		cout << element << " ";
	}
}

void Question4() {
	MedianOfTwoSortedArrays solution;

	vector<int> nums1({ 1, 3, 5, 7, 9,9,9,9});
	vector<int> nums2({});

	int result = solution.findMedianSortedArrays(nums1, nums2);
	print("The median is: ", false);
	print(result);
}

void Question3() {
	LongestSubstringWithoutRepeatingCharacters solution;
	std::string temp = "abcabcdbebabc";
	print(temp);
	print(solution.lengthOfLongestSubstring(temp));
}

void Question2() {
	AddTwoNums solution;
	ListNode* l1 = new ListNode(1);
	l1->next = new ListNode(4);
	l1->next->next = new ListNode(3);
	l1->next->next->next = new ListNode(5);

	ListNode* l2 = new ListNode(9);
	l2->next = new ListNode(9);
	l2->next->next = new ListNode(9);

	ListNode* result = solution.addTwoNumbers(l1, l2);
	while (result) {
		std::cout << result->val << "->";
		result = result->next;
	}
}

void Question1() {

}