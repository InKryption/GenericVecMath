#include <stdio.h>
#include <tuple>
#include <typeinfo>
#include "MathVector.hpp"
#include <utility>


template<typename X, typename Y = X>
using VecXY = ink::Vec<X, Y, void>;

template<typename X, typename Z = X>
using VecXZ = ink::Vec<X, void, Z>;

template<typename Y, typename Z = Y>
using VecYZ = ink::Vec<void, Y, Z>;


int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr
	VecYZ<int> f(1, 100);
	constexpr auto b = f.z;
	
	
	
	return 0;
}
