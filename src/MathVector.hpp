#ifndef INK_MATH_VECTOR_HEADER_FILE_GUARD
#define INK_MATH_VECTOR_HEADER_FILE_GUARD

#include <cstdlib>
#include <concepts>

namespace ink {
	
	namespace internal {
		
		template<typename... T>
		struct CheckAligned {
			static constexpr bool value
			=	true;	};
		
		template<typename T1, typename T2, typename... T>
		struct CheckAligned<T1, T2, T...> {
			static constexpr bool value
			=	(alignof(T1) >= alignof(T2))
			&&	CheckAligned<T2, T...>::value;	};
		
		template<typename T1, typename T2>
		struct CheckAligned<T1, T2> {
			static constexpr bool value
			=	(alignof(T1) >= alignof(T2));	};
		
		
		
		template<typename... T>
		struct AlignTypes;
		
		template<typename... T> requires(CheckAligned<T...>::value)
		struct AlignTypes<T...> {
			using type = AlignTypes;
			template<template<typename...> typename Template>
			using rebind = Template<T...>;
		};
		
		template<typename T1, typename... T>
		struct AlignTypes<T1, AlignTypes<T...>>
		{ using type = typename AlignTypes<T1, T...>::type; };
		
		template<typename T1, typename T2, typename... T>
		requires( !CheckAligned<T1, T2, T...>::value )
		struct AlignTypes<T1, T2, T...>
		{
			using _0 = std::conditional_t<(alignof(T1) >= alignof(T2)), T1, T2>;
			using _1 = std::conditional_t<(alignof(T1) >= alignof(T2)), T2, T1>;
			using type = typename AlignTypes<
				_0 , typename AlignTypes<_1, T...>::type
			>:: type;
		};
		
		template<typename... T>
		using AlignTypes_t = typename AlignTypes<T...>::type;
		
	}
	
	namespace detail {
		
		template<typename T, size_t axis_index>
		struct Axis;
		
		template<size_t axis_index>
		struct Axis<void, axis_index>
		{
			
			constexpr
			Axis(std::nullptr_t = nullptr)
			noexcept {}
			
		};
		
		template<typename T> requires(!std::same_as<T, void>)
		struct Axis<T, 0>
		{
			constexpr
			Axis()
			noexcept( noexcept(T()) ):
			x() {}
			
			constexpr
			Axis(auto&& x)
			noexcept( noexcept(T( std::declval<T>() )) ):
			x(x) {}
			
			T x;
		};
		
		template<typename T> requires(!std::same_as<T, void>)
		struct Axis<T, 1>
		{
			
			constexpr
			Axis()
			noexcept( noexcept(T()) ):
			y() {}
			
			constexpr
			Axis(auto&& y)
			noexcept( noexcept(T( std::declval<T>() )) ):
			y(y) {}
			
			T y;
			
		};
		
		template<typename T> requires(!std::same_as<T, void>)
		struct Axis<T, 2>
		{
			
			constexpr
			Axis()
			noexcept( noexcept(T()) ):
			z() {}
			
			constexpr
			Axis(auto&& z)
			noexcept( noexcept(T( std::declval<T>() )) ):
			z(z) {}
			
			T z;
			
		};
		
		
		
		template<typename Axis1, typename Axis2, typename Axis3>
		struct VecBase;
		
		template<
			typename T_A1, size_t I_A1,
			typename T_A2, size_t I_A2,
			typename T_A3, size_t I_A3
		> struct VecBase<
			Axis<T_A1, I_A1>,
			Axis<T_A2, I_A2>,
			Axis<T_A3, I_A3>
		>: public Axis<T_A1, I_A1>, Axis<T_A2, I_A2>, Axis<T_A3, I_A3>
		{
			
			private: using
			A1 = Axis<T_A1, I_A1>;
			private: using
			A2 = Axis<T_A2, I_A2>;
			private: using
			A3 = Axis<T_A3, I_A3>;
			
			private: template<size_t Idx> static constexpr auto&&
			forward_param(auto&& x, auto&& y, auto&& z) noexcept {
				if constexpr(Idx == 0) return x;
				if constexpr(Idx == 1) return y;
				if constexpr(Idx == 2) return z;
			}
			
			public: constexpr
			VecBase()
				noexcept(noexcept(A1())					&& noexcept(A2())					&& noexcept(A3())					)
				requires(std::default_initializable<A1>	&& std::default_initializable<A2>	&& std::default_initializable<A3>	):
				A1(), A2(), A3()
			{}
			
			public: constexpr
			VecBase(auto&& x, auto&& y, auto&& z)
				noexcept(noexcept( A1(std::declval<A1>()) )	&&	noexcept( A2(std::declval<A2>()) )	&&	noexcept( A3(std::declval<A3>() ))):
				A1(forward_param<I_A1>(x, y, z)),
				A2(forward_param<I_A2>(x, y, z)),
				A3(forward_param<I_A3>(x, y, z))
			{}
			
		};
		
		template<typename X, typename Y, typename Z>
		using PackedVecBase
		=	typename internal
		::	AlignTypes_t<	Axis<X,0> , Axis<Y,1> , Axis<Z,2>	>
		::	rebind< VecBase >;
		
		template<typename X, typename Y = X, typename Z = Y>
		class Vec: public PackedVecBase<X, Y, Z>
		{
			
			private: using
			base = PackedVecBase<X, Y, Z>;
			
			private: static constexpr bool
			void_X = std::same_as<void, X>,
			void_Y = std::same_as<void, Y>,
			void_Z = std::same_as<void, Z>;
			
			public: constexpr
			Vec() noexcept(noexcept(base()))
			requires(std::default_initializable<base>):
			base() {}
			
			public: constexpr
			Vec(std::convertible_to<X> auto&& x,
				std::convertible_to<Y> auto&& y,
				std::convertible_to<Z> auto&& z)
				noexcept( noexcept(base( std::declval<X>() , std::declval<Y>() , std::declval<Z>() )) )
				requires(!void_X && !void_Y && !void_Z):
				base(x, y, z)
			{}
			
			public: constexpr
			Vec(std::convertible_to<X> auto&& x,
				std::convertible_to<Y> auto&& y)
				noexcept( noexcept(base( std::declval<X>() , std::declval<Y>() , nullptr )) )
				requires(!void_X && !void_Y && void_Z):
				base(x, y, nullptr)
			{}
			
			public: constexpr
			Vec(std::convertible_to<X> auto&& x,
				std::convertible_to<Z> auto&& z)
				noexcept( noexcept(base( std::declval<X>() , nullptr , std::declval<Z>() )) )
				requires(!void_X && void_Y && !void_Z):
				base(x, nullptr, z)
			{}
			
			public: constexpr
			Vec(std::convertible_to<Y> auto&& y,
				std::convertible_to<Z> auto&& z)
				noexcept( noexcept(base( nullptr , std::declval<Y>() , std::declval<Z>() )) )
				requires(void_X && !void_Y && !void_Z):
				base(nullptr, y, z)
			{}
			
			public: constexpr
			Vec(std::convertible_to<X> auto&& x)
				noexcept( noexcept(base( std::declval<X>() , nullptr , nullptr )) )
				requires(!void_X && void_Y && void_Z):
				base(x, nullptr, nullptr)
			{}
			
			public: constexpr
			Vec(std::convertible_to<Y> auto&& y)
				noexcept( noexcept(base( nullptr , std::declval<Y>() , nullptr )) )
				requires(void_X && !void_Y && void_Z):
				base(nullptr, y, nullptr)
			{}
			
			public: constexpr
			Vec(std::convertible_to<Z> auto&& z)
				noexcept( noexcept(base( nullptr , nullptr , std::declval<Z>() )) )
				requires(void_X && void_Y && !void_Z):
				base(nullptr, nullptr, z)
			{}
			
		};
		
	}
	
	using detail::Vec;
	
	template<typename X, typename Y = X>
	using Vec2 = Vec<X, Y, void>;
	
}

#endif
