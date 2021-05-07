#include <stdio.h>
#include "MathVector.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	// constexpr ink::Vec vec1(10, nullptr, 30);
	// constexpr ink::Vec vec2(10, nullptr, 20);
	// constexpr ink::Vec vec3 = vec1 * vec2;
	
	int x, y;
	ink::Vec<int&, int&>(x,y) = ink::Vec{57, 42};
	
	printf("%i %i\n", x, y);
	
	return 0;
}
