#include "MathVector.hpp"
#include <stdio.h>
#include <cstdlib>
#include <tuple>
#include <type_traits>
#include <cmath>



int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec vec1(2.5, 7.5);
	constexpr ink::Vec vec2(3, 5, 55);
	
	auto b = vec1 % vec2;
	
}
