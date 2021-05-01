#ifndef INK_MATH_VECTOR_HEADER_FILE_GUARD
#define INK_MATH_VECTOR_HEADER_FILE_GUARD

#include <cstdlib>
#include <utility>
#include <tuple>
#include <concepts>	 

namespace ink {
	
	namespace detail {
		
		// This is a utility construct; it can be operated on with anything, but always produce a noop,
		// either returning the other operated variable reference, or itself in the case of unary operations.
		// This applies as well to operations involving comparisons (returns the appropriate reference to the
		// left-hand-side or right-hand-side argument). 
		// It is an empty struct, therefore empty-base-like optimization can be applied using [[no_unique_address]] on supporting
		// compilers.
		// 
		// Note:
		// It is -implicitly- default constructible.
		// It is -implicitly- constructible from any type T.
		// It is -explicitly- convertible to any type T.
		// This means that Noop can essentially be a dummy-target for initializations, but will warn
		// if the user converts Noop to another type without statically casting it to the desired type.
		struct Noop {
			constexpr Noop() noexcept {}
			template<typename T> constexpr Noop([[maybe_unused]] T&&) noexcept {}
			template<typename T> explicit constexpr operator T() const noexcept(noexcept(T())) { return T(); }
			
			#define BinaryOp(op)	\
			friend constexpr auto&& operator op(Noop, auto&& rhs) noexcept { return rhs; }	\
			friend constexpr auto&& operator op(auto&& lhs, Noop) noexcept { return lhs; }	\
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
			
			#define NoopAssignOp(op)	\
			template<typename T> constexpr decltype(auto) operator op(T&& rhs) const noexcept { return rhs; }
			NoopAssignOp(=)
			NoopAssignOp(+=)	NoopAssignOp(-=)
			NoopAssignOp(*=)	NoopAssignOp(/=)	NoopAssignOp(%=)
			NoopAssignOp(&=)	NoopAssignOp(|=)	NoopAssignOp(^=)
			NoopAssignOp(>>=)	NoopAssignOp(<<=)
			#undef NoopAssignOp
			
			#define ShortHandOp(op, arg)	\
			constexpr decltype(auto) operator op(arg) const noexcept { return *this; }
			ShortHandOp(++,int)
			ShortHandOp(--,int)
			ShortHandOp(++,)
			ShortHandOp(--,)
			#undef ShortHandOp
			
			template<typename... Args> constexpr decltype(auto)
			operator()([[maybe_unused]] Args&&...) const noexcept
			{ return *this; }
			
			template<typename T> constexpr decltype(auto)
			operator[]([[maybe_unused]] T&&) const noexcept
			{ return *this; }
			
		};
		
		
		
		template<typename T, typename U>
		struct SameTemplateAs_t: std::false_type {};
		
		template<template<typename...> typename Template, typename... TArgs, typename... UArgs>
		struct SameTemplateAs_t<Template<TArgs...>, Template<UArgs...>>: std::true_type {};
		
		template<template<auto...> typename Template, auto... TArgs, auto... UArgs>
		struct SameTemplateAs_t<Template<TArgs...>, Template<UArgs...>>: std::true_type {};
		
		template<template<typename, auto...> typename Template, typename T1, typename U1, auto... TArgs, auto... UArgs>
		struct SameTemplateAs_t<Template<T1, TArgs...>, Template<U1, UArgs...>>: std::true_type {};
		
		template<typename T, typename U>
		concept SameTemplateAs = SameTemplateAs_t<T, U>::value;
		
		
		
		template<typename T>
		concept is_void = std::same_as<void, T>;
		
		template<typename T>
		concept non_void = !is_void<T>;
		
		
		
		enum class XYZ { X , Y , Z };
		
		template<typename T, XYZ tag> struct Axis;
		
		
		#define SpecializeAxis(XYZ_tag, member_name)			\
																\
		template<non_void T> struct Axis<T, XYZ_tag> {			\
			public: using value_type = T;						\
			protected: static constexpr XYZ tag = XYZ_tag;		\
			public: constexpr									\
			Axis()												\
			noexcept(noexcept(T()))								\
			requires( std::is_default_constructible<T>{}() )	\
			: member_name() {}									\
																\
			public: template<typename U> constexpr				\
			Axis(U&& member_name)								\
			noexcept(noexcept(T(std::declval<U>())))			\
			requires( std::is_constructible<T, U>{}() )			\
			: member_name(std::forward<U>(member_name)) {}		\
																\
			public: T member_name;								\
		};														\
																\
		template<> struct Axis<void, XYZ_tag> {					\
			public: using value_type = std::nullptr_t;			\
			protected: static constexpr XYZ tag = XYZ_tag;		\
																\
			public: constexpr									\
			Axis([[maybe_unused]] std::nullptr_t = nullptr)		\
			noexcept {}											\
																\
			protected: static constexpr Noop member_name{};		\
		};
		
		SpecializeAxis(XYZ::X, x)
		SpecializeAxis(XYZ::Y, y)
		SpecializeAxis(XYZ::Z, z)
		#undef SpecializeNonVoidAxis
		
		template<typename X, typename Y, typename Z>
		struct AxisGroup {
			public: using XYZ = detail::XYZ;
			public: static constexpr XYZ
			TagX = XYZ::X,
			TagY = XYZ::Y,
			TagZ = XYZ::Z;
			
			public: using AxisX = Axis<X, TagX>;
			public: using AxisY = Axis<Y, TagY>;
			public: using AxisZ = Axis<Z, TagZ>;
			
			public: template<XYZ tag> using AxisType =
			std::conditional_t<(tag == TagX), X, std::conditional_t<(tag == TagY), Y, Z>>;
			
			public: template<XYZ tag> using AxisByTag =
			detail::Axis<AxisType<tag>, tag>;
			
			private:
			static constexpr size_t
			AlignPosX = 2 - (alignof(X) >= alignof(Y)) - (alignof(X) >= alignof(Z)),
			AlignPosY = 2 - (alignof(X) <  alignof(Y)) - (alignof(Y) >= alignof(Z)),
			AlignPosZ = 2 - (alignof(X) <  alignof(Z)) - (alignof(Y) <  alignof(Z));
			
			public: template<XYZ tag>
			static constexpr size_t
			AlignPosOfAxis =
			/*if*/(tag == TagX) ? AlignPosX :
			/*if*/(tag == TagY) ? AlignPosY :
			/*if*/(tag == TagZ) ? AlignPosZ : -1;
			
			public: template<size_t i>
			static constexpr XYZ
			AxisWithAlignPos =
			/*if*/(i == AlignPosX) ? TagX :
			/*if*/(i == AlignPosY) ? TagY :
			/*if*/(i == AlignPosZ) ? TagZ : static_cast<XYZ>(size_t(-1));
			
			public: template<XYZ tag1, XYZ tag2, XYZ tag3>
			static constexpr bool
			REQUIRE_ALIGN =
			(AlignPosOfAxis<tag1> == 0) && (AlignPosOfAxis<tag2> == 1) && (AlignPosOfAxis<tag3> == 2);
			
			public: template<size_t i>
			using AxisAt = AxisByTag<AxisWithAlignPos<i>>;
			
			public: static constexpr bool
			noexcept_default_constructor = noexcept(AxisX()) && noexcept(AxisY()) && noexcept(AxisZ());
			
			public: template<typename XArg, typename YArg, typename ZArg> static constexpr bool
			noexcept_constructor = noexcept(AxisX(std::declval<XArg>())) && noexcept(AxisY(std::declval<YArg>())) && noexcept(AxisZ(std::declval<ZArg>()));
			
			public: static constexpr bool
			default_constructible
			=	std::is_default_constructible_v<AxisX>
			&&	std::is_default_constructible_v<AxisY>
			&&	std::is_default_constructible_v<AxisZ>;
			
			public: template<
				typename XArg = typename AxisX::value_type,
				typename YArg = typename AxisY::value_type,
				typename ZArg = typename AxisZ::value_type
			> static constexpr bool constructible_from
			=	std::is_constructible_v<AxisX, XArg>
			&&	std::is_constructible_v<AxisY, YArg>
			&&	std::is_constructible_v<AxisZ, ZArg>;
			
		};
		
		template<typename X, typename Y, typename Z, typename MetaInfo = AxisGroup<X, Y, Z>>
		struct VecBase: public MetaInfo::AxisAt<0>, public MetaInfo::AxisAt<1>, public MetaInfo::AxisAt<2>
		{
			protected: using Meta = MetaInfo;
			
			protected:
			using baseX = typename Meta::AxisX;
			using baseY = typename Meta::AxisY;
			using baseZ = typename Meta::AxisZ;
			
			protected:
			using ctr_x = baseX::value_type;
			using ctr_y = baseY::value_type;
			using ctr_z = baseZ::value_type;
			
			private:
			using base0 = typename Meta::AxisAt<0>;
			using base1 = typename Meta::AxisAt<1>;
			using base2 = typename Meta::AxisAt<2>;
			
			public: constexpr
			VecBase() noexcept( Meta::noexcept_default_constructor )
			requires( Meta::default_constructible )
			: base0(), base1(), base2() {}
			
			#define TEMP_FORWARD_LAMBDA_MACRO(base)									\
			[&]() constexpr noexcept -> decltype(auto) {							\
				if constexpr(base::tag == XYZ::X) { return std::forward<XArg>(x); }	\
				if constexpr(base::tag == XYZ::Y) { return std::forward<YArg>(y); }	\
				if constexpr(base::tag == XYZ::Z) { return std::forward<ZArg>(z); }	\
			}()
			
			public: template<typename XArg, typename YArg, typename ZArg> constexpr
			VecBase(XArg&& x, YArg&& y, ZArg&& z) noexcept( Meta::template noexcept_constructor<XArg, YArg, ZArg> )
			requires( Meta::template constructible_from<XArg, YArg, ZArg> )
			:	base0(TEMP_FORWARD_LAMBDA_MACRO(base0)),
				base1(TEMP_FORWARD_LAMBDA_MACRO(base1)),
				base2(TEMP_FORWARD_LAMBDA_MACRO(base2)) {}
			
			#undef TEMP_FORWARD_LAMBDA_MACRO
			
		};
		
	}
	
	template<typename X, typename Y, typename Z> class Vec;
	
	template<typename X, typename Y, typename Z>
	static constexpr decltype(auto)
	make_vec(X&& x, Y&& y, Z&& z) noexcept
	{
		return Vec(
			[&]() constexpr noexcept -> X { if constexpr(std::same_as<std::decay_t<X>, detail::Noop>) { return nullptr; } else { return std::forward<X>(x); } }(),
			[&]() constexpr noexcept -> Y { if constexpr(std::same_as<std::decay_t<Y>, detail::Noop>) { return nullptr; } else { return std::forward<Y>(y); } }(),
			[&]() constexpr noexcept -> Z { if constexpr(std::same_as<std::decay_t<Z>, detail::Noop>) { return nullptr; } else { return std::forward<Z>(z); } }()
		);
	}
	
	template<typename X, typename Y = X, typename Z = void>
	class Vec: public detail::VecBase<X, Y, Z> {
		
		template<typename, typename, typename> friend class Vec;
		
		private: using
		base = detail::VecBase<X, Y, Z>;
		
		private:
		using ctr_x = typename base::baseX::value_type;
		using ctr_y = typename base::baseX::value_type;
		using ctr_z = typename base::baseX::value_type;
		
		using type_x = decltype(base::x);
		using type_y = decltype(base::y);
		using type_z = decltype(base::z);
		
		private: static constexpr bool
		void_x = std::same_as<void, X>,
		void_y = std::same_as<void, Y>,
		void_z = std::same_as<void, Z>;
		
		
		
		/* CONSTRUCTORS */
		
		public: template<typename From>
		requires(
			detail::SameTemplateAs<Vec, std::remove_cvref_t<From>>
			&&	std::convertible_to<typename std::remove_cvref_t<From>::type_x, type_x>
			&&	std::convertible_to<typename std::remove_cvref_t<From>::type_y, type_y>
			&&	std::convertible_to<typename std::remove_cvref_t<From>::type_z, type_z>)
		constexpr
		Vec(From&& from)
		: base(static_cast<type_x>(from.x), static_cast<type_y>(from.y), static_cast<type_z>(from.z)) {}
		
		public: constexpr
		Vec() noexcept(noexcept(base()))
		requires( std::is_default_constructible<base>{}() )
		: base() {}
		
		public: template<typename OX, typename OY, typename OZ> constexpr
		Vec(OX&& x, OY&& y, OZ&& z) noexcept(noexcept(base(std::declval<OX>(), std::declval<OY>(), std::declval<OZ>())))
		requires(std::is_constructible_v<base, OX, OY, OZ>)
		: base(std::forward<OX>(x), std::forward<OY>(y), std::forward<OZ>(z)) {}
		
		
		
		public: template<typename OX, typename OY> constexpr
		Vec(OX&& x, OY&& y) noexcept(noexcept(base(std::declval<OX>(), std::declval<OY>(), ctr_z())))
		requires( !void_x && !void_y && (void_z || std::is_default_constructible_v<Z>) && std::is_constructible_v<base, OX, OY, ctr_z> )
		: base(std::forward<OX>(x), std::forward<OY>(y), ctr_z()) {}
		
		public: template<typename OX, typename OZ> constexpr
		Vec(OX&& x, OZ&& z) noexcept(noexcept(base(std::declval<OX>(), ctr_y(), std::declval<OZ>())))
		requires( !void_x && void_y && !void_z && std::is_constructible_v<base, OX, ctr_y, OZ> )
		: base(std::forward<OX>(x), ctr_y(), std::forward<OZ>(z)) {}
		
		public: template<typename OY, typename OZ> constexpr
		Vec(OY&& y, OZ&& z) noexcept(noexcept(base(ctr_x(), std::declval<OY>(), std::declval<OZ>())))
		requires( void_x && !void_y && !void_z && std::is_constructible_v<base, ctr_x, OY, OZ> )
		: base(ctr_x(), std::forward<OY>(y), std::forward<OZ>(z)) {}
		
		
		
		public: template<std::convertible_to<X> OX> explicit constexpr
		Vec(OX&& x) noexcept(noexcept(base(std::declval<OX>(), ctr_y(), ctr_z())))
		requires( !void_x && (void_y || std::is_default_constructible_v<Y>) && (void_z || std::is_default_constructible_v<Z>) && std::is_constructible_v<base, X, ctr_y, ctr_z> )
		: base(std::forward<OX>(x), ctr_y(), ctr_z()) {}
		
		public: template<typename OY> explicit constexpr
		Vec(OY&& y) noexcept(noexcept(base(ctr_x(), std::declval<OY>(), ctr_z())))
		requires( void_x && !void_y && void_z && std::is_constructible_v<base, ctr_z, OY, ctr_z> )
		: base(ctr_x(), std::forward<OY>(y), ctr_z()) {}
		
		public: template<typename OZ> explicit constexpr
		Vec(OZ&& z) noexcept(noexcept(base(ctr_x(), ctr_y(), std::declval<OZ>())))
		requires( void_x && void_y && !void_z && std::is_constructible_v<base, ctr_x, ctr_y, OZ> )
		: base(ctr_x(), ctr_y(), std::forward<OZ>(z)) {}
		
		
		
		/* Get method, for get<n>(Vec) syntax. */
		
		template<size_t i> requires(i % 3 == 0) friend constexpr auto&&
		get(Vec& v) noexcept {
			if constexpr(i == 0) return v.x;
			if constexpr(i == 1) return v.y;
			if constexpr(i == 2) return v.z;
		}
		
		template<size_t i> requires(i % 3 == 0) friend constexpr auto&&
		get(Vec&& v) noexcept {
			if constexpr(i == 0) return v.x;
			if constexpr(i == 1) return v.y;
			if constexpr(i == 2) return v.z;
		}
		
		template<size_t i> requires(i % 3 == 0) friend constexpr auto&&
		get(Vec const& v) noexcept {
			if constexpr(i == 0) return v.x;
			if constexpr(i == 1) return v.y;
			if constexpr(i == 2) return v.z;
		}
		
		
		
		private: template<typename Lhs, typename Rhs>
		static constexpr bool ConstrainToVecTypes
		=	std::is_same_v<Vec, std::remove_cvref_t<Lhs>>
		&&	detail::SameTemplateAs<Vec, std::remove_cvref_t<Rhs>>;
		
		public: template<typename T, typename U>
		requires(ConstrainToVecTypes<T, U>)
		friend constexpr decltype(auto)
		operator+(T&& lhs, U&& rhs) noexcept( noexcept(lhs.x + rhs.x) && noexcept(lhs.y + rhs.y) && noexcept(lhs.z + rhs.z) )
		requires requires(T&& lhs, U&& rhs) { {lhs.x + rhs.x}; {lhs.y + rhs.y}; {lhs.z + rhs.z}; }
		{
			return Vec
			<decltype(lhs.x + rhs.x), decltype(lhs.y + rhs.y), decltype(lhs.z + rhs.z)>
			(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
		}
		
		public: template<typename T, typename U>
		requires(ConstrainToVecTypes<T, U>)
		friend constexpr decltype(auto)
		operator-(T&& lhs, U&& rhs) noexcept( noexcept(lhs.x - rhs.x) && noexcept(lhs.y - rhs.y) && noexcept(lhs.z - rhs.z) )
		requires requires(T&& lhs, U&& rhs) { {lhs.x - rhs.x}; {lhs.y - rhs.y}; {lhs.z - rhs.z}; }
		{
			return make_vec(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
		}
		
		public: template<typename T, typename U>
		requires(ConstrainToVecTypes<T, U>)
		friend constexpr decltype(auto)
		operator*(T&& lhs, U&& rhs) noexcept( noexcept(lhs.x * rhs.x) && noexcept(lhs.y * rhs.y) && noexcept(lhs.z * rhs.z) )
		requires requires(T&& lhs, U&& rhs) { {lhs.x * rhs.x}; {lhs.y * rhs.y}; {lhs.z * rhs.z}; }
		{
			return make_vec(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
		}
		
		public: template<typename T, typename U>
		requires(ConstrainToVecTypes<T, U>)
		friend constexpr decltype(auto)
		operator/(T&& lhs, U&& rhs) noexcept( noexcept(lhs.x / rhs.x) && noexcept(lhs.y / rhs.y) && noexcept(lhs.z / rhs.z) )
		requires requires(T&& lhs, U&& rhs) { {lhs.x / rhs.x}; {lhs.y / rhs.y}; {lhs.z / rhs.z}; }
		{
			return make_vec(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
		}
		
		public: template<typename T, typename U>
		requires(ConstrainToVecTypes<T, U>)
		friend constexpr decltype(auto)
		operator%(T&& lhs, U&& rhs) noexcept( noexcept(lhs.x % rhs.x) && noexcept(lhs.y % rhs.y) && noexcept(lhs.z % rhs.z) )
		requires requires(T&& lhs, U&& rhs) { {lhs.x % rhs.x}; {lhs.y % rhs.y}; {lhs.z % rhs.z}; }
		{
			return make_vec(lhs.x % rhs.x, lhs.y % rhs.y, lhs.z % rhs.z);
		}
		
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
