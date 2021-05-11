#include <stdio.h>
#include <tuple>
#include "MathVector.hpp"



int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr
	ink::Vec vec1(1,2,3);
	
	constexpr
	ink::Vec vec2(3,2,1);
	
	constexpr
	auto dot = ink::generic_vec::dot(vec1,vec2);
	
	printf("%i\n", dot);
	
	return 0;
}
