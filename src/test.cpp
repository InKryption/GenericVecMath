#include <stdio.h>
#include "MathVector.hpp"



int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	int x = 3;
	ink::Vec<int&, void> rvec(x, nullptr);
	
	return 0;
}
