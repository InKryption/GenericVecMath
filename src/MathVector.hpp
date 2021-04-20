#ifndef INK_MATH_VECTOR_HEADER_FILE_GUARD
#define INK_MATH_VECTOR_HEADER_FILE_GUARD

#include <cstdlib>
#include <concepts>

namespace ink {
	
	namespace internal {
		
		template<typename T, typename U, typename V>
		struct PackTypes;
		
		template<typename T, typename U, typename V>
		requires(	(alignof(T) >= alignof(U))
		&&			(alignof(U) >= alignof(V))
		&&			(alignof(T) >= alignof(V)) )
		struct PackTypes<T, U, V> {
			using type = PackTypes<T, U, V>;
			using gt = T;
			using md = U;
			using ls = V;
		};
		
		template<typename T, typename U, typename V>
		requires(	(alignof(T)	>	alignof(U))
		&&			(alignof(U)	<	alignof(V))
		&&			(alignof(T)	>=	alignof(V)) )
		struct PackTypes<T, U, V> {
			using type = typename PackTypes<T, V, U>::type;
		};
		
		
		
		template<typename T, typename U, typename V>
		requires(	(alignof(T)	<	alignof(U))
		&&			(alignof(U)	>	alignof(V))
		&&			(alignof(T)	>=	alignof(V)) )
		struct PackTypes<T, U, V> {
			using type = typename PackTypes<U, T, V>::type;
		};
		
		template<typename T, typename U, typename V>
		requires(	(alignof(T)	<	alignof(U))
		&&			(alignof(U)	>=	alignof(V))
		&&			(alignof(T)	<	alignof(V)) )
		struct PackTypes<T, U, V> {
			using type = typename PackTypes<U, V, T>::type;
		};
		
		
		
		template<typename T, typename U, typename V>
		requires(	(alignof(T)	>=	alignof(U))
		&&			(alignof(U)	<	alignof(V))
		&&			(alignof(T)	<	alignof(V)) )
		struct PackTypes<T, U, V> {
			using type = typename PackTypes<V, T, U>::type;
		};
		
		template<typename T, typename U, typename V>
		requires(	(alignof(T)	<	alignof(U))
		&&			(alignof(U)	<	alignof(V))
		&&			(alignof(T)	<	alignof(V)) )
		struct PackTypes<T, U, V> {
			using type = typename PackTypes<V, U, T>::type;
		};
		
		template<typename T, typename U, typename V>
		using PackTypes_t = typename PackTypes<T, U, V>::type;
		
	}
	
	namespace detail {
		
		template<typename T, size_t>
		struct Axis;
		
		template<size_t at>
		struct Axis<void, at>
		{
			constexpr Axis( [[maybe_unused]] auto&&... ) {}
			constexpr Axis( std::nullptr_t ) {}
			constexpr Axis() {}
		};
		
		template<typename T> requires(!std::same_as<T, void>)
		struct Axis<T, 0>
		{
			
			T x;
			constexpr
			Axis( std::convertible_to<T> auto&& x ):
			x(x) {}
			
			constexpr
			Axis(  ) requires( std::is_default_constructible_v<T> ):
			x() {}
			
		};
		
		template<typename T> requires(!std::same_as<T, void>)
		struct Axis<T, 1>
		{
			
			T y;
			constexpr
			Axis( std::convertible_to<T> auto&& y ):
			y(y) {}
			
			constexpr
			Axis(  ) requires( std::is_default_constructible_v<T> ):
			y() {}
			
		};
		
		template<typename T> requires(!std::same_as<T, void>)
		struct Axis<T, 2>
		{
			
			T z;
			constexpr
			Axis( std::convertible_to<T> auto&& z ):
			z(z) {}
			
			constexpr
			Axis(  ) requires( std::is_default_constructible_v<T> ):
			z() {}
			
		};
		
		
		
		template<typename T1, typename T2, typename T3>
		struct Vec_impl;
		
		template<typename T1, size_t t1, typename T2, size_t t2, typename T3, size_t t3>
		struct Vec_impl< Axis<T1 , t1>,Axis<T2 , t2>,Axis<T3 , t3> >:
			public Axis<T1, t1>,
			public Axis<T2, t2>,
			public Axis<T3, t3>
		{
			
			protected: template<typename T> using
			ctr_arg_t = std::conditional_t< (std::same_as<T, void>), std::nullptr_t, T >;
			
			protected: template<typename T> static constexpr bool
			default_or_void = std::is_default_constructible_v<T> || std::same_as<void, T>;
			
			private: template<size_t t> static constexpr auto&&
			forward_param(auto&& x, auto&& y, auto&& z)
			{
				if constexpr(0 == t) return x;
				if constexpr(1 == t) return y;
				if constexpr(2 == t) return z;
			}
			
			public: constexpr
			Vec_impl() requires(default_or_void<T1> && default_or_void<T2> && default_or_void<T3>):
				Axis<T1, t1>(),
				Axis<T2, t2>(),
				Axis<T3, t3>()
			{}
			
			public: constexpr
			Vec_impl(
				std::convertible_to<ctr_arg_t<T1>> auto&& x,
				std::convertible_to<ctr_arg_t<T2>> auto&& y,
				std::convertible_to<ctr_arg_t<T3>> auto&& z
			):
				Axis<T1, t1>(forward_param<t1>(x,y,z)),
				Axis<T2, t2>(forward_param<t2>(x,y,z)),
				Axis<T3, t3>(forward_param<t3>(x,y,z))
			{}
			
		};
		
		template<typename X, typename Y, typename Z>
		using PackedVec = Vec_impl <
			typename internal::PackTypes_t<Axis<X, 0>, Axis<Y, 1>, Axis<Z, 2> >::gt,
			typename internal::PackTypes_t<Axis<X, 0>, Axis<Y, 1>, Axis<Z, 2>>::md,
			typename internal::PackTypes_t<Axis<X, 0>, Axis<Y, 1>, Axis<Z, 2>>::ls
		>;
		
		
		
		template<typename X, typename Y = X, typename Z = Y>
		class Vec: public PackedVec<X, Y, Z> {
			
			private: template<typename T> using
			ctr_arg_t = typename PackedVec<X, Y, Z>::ctr_arg_t<T>;
			
			public: constexpr
			Vec() requires(std::is_default_constructible_v<PackedVec<X, Y, Z>>):
			PackedVec<X, Y, Z>()
			{}
			
			public: constexpr
			Vec(std::convertible_to<ctr_arg_t<X>> auto&& x,
				std::convertible_to<ctr_arg_t<Y>> auto&& y,
				std::convertible_to<ctr_arg_t<Z>> auto&& z):
			PackedVec<X, Y, Z>(x, y, z)
			{}
			
			public: constexpr
			Vec(std::convertible_to<ctr_arg_t<X>> auto&& x,
				std::convertible_to<ctr_arg_t<Y>> auto&& y)
				requires(std::same_as<Z, void>):
			PackedVec<X, Y, Z>(x, y, nullptr)
			{}
			
			public: constexpr
			Vec(std::convertible_to<ctr_arg_t<X>> auto&& x,
				std::convertible_to<ctr_arg_t<Z>> auto&& z)
				requires(std::same_as<Y, void>):
			PackedVec<X, Y, Z>(x, nullptr, z)
			{}
			
			public: constexpr
			Vec(std::convertible_to<ctr_arg_t<Y>> auto&& y,
				std::convertible_to<ctr_arg_t<Z>> auto&& z)
				requires(std::same_as<X, void>):
			PackedVec<X, Y, Z>(nullptr, y, z)
			{}
			
		};
		
	}
	
	using detail::Vec;
	
}

#endif
