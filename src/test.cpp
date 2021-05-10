#include <stdio.h>
#include "MathVector.hpp"

struct Test {
	constexpr Test operator*(int) { return Test(); }
};

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	
	
	constexpr ink::Vec v1(2, nullptr, Test());
	
	
	
	
	return 0;
}
