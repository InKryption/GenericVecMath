#include <stdio.h>
#include <tuple>
#include <typeinfo>
#include "MathVector.hpp"
#include <utility>

struct Test {
	int x;
	double y;
	float z;
	
	constexpr
	Test(decltype(x) x, decltype(y) y, decltype(z) z):
	x(x), y(y), z(z) {}
	
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

// Memory unoptimized vector
template<typename T, typename U, typename V>
struct DummyVec
{ T t; U u; V v; };

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec<int, int, void> vec(1,1);
	ink::Vec2<int> f;
	
	return 0;
}
