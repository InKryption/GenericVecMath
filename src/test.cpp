#include <stdio.h>
#include "MathVector.hpp"



int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec<int, void, int> f(2, 3);
	constexpr auto b = f.z;
	(void)f;
	
	/*DO NOT ERASE - FIX FIRST*/
	// int x = 3;
	// ink::Vec<int&, void> rvec(x, nullptr, nullptr);
	
	return 0;
}
