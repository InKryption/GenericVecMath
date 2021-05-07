#include "MathVector.hpp"



int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec vec1(1,2,2);
	constexpr ink::Vec vec2(4,3,nullptr);
	
	constexpr auto vec3 = vec2 % vec1;
	
	
	
	return 0;
}
