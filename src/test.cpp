#include <stdio.h>
#include <tuple>
#include <typeinfo>
#include "MathVector.hpp"
#include <utility>


int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	
	
	constexpr ink::Vec vec1(3, 5, true);
	constexpr ink::Vec vec2(1, nullptr, true);
	
	constexpr ink::Vec out = vec1 + vec2;
	
	printf("%i %i %i\n", out.x, out.y, out.z);
	
	return 0;
}
