#include <stdio.h>
#include "MathVector.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec vec1(nullptr,nullptr,30);
	constexpr ink::Vec vec2(4,nullptr,15);
	constexpr ink::Vec<void, void, size_t> vec3(vec1 / 0.00000000000000001);
	
	(void)vec1;
	(void)vec2;
	(void)vec3;
	
	return 0;
}
