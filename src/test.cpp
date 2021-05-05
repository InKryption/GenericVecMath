#include "MathVector.hpp"
#include <stdio.h>
#include <cstdlib>
#include <numbers>
#include <tuple>
#include <type_traits>
#include <cmath>



int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec vec1(std::numbers::sqrt2, std::numbers::sqrt2);
	constexpr ink::Vec vec2(32,2.5);
	
	constexpr auto b = mag2(vec1);
	
	printf("%f\n", b);
	
	
}
