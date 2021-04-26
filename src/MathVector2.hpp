#ifndef INK_MATH_VECTOR_HEADER_FILE_GUARD
#define INK_MATH_VECTOR_HEADER_FILE_GUARD

#include <cstdint>

namespace ink::vec {
	
	
	namespace internal {
		
		template<typename... T>
		struct CheckAligned
		{ static constexpr bool value = true; };
		
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
			template<size_t ElmntIdx>
			using data = std::tuple_element_t<ElmntIdx, std::tuple<T...>>;
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
		
		
		
		template<auto... CMP>
		struct Switch;
		
		template<>
		struct Switch<> {
			template<auto V, typename T> struct Case {};
		};
		
		/* A template construct that serves a similar purpose to std::conditional_t, utilizing a very different format */
		template<auto CMP>
		struct Switch<CMP> {
			
			using cmp_t = decltype(CMP);
			
			template<cmp_t V, typename T>
			using Case = Switch<>::Case<V,T>;
			
			template<typename... CASES>
			struct In_impl;
			
			template<cmp_t V1, typename T1, cmp_t... Vs, typename... Ts>
			requires( V1 != CMP )
			struct In_impl<Case<V1, T1>, Case<Vs, Ts>...>
			{ using type = typename In_impl<Case<Vs, Ts>...>::type; };
			
			template<cmp_t V1, typename T1, cmp_t... Vs, typename... Ts>
			requires( V1 == CMP )
			struct In_impl<Case<V1, T1>, Case<Vs, Ts>...>
			{ using type = T1; };
			
			template<typename... CASES>
			using In = typename In_impl<CASES...>::type;
			
		};
		
		template<auto CMP, typename... cases>
		using Switch_t = Switch<CMP>::template In<cases...>;
		
	}
	
	
	enum class XYZ: uint8_t
	{ X , Y , Z };
	
	template<typename T, XYZ tag>
	struct Axis {
		
		public: constexpr
		Axis() noexcept requires(std::default_initializable<T>)
		: M_data() {}
		
		public: constexpr
		Axis(std::convertible_to<T> auto&& v) noexcept( requires(decltype(v) v) {	{ T(std::forward<T>(v)) } noexcept;	} )
		: M_data(std::forward<decltype(v)>(v)) {}
		
		protected: constexpr auto&
		get() noexcept
		{ return M_data; }
		
		protected: constexpr auto&
		get() const noexcept
		{ return M_data; }
		
		private: T
		M_data;
	};
	
	template<typename T, XYZ tag>
	struct Axis {
		
		public: constexpr
		Axis(auto&& v = nullptr)
		noexcept
		{}
		
		protected: constexpr void
		get() const noexcept
		{}
		
	};
	
	namespace detail {
		
		template<typename X, typename Y, typename Z>
		class VecBase:	protected internal::AlignTypes_t<Axis<X, XYZ::X>, Axis<Y, XYZ::Y>, Axis<Z, XYZ::Z>>::template data<0>,
						protected internal::AlignTypes_t<Axis<X, XYZ::X>, Axis<Y, XYZ::Y>, Axis<Z, XYZ::Z>>::template data<1>,
						protected internal::AlignTypes_t<Axis<X, XYZ::X>, Axis<Y, XYZ::Y>, Axis<Z, XYZ::Z>>::template data<2>
		{
			private:
			template<size_t idx>
			using base_i = internal::AlignTypes_t<Axis<X, XYZ::X>, Axis<Y, XYZ::Y>, Axis<Z, XYZ::Z>>::template data<idx>;
			using base_X = Axis<X, XYZ::X>;
			using base_Y = Axis<Y, XYZ::Y>;
			using base_Z = Axis<Z, XYZ::Z>;
			
			template<size_t idx> static constexpr auto&&
			fwd_arg(auto&& x, auto&& y, auto&& z) noexcept {
				if constexpr( std::same_as<base_i<idx> , base_X> ) return x;
				if constexpr( std::same_as<base_i<idx> , base_Y> ) return y;
				if constexpr( std::same_as<base_i<idx> , base_Z> ) return z;
			}
			
			public: constexpr
			VecBase( std::convertible_to<base_X> auto&& x , std::convertible_to<base_Y> auto&& y , std::convertible_to<base_X> auto&& z ):
				base_i<0>( fwd_arg( x , y , z ) ),
				base_i<1>( fwd_arg( x , y , z ) ),
				base_i<2>( fwd_arg( x , y , z ) )
			{}
			
		};
	}
	
}

#endif
