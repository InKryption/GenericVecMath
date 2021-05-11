#include <stdio.h>
#include "MathVector.hpp"



int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec boolVec(false, false, false);
	constexpr auto b = boolVec || true;
	
	
	return 0;
}
