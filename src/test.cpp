#include <stdio.h>
// #include <tuple>
// #include <typeinfo>
#include "MathVector.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	auto v = ink::Vec(3, 3);
	
	constexpr ink::Vec<int, void, int> vtest(5, nullptr, 3);
	constexpr ink::Vec<int, int, void> vtest2(1, 1);
	
	constexpr auto b = vtest + vtest;
	constexpr auto c = b + vtest2;
	
	return 0;
}
