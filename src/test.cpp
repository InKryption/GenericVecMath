#include <stdio.h>
#include "MathVector.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec vec1(10, nullptr, 30);
	constexpr ink::Vec vec2(4, 3, 15);
	constexpr ink::Vec vec3((vec1 / 4) * 1.5);
	
	constexpr auto bvec = +vec1;
	
	(void)vec1;
	(void)vec2;
	(void)vec3;
	
	return 0;
}
