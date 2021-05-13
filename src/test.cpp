#include <stdio.h>
#include <tuple>
#include "MathVector.hpp"



int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr
	ink::Vec vec1(nullptr,nullptr,-5);
	
	constexpr
	ink::Vec vec2(nullptr,nullptr,1);
	
	
	
	constexpr
	auto cross = ink::cross(vec1,vec2);
	
	
	
	return 0;
}
