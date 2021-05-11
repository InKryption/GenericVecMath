#include <stdio.h>
#include "MathVector.hpp"



int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr
	ink::Vec<void, int, int> v{{.y = 3, .z = 7ULL}};
	
	static_assert(v.x == 0);
	static_assert(v.y == 3);
	static_assert(v.z == 7);
	
	return 0;
}
