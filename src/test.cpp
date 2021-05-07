#include <stdio.h>
#include "MathVector.hpp"

struct Test {
	int first;
	std::nullptr_t second;
};

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec<void, int, size_t> m{3, nullptr};
	
	printf("%i %i %i\n", m.x, m.y, m.z);
	
	constexpr ink::Vec vec1(10, Test(), 30);
	
	constexpr ink::Vec vec2(10, nullptr, 20);
	
	return 0;
}
