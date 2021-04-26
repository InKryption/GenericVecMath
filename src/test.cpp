#include <stdio.h>
// #include <tuple>
// #include <typeinfo>
#include "MathVector.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr int* nullv = nullptr;
	constexpr ink::Vec vec1(3, 5, 3);
	constexpr ink::Vec vec2(1, nullptr, 5ULL);
	
	constexpr ink::Vec vec_out = vec1 + vec2;
	
	printf("%i %i %llu\n", vec_out.x, vec_out.y, vec_out.z);
	
	return 0;
}
