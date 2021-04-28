#include <stdio.h>
// #include <tuple>
// #include <typeinfo>
#include "MathVector.hpp"



int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	using ink::detail::XYZ;
	using ink::detail::Axis;
	
	
	constexpr Axis<void, XYZ::X> a1{};
	constexpr Axis<int, XYZ::X> a2{3};
	constexpr Axis<int, XYZ::X> a3{a2};
	
	constexpr decltype(a2) b = a2 || a1;
	
	constexpr ink::Vec vec1(nullptr, 3, 4);
	constexpr ink::Vec vec2(3, 3, 3ull);
	
	
	return 0;
}
