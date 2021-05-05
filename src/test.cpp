#include <stdio.h>

#include "MathVector.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec vec0(nullptr, 9);
	constexpr ink::Vec vec1(-1, 8, -3);
	constexpr ink::Vec vec2(4, 2);
	
	constexpr ink::Vec vec3 = -((0.125) * vec2);
	
	printf("%f %f %f\n", vec3.x, vec3.y, vec3.y);
	
	
	return 0;
}
