#include <stdio.h>
// #include <tuple>
// #include <typeinfo>
#include "MathVector.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	auto v = ink::Vec(3, 3);
	
	ink::Vec<int&, int&> vref(v.x, v.y);
	
	auto&& b = get<2>(vref);
	
	return 0;
}
