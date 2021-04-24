#include <stdio.h>
#include <tuple>
#include <typeinfo>
#include "MathVector.hpp"
#include <utility>



int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec vec1(3, 7ull, 3);
	
	constexpr ink::detail::Axis<int, ink::detail::XYZ::X> A1(1);
	constexpr ink::detail::Axis<void, ink::detail::XYZ::X> A2;
	
	constexpr auto b = A2 * A1;
	
	return 0;
}
