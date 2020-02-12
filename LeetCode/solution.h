#pragma once

class Solution {
};

template <typename T>
void print(const T& x, bool newline = true) {
	std::cout << x;
	if (newline) {
		std::cout << std::endl;
	}
}