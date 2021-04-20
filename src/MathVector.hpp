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
		{ X x; Y y; Z z; };
		
		template<typename X, typename Y, typename Z> requires(	(alignof(X)	>	alignof(Y))	&&	(alignof(Y)	<	alignof(Z))	&&	(alignof(X)	>=	alignof(Z))	)
		struct Vec_Impl<X, Y, Z>
		{ X x; Z z; Y y; };
		
		template<typename X, typename Y, typename Z> requires(	(alignof(X)	<	alignof(Y))	&&	(alignof(Y)	>	alignof(Z))	&&	(alignof(X)	>=	alignof(Z))	)
		struct Vec_Impl<X, Y, Z>
		{ Y y; X x; Z z; };
		
		template<typename X, typename Y, typename Z> requires(	(alignof(X)	<	alignof(Y))	&&	(alignof(Y)	>	alignof(Z))	&&	(alignof(X)	<	alignof(Z))	)
		struct Vec_Impl<X, Y, Z>
		{ Y y; Z z; X x; };
		
		template<typename X, typename Y, typename Z> requires(	(alignof(X)	>=	alignof(Y))	&&	(alignof(Y)	<	alignof(Z))	&&	(alignof(X)	<	alignof(Z))	)
		struct Vec_Impl<X, Y, Z>
		{ Z z; X x; Y y; };
		
		template<typename X, typename Y, typename Z> requires(	(alignof(X)	<	alignof(Y))	&&	(alignof(Y)	<	alignof(Z))	&&	(alignof(X)	<	alignof(Z))	)
		struct Vec_Impl<X, Y, Z>
		{ Z z; Y y; X x; };
		
		
		
		template<typename X, typename Y>
		struct Vec_Impl<X, Y, void>
		{ X x; Y y; };
		
		template<typename X, typename Z>
		struct Vec_Impl<X, void, Z>
		{ X x; Z z; };
		
		template<typename Y, typename Z>
		struct Vec_Impl<void, Y, Z>
		{ Y y; Z z; };
		
		template<> struct Vec_Impl<void, void, void>;
		template<typename X> struct Vec_Impl<X, void, void>;
		template<typename Y> struct Vec_Impl<void, Y, void>;
		template<typename Z> struct Vec_Impl<void, void, Z>;
		
		static constexpr decltype(auto)
		make_vec_impl( auto&& x, auto&& y, auto&& z) {
			Vec_Impl<std::remove_cvref_t<decltype(x)>, std::remove_cvref_t<decltype(y)>, std::remove_cvref_t<decltype(z)>>
			out{};
			out.x = x;
			out.y = y;
			out.z = z;
			return out;
		}
		
		static constexpr decltype(auto)
		make_vec_impl( auto&& x, auto&& y, std::nullptr_t z = nullptr ) {
			Vec_Impl<std::remove_cvref_t<decltype(x)>, std::remove_cvref_t<decltype(y)>, void>
			out{};
			out.x = x;
			out.y = y;
			return out;
		}
		
		static constexpr decltype(auto)
		make_vec_impl( auto&& x, std::nullptr_t y, auto&& z ) {
			Vec_Impl<std::remove_cvref_t<decltype(x)>, void, std::remove_cvref_t<decltype(z)>>
			out{};
			out.x = x;
			out.z = z;
			return out;
		}
		
		static constexpr decltype(auto)
		make_vec_impl( std::nullptr_t x, auto&& y, auto&& z) {
			Vec_Impl<void, std::remove_cvref_t<decltype(y)>, std::remove_cvref_t<decltype(z)>>
			out{};
			out.y = y;
			out.z = z;
			return out;
		}
		
	}
	
	namespace detail {
		
		template<typename X, typename Y = X, typename Z = void>
		class Vec: public internal::Vec_Impl<X, Y, Z> {
			
			
			
		};
		
	}
	
}

#endif
