#include <stdio.h>
#include "MathVector.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	// constexpr ink::Vec vec1(10, nullptr, 30);
	// constexpr ink::Vec vec2(10, nullptr, 20);
	// constexpr ink::Vec vec3 = vec1 * vec2;
	
	ink::Vec m(1,2,3);
	m.operator=(ink::Vec(3,2,4));
	
	return 0;
}
