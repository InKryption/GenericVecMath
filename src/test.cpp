#include <stdio.h>
// #include <memory>
#include "MathVector2.hpp"

struct Signal {
	Signal() { puts("Default constructor"); }
	Signal(Signal const&) { puts("Copy constructor"); }
	Signal(Signal &&) { puts("Copy constructor"); }
	Signal(int) { puts("Int Argument constructor"); }
};

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec vec1(1,	2,		nullptr);
	constexpr ink::Vec vec2(1,	2.f,	nullptr);
	
	constexpr ink::Vec vec3 = vec1 + vec2;
	
	int n{};
	int& s{n};
	char c = 'a';
	ink::Vec<int&, char&, void> SVec(n, c, nullptr);
	
	
	
	return 0;
}
