#include <stdio.h>
// #include <tuple>
// #include <typeinfo>
#include "MathVector.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec vec1(nullptr, 3, 4);
	constexpr ink::Vec vec2(3, 3, 3ull);
	
	constexpr ink::Vec VECOUT = vec1 - vec2;
	
	
	return 0;
}
