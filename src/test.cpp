#include <stdio.h>
// #include <memory>
#include "MathVector2.hpp"



int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec vec1(1,	2,		nullptr);
	constexpr ink::Vec vec2(2,	2.f,	1);
	
	constexpr ink::Vec vec3 = vec1 + vec2;
	
	constexpr auto b = std::convertible_to<std::nullptr_t, char&>;
	
	int n{};
	int& s{n};
	char c = 'a';
	ink::Vec<int&, char&, void> SVec(n, c, nullptr);
	
	
	return 0;
}
