#include <stdio.h>
#include "MathVector.hpp"



int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr
	ink::Vec<int, void, int> v{{.x = 3, .y = nullptr, .z = 7ULL}};
	
	printf("%i %llu %llu\n", v.x, v.y, v.z);
	
	return 0;
}
