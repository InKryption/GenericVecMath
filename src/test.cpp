#include <stdio.h>
// #include <tuple>
// #include <typeinfo>
#include "MathVector.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	ink::Vec vec1(3, nullptr, 3);
	ink::Vec vec2(1, 3, 5ULL);
	
	auto b = vec1 + vec2;
	
	constexpr ink::detail::Axis<int, ink::Vec<int>::XYZ::Z> a1(3);
	constexpr ink::detail::Axis<void, ink::Vec<int>::XYZ::Z> a2;
	
	
	
	int x = 3;
	ink::Vec<int&, void> f(x);
	
	printf("%i\n", 2);
	
	return 0;
}
