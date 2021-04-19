#ifndef INK_MATH_VECTOR_HEADER_FILE_GUARD
#define INK_MATH_VECTOR_HEADER_FILE_GUARD

#include <cstdlib>
#include <type_traits>

namespace ink {
	
	namespace internal {
		
		template<typename X, typename Y, typename Z>
		struct Vec_Impl;
		
		template<typename X, typename Y, typename Z> requires( alignof(X) >= alignof(Y) && alignof(Y) >= alignof(Z) && alignof(X) >= alignof(Z) )
		struct Vec_Impl<X, Y, Z>
		{
			constexpr
			Vec_Impl() requires( std::default_initializable<X> && std::default_initializable<Y> && std::default_initializable<Z> ):
			x(), y(), z() {}
			
			constexpr
			Vec_Impl(X&& x, Y&& y, Z&& z):
				x(std::forward<X>(x)),
				y(std::forward<Y>(y)),
				z(std::forward<Z>(z))
			{}
			
			X x; Y y; Z z;
		};
		
		template<typename X, typename Y, typename Z> requires( alignof(X) > alignof(Y) && alignof(Y) < alignof(Z) && alignof(X) >= alignof(Z) )
		struct Vec_Impl<X, Y, Z>
		{
			constexpr
			Vec_Impl() requires( std::default_initializable<X> && std::default_initializable<Y> && std::default_initializable<Z> ):
			x(), z(), y() {}
			
			constexpr
			Vec_Impl(X&& x, Y&& y, Z&& z):
				x(std::forward<X>(x)),
				z(std::forward<Z>(z)),
				y(std::forward<Y>(y))
			{}
			
			X x; Z z; Y y;
	
		};
		
		template<typename X, typename Y, typename Z> requires( alignof(X) < alignof(Y) && alignof(Y) > alignof(Z) && alignof(X) >= alignof(Z) )
		struct Vec_Impl<X, Y, Z>
		{
			constexpr
			Vec_Impl() requires( std::default_initializable<X> && std::default_initializable<Y> && std::default_initializable<Z> ):
			y(), x(), z() {}
			
			constexpr
			Vec_Impl(X&& x, Y&& y, Z&& z):
				y(std::forward<Y>(y)),
				x(std::forward<X>(x)),
				z(std::forward<Z>(z))
			{}
			
			Y y; X x; Z z;
	
		};
		
		template<typename X, typename Y, typename Z> requires( alignof(X) < alignof(Y) && alignof(Y) > alignof(Z) && alignof(X) < alignof(Z) )
		struct Vec_Impl<X, Y, Z>
		{
			constexpr
			Vec_Impl() requires( std::default_initializable<X> && std::default_initializable<Y> && std::default_initializable<Z> ):
			y(), z(), x() {}
			
			constexpr
			Vec_Impl(X&& x, Y&& y, Z&& z):
				y(std::forward<Y>(y)),
				z(std::forward<Z>(z)),
				x(std::forward<X>(x))
			{}
			
			Y y; Z z; X x;
	
		};
		
		template<typename X, typename Y, typename Z> requires( alignof(X) >= alignof(Y) && alignof(Y) < alignof(Z) && alignof(X) < alignof(Z) )
		struct Vec_Impl<X, Y, Z>
		{
			constexpr
			Vec_Impl() requires( std::default_initializable<X> && std::default_initializable<Y> && std::default_initializable<Z> ):
			z(), x(), y() {}
			
			constexpr
			Vec_Impl(X&& x, Y&& y, Z&& z):
				z(std::forward<Z>(z)),
				x(std::forward<X>(y)),
				y(std::forward<Y>(z))
			{}
			
			Z z; X x; Y y;
			
		};
		
		
		
		template<typename X, typename Y>
		struct Vec_Impl<X, Y, void>
		{
			constexpr
			Vec_Impl() requires( std::default_initializable<X> && std::default_initializable<Y> ):
			x(), y() {}
			
			constexpr
			Vec_Impl(X&& x, Y&& y):
			x(std::forward<X>(x)),
			y(std::forward<Y>(y)) {}
			
			X x; Y y;
		};
		
		template<typename X, typename Z>
		struct Vec_Impl<X, void, Z>
		{
			constexpr
			Vec_Impl() requires( std::default_initializable<X> && std::default_initializable<Z> ):
			x(), z() {}
			
			constexpr
			Vec_Impl(X&& x, Z&& z):
			x(std::forward<X>(x)),
			z(std::forward<Z>(z)) {}
			
			X x; Z z;
		};
		
	}
	
	namespace detail {
		
		template<typename T>
		concept default_ctr_or_void = std::is_same_v<T, void> || std::is_default_constructible_v<T>;
		
		template<typename X, typename Y = X, typename Z = void>
		class Vec: public internal::Vec_Impl<X, Y, Z> {
			
			private: using
			underlying = internal::Vec_Impl<X, Y, Z>;
			
			public: constexpr
			Vec() requires(default_ctr_or_void<X> && default_ctr_or_void<Y> && default_ctr_or_void<Z>):
			underlying() {}
			
			//public: constexpr
			//Vec()
			
		};
		
		
		
	}
	
}

#endif
