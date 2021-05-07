#include "MathVector.hpp"



int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec vec1(nullptr,nullptr,6);
	constexpr ink::Vec vec2(4,nullptr,15);
	
	constexpr auto vec3 = vec1 / vec2;
	
	
	
	return 0;
}
