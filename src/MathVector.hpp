#ifndef INK_MATH_VECTOR_HEADER_FILE_GUARD
#define INK_MATH_VECTOR_HEADER_FILE_GUARD

#include <cstdlib>
#include <utility>
#include <tuple>
#include <concepts>	 

namespace ink {
	
	namespace internal {
		
		template<typename... T>
		struct CheckAligned: std::true_type {};
		
		// Is true when the varidic type argument list is ordered, such that each successive type in the list has an alignment which is
		// equal to, or lesser than that of the type preceding it (left to right). This is useful for checking if a list of types
		// would be tightly packed in a struct, when ordered as such; no type arguments will evaluate to true, since no type arguments
		// would also give way to what could be considered a 'tightly packed struct'.
		template<typename... T> static constexpr bool
		CheckAligned_v = CheckAligned<T...>::value;
		
		template<typename T1, typename T2, typename... T>
		struct CheckAligned<T1, T2, T...>: std::bool_constant
		<	(alignof(T1) >= alignof(T2)) && (CheckAligned<T2, T...>::value)	> {};
		
		template<typename T1, typename T2>
		struct CheckAligned<T1, T2>: std::bool_constant
		<	(alignof(T1) >= alignof(T2))	> {};
		
		template<typename... T>
		struct AlignTypes;
		
		template<typename... T> requires(CheckAligned_v<T...>)
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
		requires( !CheckAligned_v<T1, T2, T...> )
		class AlignTypes<T1, T2, T...>
		{
			using _0 = std::conditional_t<(alignof(T1) >= alignof(T2)), T1, T2>;
			using _1 = std::conditional_t<(alignof(T1) >= alignof(T2)), T2, T1>;
			
			public:
			using type = typename AlignTypes<
				_0 , typename AlignTypes<_1, T...>::type
			>:: type;
		};
		
		template<typename... T>
		using AlignTypes_t = typename AlignTypes<T...>::type;
		
	}
	
	template<typename X, typename Y, typename Z> class Vec;
	
	namespace detail {
		
		// This is a utility construct; it can be operated on with anything, but always produce a noop,
		// either returning the other operated variable reference, or itself in the case of unary operations.
		// This applies as well to operations involving comparisons. 
		// Note: can be constructed from a nullptr.
		// It's an empty struch, therefore should be fine to decorate this with [[no_unique_address]].
		struct Noop {
			
			template<typename T>
			using void_if_noop = std::conditional_t<(std::same_as<T, detail::Noop>), void, T>;
			
			constexpr Noop(std::nullptr_t = nullptr) noexcept {}
			
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
			friend constexpr decltype(auto) operator op(std::common_reference_with<Noop> auto&& noop) noexcept { return noop; }
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
			
			#define NoopAssignOp(op)	\
			template<typename T> constexpr decltype(auto) operator op(T&& rhs) const noexcept { return rhs; }
			NoopAssignOp(=)
			NoopAssignOp(+=)	NoopAssignOp(-=)
			NoopAssignOp(*=)	NoopAssignOp(/=)	NoopAssignOp(%=)
			NoopAssignOp(&=)	NoopAssignOp(|=)	NoopAssignOp(^=)
			NoopAssignOp(>>=)	NoopAssignOp(<<=)
			#undef NoopAssignOp
			
			constexpr decltype(auto)
			operator++() const noexcept
			{ return *this; }
			
			constexpr decltype(auto)
			operator++(int) const noexcept
			{ return *this; }
			
			constexpr decltype(auto)
			operator--() const noexcept
			{ return *this; }
			
			constexpr decltype(auto)
			operator--(int) const noexcept
			{ return *this; }
			
			template<typename... Args> constexpr decltype(auto)
			operator()([[maybe_unused]] Args&&...) const noexcept
			{ return *this; }
			
			template<typename T> constexpr decltype(auto)
			operator[]([[maybe_unused]] T&&) const noexcept
			{ return *this; }
			
			constexpr operator std::nullptr_t() const noexcept
			{ return nullptr; }
			
		};
		
		template<typename T>
		concept non_void = (!std::same_as<void, T>);
		
		
		
		enum class XYZ { X = 1 , Y = 2, Z = 4 };
		template<typename T, XYZ tag>
		struct Axis;
		
		template<non_void T>
		struct Axis<T, XYZ::X>
		{
			public: constexpr
			Axis(auto&& x)
			noexcept(noexcept(T(x)))
			: x(x) {}
			
			public: constexpr
			Axis() noexcept(noexcept(T()))
			requires(std::default_initializable<T>)
			: x() {}
			
			T x;
		};
		
		template<non_void T>
		struct Axis<T, XYZ::Y>
		{
			public: constexpr
			Axis(auto&& y)
			noexcept(noexcept(T(y)))
			: y(y) {}
			
			public: constexpr
			Axis() noexcept(noexcept(T()))
			requires(std::default_initializable<T>)
			: y() {}
			
			T y;
		};
		
		template<non_void T>
		struct Axis<T, XYZ::Z>
		{
			public: constexpr
			Axis(auto&& z)
			noexcept(noexcept(T(z)))
			: z(z) {}
			
			public: constexpr
			Axis() noexcept(noexcept(T()))
			requires(std::default_initializable<T>)
			: z() {}
			
			T z;
		};
		
		
		
		template<>
		struct Axis<void, XYZ::X>
		{
			template<typename, typename, typename> friend struct VecBase;
			public: constexpr
			Axis([[maybe_unused]] std::nullptr_t = nullptr) noexcept {}
			protected: static constexpr Noop x{};
		};
		
		template<>
		struct Axis<void, XYZ::Y>
		{
			template<typename, typename, typename> friend struct VecBase;
			public: constexpr
			Axis([[maybe_unused]] std::nullptr_t = nullptr) noexcept {}
			protected: static constexpr Noop y{};
		};
		
		template<>
		struct Axis<void, XYZ::Z>
		{
			template<typename, typename, typename> friend struct VecBase;
			constexpr Axis([[maybe_unused]] std::nullptr_t = nullptr) noexcept {}
			protected: static constexpr Noop z{};
		};
		
		
		
		template<typename A1, typename A2, typename A3>
		struct VecBase;
		
		template<typename T1, XYZ tag1, typename T2, XYZ tag2, typename T3, XYZ tag3>
		struct VecBase<Axis<T1, tag1>, Axis<T2, tag2>, Axis<T3, tag3>>:
			public Axis<T1, tag1>,
			public Axis<T2, tag2>,
			public Axis<T3, tag3>
		{
			
			private:
			using base1 = Axis<T1, tag1>;
			using base2 = Axis<T2, tag2>;
			using base3 = Axis<T3, tag3>;
			
			protected: template<typename T>
			using ctr_arg = std::conditional_t<(std::same_as<T,void>), std::nullptr_t, T>;
			
			private: template<typename arg1, typename arg2, typename arg3>
			static constexpr bool
			NOEXCEPT_CTR
			=	noexcept( base1( std::declval<arg1>() ) )
			&&	noexcept( base2( std::declval<arg2>() ) )
			&&	noexcept( base3( std::declval<arg3>() ) );
			
			private: template<XYZ tag1_eq, XYZ tag2_eq, XYZ tag3_eq>
			static constexpr bool
			ASSERT_AXIS_ORDER
			=	(tag1 == tag1_eq)
			&&	(tag2 == tag2_eq)
			&&	(tag3 == tag3_eq);
			
			public: constexpr
			VecBase() noexcept( noexcept(base1()) && noexcept(base2()) && noexcept(base3()) )
			requires(	std::default_initializable<base1>
			&&			std::default_initializable<base2>
			&&			std::default_initializable<base3>)
			: base1(), base2(), base3() {}
			
			public: constexpr
			VecBase(auto&& x, auto&& y, auto&& z)
			noexcept( NOEXCEPT_CTR<decltype(x), decltype(y), decltype(z)> )
			requires( ASSERT_AXIS_ORDER<XYZ::X, XYZ::Y, XYZ::Z> )
			: base1(x), base2(y), base3(z) {}
			
			public: constexpr
			VecBase(auto&& x, auto&& y, auto&& z)
			noexcept( NOEXCEPT_CTR<decltype(x), decltype(z), decltype(y)> )
			requires( ASSERT_AXIS_ORDER<XYZ::X, XYZ::Z, XYZ::Y> )
			: base1(x), base2(z), base3(y) {}
			
			public: constexpr
			VecBase(auto&& x, auto&& y, auto&& z)
			noexcept( NOEXCEPT_CTR<decltype(y), decltype(x), decltype(z)> )
			requires( ASSERT_AXIS_ORDER<XYZ::Y, XYZ::X, XYZ::Z> )
			: base1(y), base2(x), base3(z) {}
			
			public: constexpr
			VecBase(auto&& x, auto&& y, auto&& z)
			noexcept( NOEXCEPT_CTR<decltype(y), decltype(z), decltype(x)> )
			requires( ASSERT_AXIS_ORDER<XYZ::Y, XYZ::Z, XYZ::X> )
			: base1(y), base2(z), base3(x) {}
			
			public: constexpr
			VecBase(auto&& x, auto&& y, auto&& z)
			noexcept( NOEXCEPT_CTR<decltype(z), decltype(x), decltype(y)> )
			requires( ASSERT_AXIS_ORDER<XYZ::Z, XYZ::X, XYZ::Y> )
			: base1(z), base2(x), base3(y) {}
			
			public: constexpr
			VecBase(auto&& x, auto&& y, auto&& z)
			noexcept( NOEXCEPT_CTR<decltype(z), decltype(y), decltype(x)> )
			requires( ASSERT_AXIS_ORDER<XYZ::Z, XYZ::Y, XYZ::X> )
			: base1(z), base2(y), base3(x) {}
			
		};
		
		template<typename X, typename Y, typename Z>
		using VecBasePacked = internal::AlignTypes_t<Axis<X, XYZ::X>, Axis<Y, XYZ::Y>, Axis<Z, XYZ::Z>>::template rebind<VecBase>;
		
	}
	
	template<typename X, typename Y, typename Z>
	static constexpr decltype(auto)
	make_vec(X&& x, Y&& y, Z&& z) noexcept
	{
		
		return Vec(
			[&]()constexpr noexcept { if constexpr(std::same_as<std::decay_t<X>, detail::Noop>) { return nullptr; } else { return std::forward<X>(x); } }(),
			[&]()constexpr noexcept { if constexpr(std::same_as<std::decay_t<Y>, detail::Noop>) { return nullptr; } else { return std::forward<Y>(y); } }(),
			[&]()constexpr noexcept { if constexpr(std::same_as<std::decay_t<Z>, detail::Noop>) { return nullptr; } else { return std::forward<Z>(z); } }()
		);
	}
	
	template<typename X, typename Y = X, typename Z = void>
	class Vec: public detail::VecBasePacked<X, Y, Z> {
		
		private: using
		base = detail::VecBasePacked<X, Y, Z>;
		
		private: template<typename T>
		using void_if_noop = typename detail::Noop::void_if_noop<T>;
		
		private:
		using ctr_x = typename base::ctr_arg<X>;
		using ctr_y = typename base::ctr_arg<Y>;
		using ctr_z = typename base::ctr_arg<Z>;
		
		private: static constexpr bool
		void_x = std::same_as<void, X>,
		void_y = std::same_as<void, Y>,
		void_z = std::same_as<void, Z>;
		
		/* CONSTRUCTORS */
		
		public: constexpr
		Vec() noexcept(noexcept(base()))
		requires(std::default_initializable<base>)
		: base() {}
		
		public: constexpr
		Vec(std::common_reference_with<ctr_x> auto&& x, std::common_reference_with<ctr_y> auto&& y, std::common_reference_with<ctr_z> auto&& z)
		noexcept(noexcept(base(x, y, z)))
		: base(x, y, z) {}
		
		public: constexpr
		Vec(std::common_reference_with<ctr_x> auto&& x, std::common_reference_with<ctr_y> auto&& y)
		noexcept(noexcept(base(x, y, std::declval<ctr_z>())))
		requires( (void_z || std::default_initializable<ctr_z>) )
		: base(x, y, ctr_z()) {}
		
		public: constexpr
		Vec(std::common_reference_with<ctr_x> auto&& x)
		noexcept(noexcept(base(x, std::declval<ctr_y>(), std::declval<ctr_z>())))
		requires( (void_y || std::default_initializable<ctr_y>) && (void_z || std::default_initializable<ctr_z>))
		: base(x, ctr_y(), ctr_z()) {}
		
		public: constexpr
		Vec(std::common_reference_with<ctr_x> auto&& x, std::common_reference_with<ctr_z> auto&& z)
		noexcept(noexcept(base(x, std::declval<ctr_y>(), z)))
		requires( !void_x && void_y && !void_z )
		: base(x, ctr_y(), z) {}
		
		public: constexpr
		Vec(std::common_reference_with<ctr_y> auto&& y, std::common_reference_with<ctr_z> auto&& z)
		noexcept(noexcept(base(std::declval<ctr_x>(), y, z)))
		requires( void_x && !void_y && !void_z )
		: base(ctr_x(), y, z) {}
		
		public: constexpr
		Vec(std::common_reference_with<ctr_y> auto&& y)
		noexcept(noexcept(base(std::declval<ctr_x>(), y, std::declval<ctr_z>())))
		requires( void_x && !void_y && void_z )
		: base(ctr_x(), y, ctr_z()) {}
		
		public: constexpr
		Vec(std::common_reference_with<ctr_z> auto&& z)
		noexcept(noexcept(base(std::declval<ctr_x>(), std::declval<ctr_y>(), z)))
		requires( void_x && void_y && !void_z )
		: base(ctr_x(), ctr_y(), z) {}
		
		
		
		/* GET FUNCTION */
		public: template<size_t i> friend constexpr auto&&
		get(std::common_reference_with<Vec> auto&& vec) noexcept
		{
			if constexpr(i == 0) return vec.x;
			if constexpr(i == 1) return vec.y;
			if constexpr(i == 2) return vec.z;
		}
		
		
		
		/* OPERATORS */
		#define BINARY_OPERATOR(op, lhs_spec, rhs_spec)	\
		public: template<typename RX, typename RY, typename RZ>	\
		friend constexpr decltype(auto)	\
		operator op (Vec lhs_spec lhs, Vec<RX, RY, RZ> rhs_spec rhs)	\
		noexcept(noexcept(lhs.x op rhs.x) && noexcept(lhs.y op rhs.y) && noexcept(lhs.z op rhs.z))	\
		requires requires(Vec lhs_spec lhs, Vec<RX, RY, RZ> rhs_spec rhs)	\
		{ {lhs.x op rhs.x}; {lhs.y op rhs.y}; {lhs.z op rhs.z}; }	\
		{ return make_vec(lhs.x op rhs.x, lhs.y op rhs.y, lhs.z op rhs.z); }
		
		BINARY_OPERATOR(+,	&,		&		)
		BINARY_OPERATOR(+,	&,		&&		)
		BINARY_OPERATOR(+,	&,		const&	)
		
		BINARY_OPERATOR(+,	&&,		&		)
		BINARY_OPERATOR(+,	&&,		&&		)
		BINARY_OPERATOR(+,	&&,		const&	)
		
		BINARY_OPERATOR(+,	const&,	&		)
		BINARY_OPERATOR(+,	const&,	&&		)
		BINARY_OPERATOR(+,	const&,	const&	)
		
	};
	
	template<class X, class Y, class Z> Vec(X, Y, Z)			-> Vec<X, Y, Z>;
	
	template<class X> Vec(X, std::nullptr_t, std::nullptr_t)	-> Vec<X, void, void>;
	template<class Y> Vec(std::nullptr_t, Y, std::nullptr_t)	-> Vec<void, Y, void>;
	template<class Z> Vec(std::nullptr_t, std::nullptr_t, Z)	-> Vec<void, void, Z>;
	
	template<class X, class Y>	Vec(X, Y, std::nullptr_t)		-> Vec<X, Y, void>;
	template<class X, class Z>	Vec(X, std::nullptr_t, Z)		-> Vec<X, void, Z>;
	template<class Y, class Z>	Vec(std::nullptr_t, Y, Z)		-> Vec<void, Y, Z>;
	
	Vec(std::nullptr_t, std::nullptr_t, std::nullptr_t)			-> Vec<void, void, void>;
	Vec(std::nullptr_t, std::nullptr_t)							-> Vec<void, void, void>;
	Vec(std::nullptr_t)											-> Vec<void, void, void>;
	
	template<class X>			Vec(X)							-> Vec<X, void, void>;
	template<class X, class Y>	Vec(X, Y)						-> Vec<X, Y>;
	
	template<class X> Vec(X, std::nullptr_t)					-> Vec<X, void, void>;
	template<class Y> Vec(std::nullptr_t, Y)					-> Vec<void, Y, void>;
	
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
