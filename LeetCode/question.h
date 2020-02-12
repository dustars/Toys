#pragma once

#include <vector>
#include "solution.h"

using namespace std;

class MedianOfTwoSortedArrays : Solution {
public:
	double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
		//1. Decide which one is larger
		vector<int>& large = nums1.size() >= nums2.size() ? nums1 : nums2;
		vector<int>& small = nums1.size() >= nums2.size() ? nums2 : nums1;

		auto printV = [](const vector<int>& vec) {for (auto&& i : vec) std::cout << i << ' '; };
		printV(large);
		std::cout << std::endl;
		printV(small);
		std::cout << std::endl;

		if (small.size() == 0) {
			// check wether the larger array is even size or odd size. 
		}

		unsigned int index = 0;
		for (auto&& element : small) {
			
		}

		return 0;
	}
};

// Second time
class LongestSubstringWithoutRepeatingCharacters : Solution {
public:
	int lengthOfLongestSubstring(std::string s) {
		// If string s is empty
		if (s.length() == 0) {
			return 0;
		}

		unsigned short int resultLength = 0;
		std::string tempResult = "";

		// traverse each element in the string s
		for (auto&& element : s) {
			// Find repeated element in current substring
			std::size_t found = tempResult.find(element);
			if (found != std::string::npos) {
				if (tempResult.length() > resultLength) {
					resultLength = tempResult.length();
				}
				tempResult = tempResult.substr(found + 1, tempResult.length());
			}
			tempResult += element;
		}
		return tempResult.length() > resultLength ? tempResult.length() : resultLength;
	}
};



struct ListNode {
	int val;
	ListNode* next;
	ListNode(int x) : val(x), next(NULL) {}
};

// First time
class AddTwoNums : Solution {
public:
	ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
		ListNode* result = new ListNode(0);
		if (l1) {
			if (l2) {			// l1+l2
				result->val = l1->val + l2->val;
				if (result->val >= 10) {
					result->val -= 10;
					if (l1->next || l2->next) {
						if (l1->next) {
							++l1->next->val;
						}
						else {
							++l2->next->val;
						}
					}
					else { // both next are NULL
						result->next = new ListNode(1);
						return result;
					}
				}
				result->next = addTwoNumbers(l1->next, l2->next);
			}
			else {				// l1
				result->val = l1->val;
				if (result->val >= 10) {
					result->val -= 10;
					if (l1->next) {
						++l1->next->val;
					}
					else {
						result->next = new ListNode(1);
						return result;
					}
				}
				result->next = addTwoNumbers(l1->next, NULL);
			}
		}
		else {
			if (l2) {			// l2
				result->val = l2->val;
				if (result->val >= 10) {
					result->val -= 10;
					if (l2->next) {
						++l2->next->val;
					}
					else {
						result->next = new ListNode(1);
						return result;
					}
				}
				result->next = addTwoNumbers(NULL, l2->next);
			}
			else {				// None
				delete result;
				return NULL;
			}
		}
		return result;
	}
};

//Unfinished!!!!
class FourSum : Solution {
public:
	vector<vector<int>> fourSum(vector<int>& nums, int target) {
		vector<vector<int>> solution_set;
		for (int a = 0; a < nums.size(); ++a) {
			for (int b = a + 1; b < nums.size(); ++b) {
				for (int c = b + 1; c < nums.size(); ++c) {
					for (int d = c + 1; d < nums.size(); ++d) {
						if (nums[a] + nums[b] + nums[c] + nums[d] == target) {
							vector<int> ans{ nums[a], nums[b], nums[c], nums[d] };
							for (auto iterator = solution_set.begin(); ; ++iterator) {
								if (*iterator == ans) {
									break;
								}
								if (iterator == solution_set.end()) {
									solution_set.push_back(ans);
								}
							}
						}
					}
				}
			}
		}
		return solution_set;
	}
};

// First time
class TwoSum : Solution {
public:
	vector<int> twoSum(vector<int>& nums, int target) {
		for (int i = 0; i < nums.size(); ++i) {
			for (int j = 0; j < nums.size(); ++j) {
				if (i == j) {
					continue;
				}
				if (nums[i] + nums[j] == target) {
					vector<int> ans{ i,j };
					return ans;
				}
			}
		}
		vector<int> ans{ 0,0 };
		return ans;
	}
};
