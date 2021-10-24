#pragma once
#include "Common.h"

void TypeofAndDecltype()
{
	// "I consider auto to be a purely simplifying feature whereas the primary purpose of decltype is
	//  to enable sophisticated metaprogramming in foundation libraries" 
	auto valueType = 4;
	auto& refenceType = valueType;
	std::cout << typeid(decltype(valueType)).name() << std::endl;
	std::cout << typeid(decltype(refenceType)).name() << std::endl;
}