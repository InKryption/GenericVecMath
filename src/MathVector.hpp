#ifndef INK_MATH_VECTOR_HEADER_FILE_GUARD
#define INK_MATH_VECTOR_HEADER_FILE_GUARD

#include <cstdlib>
#include <concepts>

#include <memory>

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

		enum class XYZ { X , Y , Z };
		template<typename T,
						XYZ tag>
								struct AxisBase;
		
		template<non_void T>	struct AxisBase<T, XYZ::X> { using ctr_arg = T; T x; };
		template<non_void T>	struct AxisBase<T, XYZ::Y> { using ctr_arg = T; T y; };
		template<non_void T>	struct AxisBase<T, XYZ::Z> { using ctr_arg = T; T z; };
		
		template<XYZ tag>		struct AxisBase<void, tag> {
			constexpr AxisBase(std::nullptr_t = nullptr) noexcept {}
			protected:
			using value_type = void;
			using ctr_arg = std::nullptr_t;
		};
		
		
		
		template<typename T, XYZ tag>
		struct Axis:
		public AxisBase<T, tag> {
			
			template<typename, XYZ> friend struct Axis;
			
			protected: using
			base = AxisBase<T, tag>;

			protected: using
			value_type = T;

			protected: using
			ctr_arg = typename base::ctr_arg;

			private: template<typename... Arg> static constexpr bool
			NOEXCEPT_CTR = noexcept(base{std::forward<Arg>(std::declval<Arg>())...});

			protected: constexpr decltype(auto)
			data() noexcept
			{
				if constexpr(std::same_as<void, T>) return *this;
				else {
					if constexpr(tag == XYZ::X) return *std::addressof(this->x);
					if constexpr(tag == XYZ::Y) return *std::addressof(this->y);
					if constexpr(tag == XYZ::Z) return *std::addressof(this->z);
				}
			}

			protected: constexpr decltype(auto)
			data() const noexcept
			{

				if constexpr(std::same_as<void, T>) return *this;
				else {
					if constexpr(tag == XYZ::X) return *std::addressof(this->x);
					if constexpr(tag == XYZ::Y) return *std::addressof(this->y);
					if constexpr(tag == XYZ::Z) return *std::addressof(this->z);
				}

			}
			
			public: constexpr
			Axis() noexcept(NOEXCEPT_CTR<>)
			requires(std::default_initializable<base>):
			base{} {}

			public: constexpr
			Axis(ctr_arg& v) noexcept(NOEXCEPT_CTR<ctr_arg&>)
			requires(std::constructible_from<base, T&>):
			base{std::forward<T&>(v)} {}

			public: constexpr
			Axis(ctr_arg&& v) noexcept(NOEXCEPT_CTR<ctr_arg&&>)
			requires(std::constructible_from<base, T&&>):
			base{std::forward<T&&>(v)} {}

			public: constexpr
			Axis(ctr_arg const& v) noexcept(NOEXCEPT_CTR<ctr_arg const&>)
			requires(std::constructible_from<base, ctr_arg const&>):
			base{std::forward<ctr_arg const&>(v)} {}
			
			private: static constexpr bool
			is_void = std::same_as<void, T>;
			
			public: template<typename U> constexpr decltype(auto)
			operator+(Axis<U, tag> const& rhs) const
			{
				if constexpr(is_void) return Axis<U, tag>( rhs.data() );
				else return Axis<decltype(data() + rhs.data()), tag>( data() + rhs.data() );
			}
			
			public: template<typename U> constexpr decltype(auto)
			operator-(Axis<U, tag> const& rhs) const
			{
				if constexpr(is_void) return Axis<U, tag>( rhs.data() );
				else return Axis<decltype(data() - rhs.data()), tag>( data() - rhs.data() );
			}
			
			public: template<typename U> constexpr decltype(auto)
			operator*(Axis<U, tag> const& rhs) const
			{
				if constexpr(is_void) return Axis<U, tag>( rhs.data() );
				else return Axis<decltype(data() * rhs.data()), tag>( data() * rhs.data() );
			}
			
			public: template<typename U> constexpr decltype(auto)
			operator/(Axis<U, tag> const& rhs) const
			{
				if constexpr(is_void) return Axis<U, tag>( rhs.data() );
				else return Axis<decltype(data() / rhs.data()), tag>( data() / rhs.data() );
			}
			
		};
		
		template<typename T> using XAxis = Axis<T, XYZ::X>;
		template<typename T> using YAxis = Axis<T, XYZ::Y>;
		template<typename T> using ZAxis = Axis<T, XYZ::Z>;

		template<size_t Which, typename X, typename Y, typename Z>
		using AxisByAlignment = internal::AlignTypes_t<XAxis<X>, YAxis<Y>, ZAxis<Z>>::template data<Which>;

	}

	template<typename X, typename Y = void, typename Z = void>
	class Vec:
	public detail::AxisByAlignment<0, X, Y, Z>,
	public detail::AxisByAlignment<1, X, Y, Z>,
	public detail::AxisByAlignment<2, X, Y, Z>
	{
		private: using XYZ = detail::XYZ;

		public: using value_type_x = X;
		public: using value_type_y = Y;
		public: using value_type_z = Z;

		private: template<size_t Idx>
		using base_bo = detail::AxisByAlignment<Idx, X, Y, Z>;

		private: template<detail::XYZ tag>
		using base_bt = internal::
		Switch_t<tag,
			typename internal::Switch<>::	Case<(XYZ::X),	detail::Axis<X, XYZ::X> > ,
			typename internal::Switch<>::	Case<(XYZ::Y),	detail::Axis<Y, XYZ::Y> > ,
			typename internal::Switch<>::	Case<(XYZ::Z),	detail::Axis<Z, XYZ::Z> >
		>;

		using base_X = base_bt<XYZ::X>;
		using base_Y = base_bt<XYZ::Y>;
		using base_Z = base_bt<XYZ::Z>;

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
		
		/* Default constructor */

		public: constexpr
		Vec()
				noexcept( NOEXCEPT_CTR<base_X> && NOEXCEPT_CTR<base_Y> && NOEXCEPT_CTR<base_Z> )
				requires( std::default_initializable<base_X> && std::default_initializable<base_Y> && std::default_initializable<base_Z> ):
		base_bo<0>(),
		base_bo<1>(),
		base_bo<2>() {}
		
		
		
		/* All Three Axies Are Non-Void */
		
		public: constexpr
		Vec(std::convertible_to<typename base_X::ctr_arg> auto&& x,
			std::convertible_to<typename base_Y::ctr_arg> auto&& y,
			std::convertible_to<typename base_Z::ctr_arg> auto&& z)
				noexcept( NOEXCEPT_CTR<base_X, decltype(x)> && NOEXCEPT_CTR<base_Y, decltype(y)> && NOEXCEPT_CTR<base_Z, decltype(z)> ):
		base_bo<0>( forward_param<0>(std::forward<decltype(x)>(x), std::forward<decltype(y)>(y), std::forward<decltype(z)>(z)) ),
		base_bo<1>( forward_param<1>(std::forward<decltype(x)>(x), std::forward<decltype(y)>(y), std::forward<decltype(z)>(z)) ),
		base_bo<2>( forward_param<2>(std::forward<decltype(x)>(x), std::forward<decltype(y)>(y), std::forward<decltype(z)>(z)) ) {}
		
		
		
		/* Only Two Arguments required */
		
		public: constexpr
		Vec(std::convertible_to<typename base_X::ctr_arg> auto&& x, std::convertible_to<typename base_Y::ctr_arg> auto&& y)
				noexcept( NOEXCEPT_CTR<base_X, decltype(x)> && NOEXCEPT_CTR<base_Y, decltype(y)> )
				requires( std::same_as<void, Z> || std::default_initializable<Z> ):
		base_bo<0>( forward_param<0>(std::forward<decltype(x)>(x), std::forward<decltype(y)>(y), typename base_Z::ctr_arg()) ),
		base_bo<1>( forward_param<1>(std::forward<decltype(x)>(x), std::forward<decltype(y)>(y), typename base_Z::ctr_arg()) ),
		base_bo<2>( forward_param<2>(std::forward<decltype(x)>(x), std::forward<decltype(y)>(y), typename base_Z::ctr_arg()) ) {}
		
		public: constexpr
		Vec(std::convertible_to<typename base_X::ctr_arg> auto&& x, std::convertible_to<typename base_Z::ctr_arg> auto&& z)
				noexcept( NOEXCEPT_CTR<base_X, decltype(x)> && NOEXCEPT_CTR<base_Z, decltype(z)> )
				requires( std::same_as<void, Y> ):
		base_bo<0>( forward_param<0>(std::forward<decltype(x)>(x), typename base_Y::ctr_arg(), std::forward<decltype(z)>(z)) ),
		base_bo<1>( forward_param<1>(std::forward<decltype(x)>(x), typename base_Y::ctr_arg(), std::forward<decltype(z)>(z)) ),
		base_bo<2>( forward_param<2>(std::forward<decltype(x)>(x), typename base_Y::ctr_arg(), std::forward<decltype(z)>(z)) ) {}
		
		public: constexpr
		Vec(std::convertible_to<typename base_Y::ctr_arg> auto&& y, std::convertible_to<typename base_Z::ctr_arg> auto&& z)
				noexcept( NOEXCEPT_CTR<base_Y, decltype(y)> && NOEXCEPT_CTR<base_Z, decltype(z)> )
				requires( std::same_as<void, X> ):
		base_bo<0>( forward_param<0>(typename base_X::ctr_arg(), std::forward<decltype(y)>(y), std::forward<decltype(z)>(z)) ),
		base_bo<1>( forward_param<1>(typename base_X::ctr_arg(), std::forward<decltype(y)>(y), std::forward<decltype(z)>(z)) ),
		base_bo<2>( forward_param<2>(typename base_X::ctr_arg(), std::forward<decltype(y)>(y), std::forward<decltype(z)>(z)) ) {}
		
		/* Only One Argument required */
		
		public: constexpr
		Vec(std::convertible_to<typename base_X::ctr_arg> auto&& x)
				noexcept( NOEXCEPT_CTR<base_X, decltype(x)> )
				requires( (std::same_as<void, Y> || std::default_initializable<base_Y>) && (std::same_as<void, Z> || std::default_initializable<base_Z>) ):
		base_X( std::forward<decltype(x)>(x) ) {}
		
		public: constexpr
		Vec(std::convertible_to<typename base_Y::ctr_arg> auto&& y)
				noexcept( NOEXCEPT_CTR<base_Y, decltype(y)> )
				requires( std::same_as<void, X> && (std::same_as<void, Z> || std::default_initializable<base_Z>) ):
		base_Y( std::forward<decltype(y)>(y) ) {}
		
		public: constexpr
		Vec(std::convertible_to<typename base_Z::ctr_arg> auto&& z)
				noexcept( NOEXCEPT_CTR<base_Z, decltype(z)> )
				requires( std::same_as<void, X> && std::same_as<void, Y> ):
		base_Z( std::forward<decltype(z)>(z) ) {}
		
		
		
		/* get<>() method */
		
		private: template<size_t Idx> requires(Idx < 3) constexpr decltype(auto)
		get_impl() noexcept {
			if constexpr(Idx == 0) return this->base_X::data();
			if constexpr(Idx == 1) return this->base_Y::data();
			if constexpr(Idx == 2) return this->base_Z::data();
		}
		
		private: template<size_t Idx> requires(Idx < 3) constexpr decltype(auto)
		get_impl() const noexcept {
			if constexpr(Idx == 0) return this->base_X::data();
			if constexpr(Idx == 1) return this->base_Y::data();
			if constexpr(Idx == 2) return this->base_Z::data();
		}
		
		public: template<size_t Idx> friend constexpr decltype(auto)
		get(Vec & v) noexcept
		{ return v.template get_impl<Idx>(); }
		
		public: template<size_t Idx> friend constexpr decltype(auto)
		get(Vec && v) noexcept
		{ return v.template get_impl<Idx>(); }
		
		public: template<size_t Idx> friend constexpr decltype(auto)
		get(Vec const& v) noexcept
		{ return v.template get_impl<Idx>(); }
		
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
