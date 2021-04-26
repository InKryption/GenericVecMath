#ifndef INK_MATH_VECTOR_HEADER_FILE_GUARD
#define INK_MATH_VECTOR_HEADER_FILE_GUARD

#include <cstdlib>
#include <concepts>

namespace ink {

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
	
	template<typename X, typename Y, typename Z> class Vec;
	
	namespace detail {
		
		template<typename T>
		concept non_void = (!std::same_as<void, T>);
		
		enum class XYZ { X = 1 , Y = 2, Z = 4 };
		template<typename T, XYZ tag>
		struct AxisBase;
		
		template<non_void T>	struct AxisBase<T, XYZ::X> { T x; };
		template<non_void T>	struct AxisBase<T, XYZ::Y> { T y; };
		template<non_void T>	struct AxisBase<T, XYZ::Z> { T z; };
		template<XYZ tag>		struct AxisBase<void, tag> { constexpr AxisBase(std::nullptr_t = nullptr) noexcept {} };
		
		template<typename T, XYZ tag>
		struct Axis: public AxisBase<T, tag> {
			
			template<typename, XYZ> friend struct Axis;
			
			protected: using
			base = AxisBase<T, tag>;
			
			public: using
			value_type = T;
			
			protected: using
			ctr_arg = std::conditional_t<std::same_as<T, void>, std::nullptr_t, T>;
			
			private: template<typename... Arg> static constexpr bool
			NOEXCEPT_CTR = noexcept(base{std::forward<Arg>(std::declval<Arg>())...});
			
			protected: constexpr decltype(auto)
			data() noexcept
			{
				if constexpr(std::same_as<void, T>) return;
				else {
					if constexpr(tag == XYZ::X) return *std::addressof(this->x);
					if constexpr(tag == XYZ::Y) return *std::addressof(this->y);
					if constexpr(tag == XYZ::Z) return *std::addressof(this->z);
				}
			}
			
			protected: constexpr decltype(auto)
			data() const noexcept
			{

				if constexpr(std::same_as<void, T>) return;
				else {
					if constexpr(tag == XYZ::X) return *std::addressof(this->x);
					if constexpr(tag == XYZ::Y) return *std::addressof(this->y);
					if constexpr(tag == XYZ::Z) return *std::addressof(this->z);
				}

			}
			
			public: constexpr
			Axis() noexcept(NOEXCEPT_CTR<>)
			requires(std::default_initializable<T>):
			base{} {}

			public: constexpr
			Axis(std::convertible_to<ctr_arg> auto&& v) noexcept(noexcept( base{std::forward<decltype(v)>(v)} ))
			: base{std::forward<decltype(v)>(v)} {}
			
			protected: static constexpr bool
			is_void = std::same_as<void, T>;
			
		};
		
		template<size_t Which, typename X, typename Y, typename Z>
		using AxisByAlignment = internal::AlignTypes_t<Axis<X, XYZ::X>, Axis<Y, XYZ::Y>, Axis<Z, XYZ::Z>>::template data<Which>;
		
		template<typename X, typename Y, typename Z>
		struct VecBase:
			public AxisByAlignment<0, X, Y, Z>,
			public AxisByAlignment<1, X, Y, Z>,
			public AxisByAlignment<2, X, Y, Z>
		{
			
			protected: using XYZ = detail::XYZ;
			
			private: template<size_t Idx>
			using base_bo = detail::AxisByAlignment<Idx, X, Y, Z>;
			
			protected: template<XYZ tag>
			using base = internal::
			Switch_t<tag,
				typename internal::Switch<>::	Case<(XYZ::X),	detail::Axis<X, XYZ::X> > ,
				typename internal::Switch<>::	Case<(XYZ::Y),	detail::Axis<Y, XYZ::Y> > ,
				typename internal::Switch<>::	Case<(XYZ::Z),	detail::Axis<Z, XYZ::Z> >
			>;
			
			using base_X = base<XYZ::X>;
			using base_Y = base<XYZ::Y>;
			using base_Z = base<XYZ::Z>;
			
			public: template<XYZ tag>
			using value_type = typename base<tag>::value_type;
			
			private: template<typename _base, typename... T> static constexpr bool
			NOEXCEPT_CTR = noexcept(_base(std::forward<T>(std::declval<T>())...));
			
			private: template<size_t Idx> static constexpr auto&&
			forward_param(auto&& x, auto&& y, auto&& z) noexcept {
				if constexpr(std::same_as<base_bo<Idx>, base_X>)
					return x;
				if constexpr(std::same_as<base_bo<Idx>, base_Y>)
					return y;
				if constexpr(std::same_as<base_bo<Idx>, base_Z>)
					return z;
			}
			
			public: constexpr
			VecBase(
				std::convertible_to<typename base_X::ctr_arg> auto&& x,
				std::convertible_to<typename base_Y::ctr_arg> auto&& y,
				std::convertible_to<typename base_Z::ctr_arg> auto&& z)
					noexcept( noexcept(base_X(x)) && noexcept(base_Y(y)) && noexcept(base_Z(z)) ):
			base_bo<0>( forward_param<0>( x , y , z ) ),
			base_bo<1>( forward_param<1>( x , y , z ) ),
			base_bo<2>( forward_param<2>( x , y , z ) ) {}
			
		};
		
	}

	template<typename X, typename Y = void, typename Z = void>
	class Vec: public detail::VecBase<X, Y, Z>
	{
		
		private: using
		base = detail::VecBase<X, Y, Z>;
		
		private: using axis_X = typename base::base_X;
		private: using axis_Y = typename base::base_Y;
		private: using axis_Z = typename base::base_Z;
		
		private: using ctr_X = typename axis_X::ctr_arg;
		private: using ctr_Y = typename axis_Y::ctr_arg;
		private: using ctr_Z = typename axis_Z::ctr_arg;
		
		
		
		/* Default Constructor */
		
		public: constexpr
		Vec()
		noexcept( noexcept(base( ctr_X() , ctr_Y() , ctr_Z() )) )
		requires( std::default_initializable<axis_X> && std::default_initializable<axis_Y> && std::default_initializable<axis_Z> )
		: base( ctr_X(), ctr_Y(), ctr_Z() ) {}
		
		
		
		/* Standard & Deducible Constructors */
		
		public: constexpr
		Vec(std::convertible_to<ctr_X> auto && x,
			std::convertible_to<ctr_Y> auto && y,
			std::convertible_to<ctr_Z> auto && z)
		noexcept(noexcept( base( x , y , z ) ))
		: base( x , y , z ) {}
		
		public: constexpr
		Vec(std::convertible_to<ctr_X> auto&& x,
			std::convertible_to<ctr_Y> auto&& y)
		noexcept(noexcept( base( x , y , ctr_Z() ) ))
		requires( std::default_initializable<axis_Z> || axis_Z::is_void )
		: base( x , y , ctr_Z() ) {}
		
		public: constexpr
		Vec(std::convertible_to<ctr_X> auto&& x)
		noexcept(noexcept( base( x , ctr_Y(), ctr_Z() ) ))
		requires( (std::default_initializable<axis_Y> || axis_Y::is_void) && (std::default_initializable<axis_Z> || axis_Z::is_void) )
		: base( x , ctr_Y() , ctr_Z() ) {}
		
		
		
		/* Conditional Constructors */
		
		public: constexpr
		Vec(std::convertible_to<ctr_Y> auto&& y,
			std::convertible_to<ctr_Z> auto&& z)
		noexcept(noexcept( base( ctr_X() , y , z ) ))
		requires( axis_X::is_void )
		: base( ctr_X() , y , z ) {}
		
		public: constexpr
		Vec(std::convertible_to<ctr_X> auto&& x,
			std::convertible_to<ctr_Z> auto&& z)
		noexcept(noexcept( base( x , ctr_Y() , z) ))
		requires( axis_Y::is_void )
		: base( x , ctr_Y() , z ) {}
		
		#if defined(__GNUC__)
		#define ATTRIBUTE_UNUSED __attribute__((unused))
		#else
		#define ATTRIBUTE_UNUSED
		#endif
		
		#define DefBinaryOpFriend(op, lhs_spec, rhs_spec)											\
		public: template<typename RX, typename RY, typename RZ>										\
		friend constexpr decltype(auto)																\
		operator op (Vec lhs_spec lhs, Vec<RX, RY, RZ> rhs_spec rhs) noexcept(						\
				noexcept( requires(Vec lhs, Vec<RX, RY, RZ> rhs) { { lhs.x op rhs.x } noexcept; } || std::same_as<void, X> || std::same_as<void, RX>)	\
			&&	noexcept( requires(Vec lhs, Vec<RX, RY, RZ> rhs) { { lhs.y op rhs.y } noexcept; } || std::same_as<void, Y> || std::same_as<void, RY>)	\
			&&	noexcept( requires(Vec lhs, Vec<RX, RY, RZ> rhs) { { lhs.z op rhs.z } noexcept; } || std::same_as<void, Z> || std::same_as<void, RZ>)	\
		) requires (																				\
				(requires(X lhs_spec lhs, RX rhs_spec rhs) { {lhs op rhs}; } || std::same_as<void, X> || std::same_as<void, RX> )						\
			&&	(requires(Y lhs_spec lhs, RY rhs_spec rhs) { {lhs op rhs}; } || std::same_as<void, Y> || std::same_as<void, RY> )						\
			&&	(requires(Z lhs_spec lhs, RZ rhs_spec rhs) { {lhs op rhs}; } || std::same_as<void, Z> || std::same_as<void, RZ> )						\
			&&	!std::same_as<void, X> && !std::same_as<void, Y> && !std::same_as<void, X>)			\
		{																							\
			constexpr bool NOEXCEPT_FUNC = noexcept(												\
					noexcept( requires(Vec lhs, Vec<RX, RY, RZ> rhs) { { lhs.x op rhs.x } noexcept; } || std::same_as<void, X> || std::same_as<void, RX>)	\
				&&	noexcept( requires(Vec lhs, Vec<RX, RY, RZ> rhs) { { lhs.y op rhs.y } noexcept; } || std::same_as<void, Y> || std::same_as<void, RY>)	\
				&&	noexcept( requires(Vec lhs, Vec<RX, RY, RZ> rhs) { { lhs.z op rhs.z } noexcept; } || std::same_as<void, Z> || std::same_as<void, RZ>));	\
			struct Empty { constexpr operator std::nullptr_t() noexcept { return nullptr; } };		\
			constexpr Empty empty;																	\
			decltype(auto) x_out = [&]() constexpr noexcept(NOEXCEPT_FUNC) -> decltype(auto) {		\
				if constexpr( std::same_as<X, void> &&  std::same_as<RX, void>)	return empty;		\
				if constexpr(!std::same_as<X, void> &&  std::same_as<RX, void>)	return *&lhs.x;		\
				if constexpr( std::same_as<X, void> && !std::same_as<RX, void>)	return *&rhs.x;		\
				if constexpr(!std::same_as<X, void> && !std::same_as<RX, void>)						\
				{ return lhs.x op rhs.x; }															\
			}();																					\
			decltype(auto) y_out = [&]() constexpr noexcept(NOEXCEPT_FUNC) -> decltype(auto) {		\
				if constexpr( std::same_as<Y, void> &&  std::same_as<RY, void>)	return empty;		\
				if constexpr(!std::same_as<Y, void> &&  std::same_as<RY, void>)	return lhs.y;		\
				if constexpr( std::same_as<Y, void> && !std::same_as<RY, void>)	return rhs.y;		\
				if constexpr(!std::same_as<Y, void> && !std::same_as<RY, void>)						\
				{ return lhs.y op rhs.y; }															\
			}();																					\
			decltype(auto) z_out = [&]() constexpr noexcept(NOEXCEPT_FUNC) -> decltype(auto) {		\
				if constexpr( std::same_as<Z, void> &&  std::same_as<RZ, void>)	return empty;		\
				if constexpr(!std::same_as<Z, void> &&  std::same_as<RZ, void>)	return lhs.z;		\
				if constexpr( std::same_as<Z, void> && !std::same_as<RZ, void>)	return rhs.z;		\
				if constexpr(!std::same_as<Z, void> && !std::same_as<RZ, void>)						\
				{ return lhs.z op rhs.z; }															\
			}();																					\
																									\
			return Vec<																				\
			std::conditional_t<( std::same_as<void,X> &&  std::same_as<void,RX>), void,				\
			std::conditional_t<(!std::same_as<void,X> && !std::same_as<void,RX>), decltype(x_out),	\
			std::conditional_t<(std::same_as<void,X>), RX, X>>>										\
			,																						\
			std::conditional_t<( std::same_as<void,Y> &&  std::same_as<void,RY>), void,				\
			std::conditional_t<(!std::same_as<void,Y> && !std::same_as<void,RY>), decltype(y_out),	\
			std::conditional_t<(std::same_as<void,Y>), RY, Y>>>										\
			,																						\
			std::conditional_t<( std::same_as<void,Z> &&  std::same_as<void,RZ>), void,				\
			std::conditional_t<(!std::same_as<void,Z> && !std::same_as<void,RZ>), decltype(z_out),	\
			std::conditional_t<(std::same_as<void,Z>), RZ, Z>>>										\
			>( x_out , y_out , z_out );																\
		}
		
		#define DEFINE_BINARY_OP_VARIANTS(op)				\
			DefBinaryOpFriend(op,	&,			&		)	\
			DefBinaryOpFriend(op,	&,			&&		)	\
			DefBinaryOpFriend(op,	&,			const&	)	\
			DefBinaryOpFriend(op,	&&,			&		)	\
			DefBinaryOpFriend(op,	&&,			&&		)	\
			DefBinaryOpFriend(op,	&&,			const&	)	\
			DefBinaryOpFriend(op,	const&,		&		)	\
			DefBinaryOpFriend(op,	const&,		&&		)	\
			DefBinaryOpFriend(op,	const&,		const&	)	\
		
		#define BinaryOps(m) m(+) m(-) m(*) m(/) m(%) m(&) m(|) m(^) m(&&) m(||) m(==) m(!=) m(<) m(>) m(<=) m(>=)
		BinaryOps(DEFINE_BINARY_OP_VARIANTS)
		
		#undef BinaryOps
		#undef DEFINE_BINARY_OP_VARIANTS
		#undef DefBinaryOpFriend
		
	};
	
	/* Template Deduction Guides */
	
	template<typename X, typename Y> Vec(X, Y) -> Vec<X, Y, void>;
	template<typename X, typename Y, typename Z> Vec(X, Y, Z) -> Vec<X, Y, Z>;
	
	template<typename Y, typename Z> Vec(std::nullptr_t, Y, Z) -> Vec<void, Y, Z>;
	template<typename X, typename Z> Vec(X, std::nullptr_t, Z) -> Vec<X, void, Z>;
	template<typename X, typename Y> Vec(X, Y, std::nullptr_t) -> Vec<X, Y, void>;
	
	template<typename X> Vec(X) -> Vec<X, void, void>;
	template<typename X> Vec(X, std::nullptr_t, std::nullptr_t) -> Vec<X, void, void>;
	
	template<typename Y> Vec(std::nullptr_t, Y) -> Vec<void, Y>;
	template<typename Y> Vec(std::nullptr_t, Y, std::nullptr_t) -> Vec<void, Y, void>;
	
	template<typename Z> Vec(std::nullptr_t, std::nullptr_t, Z) -> Vec<void, void, Z>;
	
}

template<typename... T>
struct std::tuple_size<ink::Vec<T...>>
: std::integral_constant<std::size_t, sizeof...(T)> {};

template<typename X, typename Y, typename Z>
struct std::tuple_element<0, ink::Vec<X, Y, Z>>
{ using type = std::conditional_t< !(std::same_as<void,X>), X, ink::detail::Axis<X, ink::detail::XYZ::X>>; };

template<typename X, typename Y, typename Z>
struct std::tuple_element<1, ink::Vec<X, Y, Z>>
{ using type = std::conditional_t< !(std::same_as<void,Y>), Y, ink::detail::Axis<Y, ink::detail::XYZ::Y>>; };

template<typename X, typename Y, typename Z>
struct std::tuple_element<2, ink::Vec<X, Y, Z>>
{ using type = std::conditional_t< !(std::same_as<void,Z>), Z, ink::detail::Axis<Z, ink::detail::XYZ::Z>>; };

std::tuple_element_t<0, ink::Vec<int, char, bool>> f;

#endif
