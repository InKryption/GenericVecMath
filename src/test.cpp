#include <stdio.h>
#include <typeinfo>
#include <numbers>
#include <concepts>
#include <tuple>
#include <cmath>
#include <assert.h>

// #include <iostream>
// #include <iomanip>

#include "MathVector.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec vec0(nullptr, 9);
	constexpr ink::Vec vec1(-1, 8, -3);
	constexpr ink::Vec vec2(4, 2);
	
	constexpr ink::Vec vec3 = vec2 * 2;
	
	printf("%i %i %i\n", vec3.x, vec3.y, vec3.z);
	
	
	return 0;
}
