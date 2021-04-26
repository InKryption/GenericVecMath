#include <stdio.h>
#include <tuple>
#include <typeinfo>
#include "MathVector.hpp"
#include <utility>


int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	
	constexpr void* b = nullptr;
	constexpr ink::Vec vec1(3, 5, b);
	constexpr ink::Vec vec2(1, nullptr, true);
	
	ink::Vec vec_out = vec1 + vec2;
	
	printf("%i %i %i\n", vec_out.x, vec_out.y, vec_out.z);
	
	return 0;
}
