#include <stdio.h>
#include <memory>
// #include <tuple>
// #include <typeinfo>
#include "MathVector.hpp"



int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec v1(1, 4, 14);
	constexpr ink::Vec v2(3, 7, 2.5);
	
	constexpr auto b = v1 * v2;
	
	constexpr auto res = b.z;
	
	return 0;
}
