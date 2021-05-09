#include <stdio.h>
#include "MathVector.hpp"



int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	//auto cross = [](auto&&...) { printf("Hallo!\n"); return 0; };
	constexpr ink::Vec vx('a');
	constexpr ink::Vec<void, char> vy('b');
	constexpr ink::Vec vz(nullptr, nullptr, 'c');
	constexpr ink::Vec<char, char, char> vxyz
	= static_cast<decltype(vxyz)>(vx + vy + vz);
	auto Cross = cross(vx,vy);
	
	return 0;
}
