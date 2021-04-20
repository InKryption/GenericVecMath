#ifndef INK_MATH_VECTOR_HEADER_FILE_GUARD
#define INK_MATH_VECTOR_HEADER_FILE_GUARD

#include <cstdlib>
#include <concepts>

namespace ink {
	
	namespace internal {
		
		template<typename X, typename Y, typename Z>
		struct Vec_Impl;
		
		template<typename X, typename Y, typename Z> requires(	(alignof(X)	>=	alignof(Y))	&&	(alignof(Y)	>=	alignof(Z))	&&	(alignof(X)	>=	alignof(Z))	)
		struct Vec_Impl<X, Y, Z>
		{
			X x; Y y; Z z;
			public: constexpr
			Vec_Impl(X&& x, Y&& y, Z&& z):
				x(std::forward<X>(x)),
				y(std::forward<Y>(y)),
				z(std::forward<Z>(z))
			{}
		};
		
		template<typename X, typename Y, typename Z> requires(	(alignof(X)	>	alignof(Y))	&&	(alignof(Y)	<	alignof(Z))	&&	(alignof(X)	>=	alignof(Z))	)
		struct Vec_Impl<X, Y, Z>
		{
			X x; Z z; Y y;
			public: constexpr
			Vec_Impl(X&& x, Y&& y, Z&& z):
				x(std::forward<X>(x)),
				z(std::forward<Z>(z)),
				y(std::forward<Y>(y))
			{}
		};
		
		template<typename X, typename Y, typename Z> requires(	(alignof(X)	<	alignof(Y))	&&	(alignof(Y)	>	alignof(Z))	&&	(alignof(X)	>=	alignof(Z))	)
		struct Vec_Impl<X, Y, Z>
		{
			Y y; X x; Z z;
			public: constexpr
			Vec_Impl(X&& x, Y&& y, Z&& z):
				y(std::forward<Y>(y)),
				x(std::forward<X>(x)),
				z(std::forward<Z>(z))
			{}
		};
		
		template<typename X, typename Y, typename Z> requires(	(alignof(X)	<	alignof(Y))	&&	(alignof(Y)	>	alignof(Z))	&&	(alignof(X)	<	alignof(Z))	)
		struct Vec_Impl<X, Y, Z>
		{
			Y y; Z z; X x;
			public: constexpr
			Vec_Impl(X&& x, Y&& y, Z&& z):
				y(std::forward<Y>(y)),
				z(std::forward<Z>(z)),
				x(std::forward<X>(x))
			{}
		};
		
		template<typename X, typename Y, typename Z> requires(	(alignof(X)	>=	alignof(Y))	&&	(alignof(Y)	<	alignof(Z))	&&	(alignof(X)	<	alignof(Z))	)
		struct Vec_Impl<X, Y, Z>
		{
			Z z; X x; Y y;
			public: constexpr
			Vec_Impl(X&& x, Y&& y, Z&& z):
				z(std::forward<Z>(z)),
				x(std::forward<X>(x)),
				y(std::forward<Y>(y))
			{}
		};
		
		template<typename X, typename Y, typename Z> requires(	(alignof(X)	<	alignof(Y))	&&	(alignof(Y)	<	alignof(Z))	&&	(alignof(X)	<	alignof(Z))	)
		struct Vec_Impl<X, Y, Z>
		{
			Z z; Y y; X x;
			public: constexpr
			Vec_Impl(X&& x, Y&& y, Z&& z):
				z(std::forward<Z>(z)),
				y(std::forward<Y>(y)),
				x(std::forward<X>(x))
			{}
		};
		
		
		
		template<typename X, typename Y>
		struct Vec_Impl<X, Y, void>
		{
			X x; Y y;
			public: constexpr
			Vec_Impl(X&& x, Y&& y, [[maybe_unused]] std::nullptr_t z = nullptr):
				x(std::forward<X>(x)),
				y(std::forward<Y>(y))
			{}
		};
		
		template<typename X, typename Z>
		struct Vec_Impl<X, void, Z>
		{
			X x; Z z;
			public: constexpr
			Vec_Impl(X&& x, [[maybe_unused]] std::nullptr_t y, Z&& z):
				x(std::forward<X>(x)),
				z(std::forward<Z>(z))
			{}
		};
		
		template<typename Y, typename Z>
		struct Vec_Impl<void, Y, Z>
		{
			Y y; Z z;
			public: constexpr
			Vec_Impl([[maybe_unused]] std::nullptr_t x, Y&& y, Z&& z):
				y(std::forward<Y>(y)),
				z(std::forward<Z>(z))
			{}
			
		};
		
		
		
		template<> struct Vec_Impl<void, void, void>;
		template<typename X> struct Vec_Impl<X, void, void>;
		template<typename Y> struct Vec_Impl<void, Y, void>;
		template<typename Z> struct Vec_Impl<void, void, Z>;
		
	}
	
	namespace detail {
		
		template<typename T>
		concept default_ctr_or_void = std::is_same_v<T, void> || std::is_default_constructible_v<T>;
		
		template<typename X, typename Y = X, typename Z = void>
		class Vec: public internal::Vec_Impl<X, Y, Z> {
			
			private: using
			underlying = internal::Vec_Impl<X, Y, Z>;
			
			public: constexpr
			Vec() requires(default_ctr_or_void<X> && default_ctr_or_void<Y> && default_ctr_or_void<Z>): underlying(
				std::conditional_t<(std::same_as<void, X>), std::nullptr_t, X>(),
				std::conditional_t<(std::same_as<void, Y>), std::nullptr_t, Y>(),
				std::conditional_t<(std::same_as<void, Z>), std::nullptr_t, Z>())
			{}
			
			public: constexpr
			Vec(	std::conditional_t<(std::same_as<void, X>), std::nullptr_t, X&&> x,
					std::conditional_t<(std::same_as<void, Y>), std::nullptr_t, Y&&> y,
					std::conditional_t<(std::same_as<void, Z>), std::nullptr_t, Z&&> z
			): underlying(
				std::forward<X>(x),
				std::forward<Y>(y),
				std::forward<Z>(z))
			{}
			
		};
		
	}
	
}

#endif
