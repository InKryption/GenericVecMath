#include <stdio.h>
#include <numbers>
#include "MathVector.hpp"



int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	int x, y;
	ink::Vec<int&, int&, void> m(x,y);
	
	m = ink::Vec(1,2);
	
	printf("%i %i\n", x, y);
	
	return 0;
}
