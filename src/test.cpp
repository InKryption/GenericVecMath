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
	
	constexpr
	ink::Vec<int, void, size_t> f(1, nullptr, 2);
	
	return 0;
}
