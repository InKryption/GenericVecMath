#include <stdio.h>
#include <numbers>
#include "MathVector.hpp"



int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	using std::numbers::sqrt2;
	constinit static int m = 3;
	printf("%f\n", std::sqrt(ink::Vec{sqrt2,sqrt2}.mag2()) );
	
	return 0;
}
