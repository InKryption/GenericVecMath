#include <iostream>
#include "MathVector.hpp"



int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec vx('a');
	constexpr ink::Vec<void, char> vy('b');
	constexpr ink::Vec vz(nullptr, nullptr, 'c');
	constexpr ink::Vec vxyz = vx + vy + vz;
	
	return 0;
}
