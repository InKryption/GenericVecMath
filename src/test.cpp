#include <stdio.h>
#include "MathVector.hpp"



int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec v1(2, 6, 4);
	
	constexpr ink::Vec b = 1.5 * v1;
	
	return 0;
}
