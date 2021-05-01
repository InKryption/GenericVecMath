#include <stdio.h>
#include <memory>
// #include <tuple>
// #include <typeinfo>
#include "MathVector.hpp"



int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec v1(3, 2, (int)'a');
	constexpr ink::Vec<int, int, int> v2(v1);
	
	printf("%i, %i, %c\n", v2.x, v2.y, v2.z);
	
	return 0;
}
