#ifndef INK_MATH_VECTOR_HEADER_FILE_GUARD
#define INK_MATH_VECTOR_HEADER_FILE_GUARD

#include <cstdlib>
#include <utility>
#include <tuple>
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
		
		// This is a utility construct; it can be operated on with anything, but always produce a noop,
		// either returning the other operated variable reference, or itself in the case of unary operations.
		// This applies as well to operations involving comparisons. 
		// Note: can be constructed from a nullptr.
		// It's an empty struch, therefore should be fine to decorate this with [[no_unique_address]].
		struct Noop {
			
			constexpr Noop() noexcept {}
			constexpr Noop(std::nullptr_t) noexcept {}
			
			#define BinaryOp(op)	\
			friend constexpr decltype(auto) operator op(Noop, auto&& rhs) noexcept { return rhs; }	\
			friend constexpr decltype(auto) operator op(auto&& lhs, Noop) noexcept { return lhs; }	\
			friend constexpr decltype(auto) operator op(Noop, Noop) noexcept { return Noop(); }
			BinaryOp(+)	BinaryOp(-)
			BinaryOp(*)	BinaryOp(/)	BinaryOp(%)
			BinaryOp(&)	BinaryOp(|)	BinaryOp(^)
			BinaryOp(<<)
			BinaryOp(>>)
			#undef BinaryOp
			
			#define UnaryOp(op)	\
			friend constexpr decltype(auto) operator op(Noop noop) noexcept { return noop; }
			UnaryOp(+)	UnaryOp(-)
			UnaryOp(*)	UnaryOp(&)
			UnaryOp(!)	UnaryOp(~)
			#undef UnaryOp
			
			#define BoolOp(op)	\
			template<typename T> requires(!std::convertible_to<T, Noop const&>) friend constexpr decltype(auto) operator op(Noop, T&& rhs) noexcept { return rhs; }	\
			template<typename T> requires(!std::convertible_to<T, Noop const&>) friend constexpr decltype(auto) operator op(T&& lhs, Noop) noexcept { return lhs; }
			BoolOp(&&)	BoolOp(||)
			BoolOp(==)	BoolOp(!=)
			BoolOp(>=)	BoolOp(<=)
			BoolOp(>)	BoolOp(<)
			BoolOp(<=>)
			#undef BoolOp
			
			#define NoopCmp(op)	\
			friend constexpr decltype(auto) operator op(Noop, Noop) noexcept { return Noop(); }
			NoopCmp(==)	NoopCmp(!=)
			NoopCmp(>=)	NoopCmp(<=)
			NoopCmp(>)	NoopCmp(<)
			#undef NoopCmp
			
			template<typename T> constexpr decltype(auto)
			operator=(T&&)
			{ return *this; }
			
			template<typename... Args> constexpr decltype(auto)
			operator()(Args&&...) noexcept
			{ return *this; }
			
			template<typename T> constexpr decltype(auto)
			operator[](T&&) noexcept
			{ return *this; }
			
		};
		
		
		template<typename T>
		concept non_void = (!std::same_as<void, T>);
		
		enum class XYZ { X = 1 , Y = 2, Z = 4 };
		template<typename T, XYZ tag>
		struct AxisBase;
		
		template<non_void T>	struct AxisBase<T, XYZ::X> { T x; };
		template<non_void T>	struct AxisBase<T, XYZ::Y> { T y; };
		template<non_void T>	struct AxisBase<T, XYZ::Z> { T z; };
		
		template<>				struct AxisBase<void, XYZ::X> { [[no_unique_address]] Noop x; };
		template<>				struct AxisBase<void, XYZ::Y> { [[no_unique_address]] Noop y; };
		template<>				struct AxisBase<void, XYZ::Z> { [[no_unique_address]] Noop z; };
		
		template<typename T, XYZ tag>
		struct Axis: public AxisBase<T, tag> {
			
			template<typename, XYZ> friend struct Axis;
			template<typename, typename, typename> struct VecBase;
			template<typename X, typename Y, typename Z> class Vec;
			
			protected: static constexpr bool
			is_void = std::same_as<void, T>;
			
			protected: using
			base = AxisBase<T, tag>;
			
			public: using
			value_type = T;
			
			protected: using
			ctr_arg = std::conditional_t<std::same_as<T, void>, Noop, T>;
			
			public: constexpr
			Axis() noexcept(noexcept(base{}))
			requires(std::default_initializable<base>):
			base{} {}
			
			public: constexpr
			Axis(std::convertible_to<ctr_arg> auto&& v) noexcept(noexcept( base{std::forward<decltype(v)>(v)} ))
			: base{std::forward<decltype(v)>(v)} {}
			
			public: template<std::convertible_to<T> U> requires(tag == XYZ::X) constexpr
			Axis(Axis<U, tag> const& other) noexcept(noexcept( base{std::forward<decltype(other.get_axis_data())>(other.get_axis_data())} ))
			: base{std::forward<decltype(other.get_axis_data())>(other.get_axis_data())} {}
			
			private: constexpr auto&
			get_axis_data() noexcept {
				if constexpr(tag == XYZ::X) return this->x;
				if constexpr(tag == XYZ::Y) return this->y;
				if constexpr(tag == XYZ::Z) return this->z;
			}
			
			private: constexpr auto&
			get_axis_data() const noexcept {
				if constexpr(tag == XYZ::X) return this->x;
				if constexpr(tag == XYZ::Y) return this->y;
				if constexpr(tag == XYZ::Z) return this->z;
			}
			
			public: friend constexpr decltype(auto)
			operator+(std::convertible_to<Axis const&> auto&& lhs, auto&& rhs)
			noexcept(noexcept(lhs.get_axis_data() + rhs.get_axis_data()))
			requires requires(decltype(lhs) l, decltype(rhs) r)
			{ {l.get_axis_data() + r.get_axis_data()}; }
			{
				return Axis<decltype(lhs.get_axis_data() + rhs.get_axis_data()), tag>
				(lhs.get_axis_data() + rhs.get_axis_data());
			}
			
			public: friend constexpr decltype(auto)
			operator+(std::convertible_to<Axis const&> auto&& v)
			noexcept(noexcept(+v.get_axis_data()))
			requires requires(decltype(v) v)
			{ {+v.get_axis_data()}; }
			{
				return Axis<decltype(+v.get_axis_data()), tag>
				(+v.get_axis_data());
			}
			
			public: friend constexpr decltype(auto)
			operator-(std::convertible_to<Axis const&> auto&& lhs, auto&& rhs)
			noexcept(noexcept(lhs.get_axis_data() - rhs.get_axis_data()))
			requires requires(decltype(lhs) l, decltype(rhs) r)
			{ {l.get_axis_data() - r.get_axis_data()}; }
			{
				return Axis<decltype(lhs.get_axis_data() - rhs.get_axis_data()), tag>
				(lhs.get_axis_data() - rhs.get_axis_data());
			}
			
			public: friend constexpr decltype(auto)
			operator-(std::convertible_to<Axis const&> auto&& v)
			noexcept(noexcept((-v.get_axis_data())))
			requires requires(decltype(v) v)
			{ {-v.get_axis_data()}; }
			{
				return Axis<decltype(-v.get_axis_data()), tag>
				(-v.get_axis_data());
			}
			
			public: friend constexpr decltype(auto)
			operator*(std::convertible_to<Axis const&> auto&& lhs, auto&& rhs)
			noexcept(noexcept(lhs.get_axis_data() * rhs.get_axis_data()))
			requires requires(decltype(lhs) l, decltype(rhs) r)
			{ {l.get_axis_data() * r.get_axis_data()}; }
			{
				return Axis<decltype(lhs.get_axis_data() * rhs.get_axis_data()), tag>
				(lhs.get_axis_data() * rhs.get_axis_data());
			}
			
			public: friend constexpr decltype(auto)
			operator/(std::convertible_to<Axis const&> auto&& lhs, auto&& rhs)
			noexcept(noexcept(lhs.get_axis_data() / rhs.get_axis_data()))
			requires requires(decltype(lhs) l, decltype(rhs) r)
			{ {l.get_axis_data() / r.get_axis_data()}; }
			{
				return Axis<decltype(lhs.get_axis_data() / rhs.get_axis_data()), tag>
				(lhs.get_axis_data() / rhs.get_axis_data());
			}
			
			public: friend constexpr decltype(auto)
			operator%(std::convertible_to<Axis const&> auto&& lhs, auto&& rhs)
			noexcept(noexcept(lhs.get_axis_data() % rhs.get_axis_data()))
			requires requires(decltype(lhs) l, decltype(rhs) r)
			{ {l.get_axis_data() % r.get_axis_data()}; }
			{
				return Axis<decltype(lhs.get_axis_data() % rhs.get_axis_data()), tag>
				(lhs.get_axis_data() % rhs.get_axis_data());
			}
			
			public: friend constexpr decltype(auto)
			operator!(std::convertible_to<Axis const&> auto&& v)
			noexcept(noexcept(!v.get_axis_data()))
			requires requires(decltype(v) v)
			{ {!v.get_axis_data()}; }
			{
				return Axis<decltype(!v.get_axis_data()), tag>
				(!v.get_axis_data());
			}
			
			public: friend constexpr decltype(auto)
			operator==(std::convertible_to<Axis const&> auto&& lhs, auto&& rhs)
			noexcept(noexcept(lhs.get_axis_data() == rhs.get_axis_data()))
			requires requires(decltype(lhs) l, decltype(rhs) r)
			{ {l.get_axis_data() == r.get_axis_data()}; }
			{
				return Axis<decltype(lhs.get_axis_data() == rhs.get_axis_data()), tag>
				(lhs.get_axis_data() == rhs.get_axis_data());
			}
			
			public: friend constexpr decltype(auto)
			operator!=(std::convertible_to<Axis const&> auto&& lhs, auto&& rhs)
			noexcept(noexcept(lhs.get_axis_data() != rhs.get_axis_data()))
			requires requires(decltype(lhs) l, decltype(rhs) r)
			{ {l.get_axis_data() != r.get_axis_data()}; }
			{
				return Axis<decltype(lhs.get_axis_data() != rhs.get_axis_data()), tag>
				(lhs.get_axis_data() != rhs.get_axis_data());
			}
			
			public: friend constexpr decltype(auto)
			operator>=(std::convertible_to<Axis const&> auto&& lhs, auto&& rhs)
			noexcept(noexcept(lhs.get_axis_data() >= rhs.get_axis_data()))
			requires requires(decltype(lhs) l, decltype(rhs) r)
			{ {l.get_axis_data() >= r.get_axis_data()}; }
			{
				return Axis<decltype(lhs.get_axis_data() >= rhs.get_axis_data()), tag>
				(lhs.get_axis_data() >= rhs.get_axis_data());
			}
			
			public: friend constexpr decltype(auto)
			operator<=(std::convertible_to<Axis const&> auto&& lhs, auto&& rhs)
			noexcept(noexcept(lhs.get_axis_data() <= rhs.get_axis_data()))
			requires requires(decltype(lhs) l, decltype(rhs) r)
			{ {l.get_axis_data() <= r.get_axis_data()}; }
			{
				return Axis<decltype(lhs.get_axis_data() <= rhs.get_axis_data()), tag>
				(lhs.get_axis_data() <= rhs.get_axis_data());
			}
			
			public: friend constexpr decltype(auto)
			operator>(std::convertible_to<Axis const&> auto&& lhs, auto&& rhs)
			noexcept(noexcept(lhs.get_axis_data() > rhs.get_axis_data()))
			requires requires(decltype(lhs) l, decltype(rhs) r)
			{ {l.get_axis_data() > r.get_axis_data()}; }
			{
				return Axis<decltype(lhs.get_axis_data() > rhs.get_axis_data()), tag>
				(lhs.get_axis_data() > rhs.get_axis_data());
			}
			
			public: friend constexpr decltype(auto)
			operator<(std::convertible_to<Axis const&> auto&& lhs, auto&& rhs)
			noexcept(noexcept(lhs.get_axis_data() < rhs.get_axis_data()))
			requires requires(decltype(lhs) l, decltype(rhs) r)
			{ {l.get_axis_data() < r.get_axis_data()}; }
			{
				return Axis<decltype(lhs.get_axis_data() > rhs.get_axis_data()), tag>
				(lhs.get_axis_data() < rhs.get_axis_data());
			}
			
			public: friend constexpr decltype(auto)
			operator&&(std::convertible_to<Axis const&> auto&& lhs, auto&& rhs)
			noexcept(noexcept(lhs.get_axis_data() && rhs.get_axis_data()))
			requires requires(decltype(lhs) l, decltype(rhs) r)
			{ {l.get_axis_data() && r.get_axis_data()}; }
			{
				return Axis<decltype(lhs.get_axis_data() && rhs.get_axis_data()), tag>
				(lhs.get_axis_data() && rhs.get_axis_data());
			}
			
			public: friend constexpr decltype(auto)
			operator||(std::convertible_to<Axis const&> auto&& lhs, auto&& rhs)
			noexcept(noexcept(lhs.get_axis_data() || rhs.get_axis_data()))
			requires requires(decltype(lhs) l, decltype(rhs) r)
			{ {l.get_axis_data() || r.get_axis_data()}; }
			{
				return Axis<decltype(lhs.get_axis_data() || rhs.get_axis_data()), tag>
				(lhs.get_axis_data() || rhs.get_axis_data());
			}
			
		};
		
		template<size_t Which, typename X, typename Y, typename Z>
		using AxisByAlignment = internal::AlignTypes_t<Axis<X, XYZ::X>, Axis<Y, XYZ::Y>, Axis<Z, XYZ::Z>>::template data<Which>;
		
		template<typename X, typename Y, typename Z>
		struct VecBase:
			public AxisByAlignment<0, X, Y, Z>,
			public AxisByAlignment<1, X, Y, Z>,
			public AxisByAlignment<2, X, Y, Z>
		{
			
			template<typename, XYZ> friend struct Axis;
			template<typename, typename, typename> friend struct VecBase;
			template<typename, typename, typename> friend class ink::Vec;
			
			public: using XYZ = detail::XYZ;
			
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
			base_bo<0>( forward_param<0>( std::forward<decltype(x)>(x) , std::forward<decltype(y)>(y) , std::forward<decltype(z)>(z) ) ),
			base_bo<1>( forward_param<1>( std::forward<decltype(x)>(x) , std::forward<decltype(y)>(y) , std::forward<decltype(z)>(z) ) ),
			base_bo<2>( forward_param<2>( std::forward<decltype(x)>(x) , std::forward<decltype(y)>(y) , std::forward<decltype(z)>(z) ) ) {}
			
		};
		
	}

	template<typename X, typename Y = X, typename Z = void>
	class Vec: public detail::VecBase<X, Y, Z>
	{
		
		template<typename x, typename y, typename z> friend class Vec;
		
		private: using
		base = detail::VecBase<X, Y, Z>;
		
		public: using XYZ = base::XYZ;
		
		private: template<typename T, XYZ tag>
		using Axis = detail::Axis<T, tag>;
		
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
		: base( std::forward<decltype(x)>(x) , std::forward<decltype(y)>(y) , std::forward<decltype(z)>(z) ) {}
		
		public: constexpr
		Vec(std::convertible_to<ctr_X> auto&& x,
			std::convertible_to<ctr_Y> auto&& y)
		noexcept(noexcept( base( x , y , ctr_Z() ) ))
		requires( std::default_initializable<axis_Z> || axis_Z::is_void )
		: base( std::forward<decltype(x)>(x) , std::forward<decltype(y)>(y) , ctr_Z() ) {}
		
		public: constexpr
		Vec(std::convertible_to<ctr_X> auto&& x)
		noexcept(noexcept( base( x , ctr_Y(), ctr_Z() ) ))
		requires( (std::default_initializable<axis_Y> || axis_Y::is_void) && (std::default_initializable<axis_Z> || axis_Z::is_void) )
		: base( std::forward<decltype(x)>(x) , ctr_Y() , ctr_Z() ) {}
		
		
		
		/* Conditional Constructors */
		
		public: constexpr
		Vec(std::convertible_to<ctr_Y> auto&& y,
			std::convertible_to<ctr_Z> auto&& z)
		noexcept(noexcept( base( ctr_X() , y , z ) ))
		requires( axis_X::is_void )
		: base( ctr_X() , std::forward<decltype(y)>(y) , std::forward<decltype(z)>(z) ) {}
		
		public: constexpr
		Vec(std::convertible_to<ctr_X> auto&& x,
			std::convertible_to<ctr_Z> auto&& z)
		noexcept(noexcept( base( x , ctr_Y() , z) ))
		requires( axis_Y::is_void )
		: base( x , ctr_Y() , z ) {}
		
		private: template<typename OX, typename OY, typename OZ>
		using OxVecBase = Vec<OX, OY, OZ>::base::base_X;
		private: template<typename OX, typename OY, typename OZ>
		using OyVecBase = Vec<OX, OY, OZ>::base::base_Y;
		private: template<typename OX, typename OY, typename OZ>
		using OzVecBase = Vec<OX, OY, OZ>::base::base_Z;
		
		#define BINARY_OP(opname, op, rhs_spec)																			\
		\
		public: template<typename RX, typename RY, typename RZ>															\
		static constexpr decltype(auto)																					\
		opname(																											\
			std::convertible_to<Vec const&> auto&& lhs,																	\
			Vec<RX, RY, RZ> rhs_spec rhs																				\
		) noexcept (																									\
				noexcept( axis_X::opname(lhs.template get_axis<XYZ::X>(), rhs.template get_axis<XYZ::X>()) )			\
			&&	noexcept( axis_Y::opname(lhs.template get_axis<XYZ::Y>(), rhs.template get_axis<XYZ::Y>()) )			\
			&&	noexcept( axis_Z::opname(lhs.template get_axis<XYZ::Z>(), rhs.template get_axis<XYZ::Z>()) )			\
		) requires requires(decltype(lhs) lhs, decltype(rhs) rhs) {														\
			axis_X::opname(lhs.template get_axis<XYZ::X>(), rhs.template get_axis<XYZ::X>());							\
			axis_Y::opname(lhs.template get_axis<XYZ::Y>(), rhs.template get_axis<XYZ::Y>());							\
			axis_Z::opname(lhs.template get_axis<XYZ::Z>(), rhs.template get_axis<XYZ::Z>());							\
		}																												\
		{																												\
			auto&& x_axis_out =	axis_X::opname(lhs.template get_axis<XYZ::X>(), rhs.template get_axis<XYZ::X>());		\
			auto&& y_axis_out =	axis_Y::opname(lhs.template get_axis<XYZ::Y>(), rhs.template get_axis<XYZ::Y>());		\
			auto&& z_axis_out =	axis_Z::opname(lhs.template get_axis<XYZ::Z>(), rhs.template get_axis<XYZ::Z>());		\
																														\
			using OX = typename std::remove_reference_t<decltype(x_axis_out)>::value_type;								\
			using OY = typename std::remove_reference_t<decltype(y_axis_out)>::value_type;								\
			using OZ = typename std::remove_reference_t<decltype(z_axis_out)>::value_type;								\
																														\
			return Vec<OX, OY, OZ>																						\
			(																											\
				static_cast<Vec<OX, OY, OZ>::base>(x_axis_out).template data<XYZ::X>(),											\
				static_cast<Vec<OX, OY, OZ>::base>(y_axis_out).template data<XYZ::Y>(),																								\
				static_cast<Vec<OX, OY, OZ>::base>(z_axis_out).template data<XYZ::Z>()																								\
			);																											\
		}
		
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
