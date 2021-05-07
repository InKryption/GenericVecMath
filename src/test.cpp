#include <stdio.h>
#include "MathVector.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec vec1(3, 0, -1);
	constexpr ink::Vec vec2(0, 3, 1);
	constexpr ink::Vec vec3 = vec1 * vec2;
	
	constexpr auto b = cross(vec1, vec2);
	
	return 0;
}
