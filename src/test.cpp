#include <stdio.h>
#include <tuple>
#include "MathVector.hpp"
#include "VectorMath.hpp"
#include <utility>

struct Test {
	int x;
	double y;
	float z;
};

template<size_t Idx> static constexpr auto&&
get(Test&& t) {
	if constexpr(Idx == 0) return t.x;
	if constexpr(Idx == 1) return t.y;
	if constexpr(Idx == 2) return t.z;
}

template<size_t Idx> static constexpr auto&&
get(Test& t) {
	if constexpr(Idx == 0) return t.x;
	if constexpr(Idx == 1) return t.y;
	if constexpr(Idx == 2) return t.z;
}

template<size_t Idx> static constexpr auto&&
get(Test const& t) {
	if constexpr(Idx == 0) return t.x;
	if constexpr(Idx == 1) return t.y;
	if constexpr(Idx == 2) return t.z;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr int i = 3;
	
	constexpr ink::detail::Vec<int, char, int> f{};
	
	return 0;
}
