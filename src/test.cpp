#include "MathVector.hpp"
#include <stdio.h>
#include <cstdlib>
#include <cstdint>
#include <numbers>
#include <numeric>
#include <tuple>
#include <memory>
#include <type_traits>
#include <cmath>

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec vec1(3);
	constexpr ink::Vec vec2(5);
	
	int x = 0, y = 0, z = 0;
	
	ink::Vec<int&, int&, int&> ref_vec(x,y,z);
	
	constexpr ink::Vec v = cross(vec1, vec2);
	
	printf("%i\n", 3);
	
	
}
