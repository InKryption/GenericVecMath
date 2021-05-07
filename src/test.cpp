#include <stdio.h>
#include "MathVector.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec vec1(nullptr,nullptr,30);
	constexpr ink::Vec vec2(4,nullptr,15);
	constexpr ink::Vec<void, void, int> vec3((vec1 / 4) * 1.8);
	
	constexpr auto b = (vec3 >= vec2) <= ink::Vec<void>();
	
	(void)vec1;
	(void)vec2;
	(void)vec3;
	
	return 0;
}
