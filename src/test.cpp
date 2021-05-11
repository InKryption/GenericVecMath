#include <stdio.h>
#include "MathVector.hpp"

struct Test {
	char value = '\0';
	// constexpr Test operator*(int) { return Test(); }
};

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	ink::Vec<int, int, Test> v1(2, nullptr, Test());
	ink::Vec<void, int, Test> m{(v1)};
	auto b = v1.z;
	
	return 0;
}
