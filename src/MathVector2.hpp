#ifndef INK_MATH_VECTOR_HEADER_FILE_GUARD
#define INK_MATH_VECTOR_HEADER_FILE_GUARD

#include <cstdlib>
#include <utility>
#include <type_traits>

namespace ink {
	
	namespace detail {
		template<typename T, typename U>
		struct SameTemplate_t: std::false_type {};
		
		template<template<typename...> typename Template, typename... TArgs, typename... UArgs>
		struct SameTemplate_t<Template<TArgs...>, Template<UArgs...>>: std::true_type {};
		
		template<template<auto...> typename Template, auto... TArgs, auto... UArgs>
		struct SameTemplate_t<Template<TArgs...>, Template<UArgs...>>: std::true_type {};
		
		template<template<typename, auto...> typename Template, typename T1, typename U1, auto... TArgs, auto... UArgs>
		struct SameTemplate_t<Template<T1, TArgs...>, Template<U1, UArgs...>>: std::true_type {};
		
		template<typename T, typename U>
		concept SameTemplate = SameTemplate_t<T, U>::value;
	}
	
	using detail::SameTemplate;
	template<typename From, typename To>
	concept reference_to = std::convertible_to<From, To const&>;
	
	
	/**
	 * This is a utility construct; it can be operated on with anything, but always produce a noop,
	 * either returning the other operated variable reference, or itself in the case of unary operations.
	 * This applies as well to operations involving comparisons (returns the appropriate reference to the
	 * left-hand-side or right-hand-side argument). 
	 * It is an empty struct, therefore empty-base-like optimization can be applied using [[no_unique_address]] on supporting
	 * compilers.
	 * Note:
	 * It is -implicitly- default constructible.
	 * It is -implicitly- constructible from any type T.
	 * It is -explicitly- convertible to any type T.
	 * This means that Noop can essentially be a dummy-target for initializations, but will warn
	 * if the user converts Noop to another type without statically casting it to the desired type.
	 */
	struct Noop {
		constexpr			Noop(std::nullptr_t = nullptr) noexcept {}
		explicit constexpr	Noop([[maybe_unused]] auto&&) noexcept {}
		template<typename T> explicit constexpr operator T() noexcept(T()) { return T(); }
		
		#define BinaryOp(op)																			\
		friend constexpr decltype(auto) operator op(Noop, auto&& rhs)	noexcept {	return rhs;		}	\
		friend constexpr decltype(auto) operator op(auto&& lhs, Noop)	noexcept {	return lhs;		}	\
		friend constexpr decltype(auto) operator op(Noop, Noop)			noexcept {	return Noop();	}
			BinaryOp(==)	BinaryOp(!=)
			BinaryOp(>=)	BinaryOp(>)
			BinaryOp(<=)	BinaryOp(<)
			BinaryOp(+)		BinaryOp(-)
			BinaryOp(*)		BinaryOp(/)		BinaryOp(%)
			BinaryOp(|)		BinaryOp(&)		BinaryOp(^)
			BinaryOp(>>)	BinaryOp(<<)
			BinaryOp(&&)	BinaryOp(||)	BinaryOp(<=>)
		#undef BinaryOp
		
		#define UnaryOp(op)	friend constexpr decltype(auto) operator op(reference_to<Noop> auto&& noop) noexcept { return noop; }
			UnaryOp(+)		UnaryOp(-)
			UnaryOp(!)		UnaryOp(~)
			UnaryOp(*)		UnaryOp(&)
		#undef UnaryOp
		
		#define ShortHandOp(op, arg) constexpr decltype(auto) operator op(arg) const noexcept { return *this; }
			ShortHandOp(++, int)
			ShortHandOp(--,int)
			ShortHandOp(++,)
			ShortHandOp(--,)
		#undef ShortHandOp
		
		#define AssignOp(op) constexpr decltype(auto) operator op(auto&& rhs) const noexcept { return *this; }
			AssignOp(=)
			AssignOp(+=)	AssignOp(-=)
			AssignOp(*=)	AssignOp(/=)	AssignOp(%=)
			AssignOp(|=)	AssignOp(&=)	AssignOp(^=)
			AssignOp(>>=)	AssignOp(<<=)
		#undef AssignOp
		
		template<typename... Args> constexpr decltype(auto)
		operator()([[maybe_unused]] Args&&...) const noexcept { return *this; }
		
		template<typename T> constexpr decltype(auto)
		operator[]([[maybe_unused]] T&&) const noexcept { return *this; }
		
	};
	
	namespace detail {
		
		enum class XYZ { X , Y , Z };
		template<typename T, XYZ tag> struct Axis;
		
		
		template<> struct Axis<void, XYZ::X> { constexpr Axis(std::nullptr_t = nullptr) noexcept {} protected: static constexpr Noop x{}; };
		template<> struct Axis<void, XYZ::Y> { constexpr Axis(std::nullptr_t = nullptr) noexcept {} protected: static constexpr Noop y{}; };
		template<> struct Axis<void, XYZ::Z> { constexpr Axis(std::nullptr_t = nullptr) noexcept {} protected: static constexpr Noop z{}; };
		
		
		
		template<typename T> requires(!std::is_same_v<void, T>)
		struct Axis<T, XYZ::X> {
			constexpr Axis() requires(std::default_initializable<T>) {
			constexpr Axis(reference_to<T> auto&& x): x(x) {}
			T x;
		};
		
		template<typename T> requires(!std::is_same_v<void, T>)
		struct Axis<T, XYZ::Y> {
			constexpr Axis() requires(std::default_initializable<T>) {}
			constexpr Axis(reference_to<T> auto&& y): y(y) {}
			T y;
		};
		
		template<typename T> requires(!std::is_same_v<void, T>)
		struct Axis<T, XYZ::Z> {
			constexpr Axis() requires(std::default_initializable<T>) {}
			constexpr Axis(reference_to<T> auto&& z): z(z) {}
			T z;
		};
		
		template<typename X, typename Y, typename Z>
		struct AxisGroupMetaInfo {
			
			public:
			using Tag = XYZ;
			
			public: template<Tag tag>
			using real_type = std::conditional_t<(tag == Tag::X), X, std::conditional_t<(tag == Tag::Y), Y, Z>>;
			
			public: template<Tag tag>
			using value_type = std::conditional_t<(std::is_same_v<real_type<tag>, void>), std::nullptr_t, real_type<tag>>;
			
			public: template<Tag tag>
			using GetAxis = detail::Axis<real_type<tag>, tag>;
			
			public: template<Tag tag>
			static constexpr size_t IndexOfAxis =
			(tag == Tag::X) ? (2 - char(alignof(real_type<Tag::X>) >= alignof(real_type<Tag::Y>)) - char(alignof(real_type<Tag::X>) >= alignof(real_type<Tag::Z>))) :
			(tag == Tag::Y) ? (2 - char(alignof(real_type<Tag::X>) <  alignof(real_type<Tag::Y>)) - char(alignof(real_type<Tag::Y>) >= alignof(real_type<Tag::Z>))) :
			(tag == Tag::Z) ? (2 - char(alignof(real_type<Tag::X>) <  alignof(real_type<Tag::Z>)) - char(alignof(real_type<Tag::Y>) <  alignof(real_type<Tag::Z>))) : -1;
			
			public: template<size_t i>
			static constexpr Tag AxisAtIndex =
			(i == IndexOfAxis<Tag::X>) ? (Tag::X) :
			(i == IndexOfAxis<Tag::Y>) ? (Tag::Y) :
			(i == IndexOfAxis<Tag::Z>) ? (Tag::Z) : static_cast<Tag>(static_cast<size_t>(-1));
			
			public: template<size_t i>
			using AxisAt = GetAxis< AxisAtIndex<i> >;
			
			public: template<Tag tag0, Tag tag1, Tag tag2>
			static constexpr bool REQUIRE_ORDER =
			(IndexOfAxis<tag0> == 0) && (IndexOfAxis<tag1> == 1) && (IndexOfAxis<tag2> == 2);
			
		};
		
		template<typename X, typename Y, typename Z, typename MetaInfo = AxisGroupMetaInfo<X, Y, Z>>
		struct VecBase:
			public MetaInfo::AxisAt<0>,
			public MetaInfo::AxisAt<1>,
			public MetaInfo::AxisAt<2>
		{
			
			protected:	using Meta = MetaInfo;
			
			private:	using Tag = typename Meta::Tag;
			
			protected:	using baseX = typename Meta::GetAxis<XYZ::X>;
			protected:	using baseY = typename Meta::GetAxis<XYZ::Y>;
			protected:	using baseZ = typename Meta::GetAxis<XYZ::Z>;
			
			protected:	using ctrX = typename Meta::value_type<XYZ::X>;
			protected:	using ctrY = typename Meta::value_type<XYZ::Y>;
			protected:	using ctrZ = typename Meta::value_type<XYZ::Z>;
			
			private:	using base0 = typename Meta::AxisAt<0>;
			private:	using base1 = typename Meta::AxisAt<1>;
			private:	using base2 = typename Meta::AxisAt<2>;
			
			public: constexpr
			VecBase()
			noexcept(noexcept(base0(), base1(), base2()))
			requires(std::default_initializable<base0> && std::default_initializable<base1> && std::default_initializable<base2>)
			: base0(), base1(), base2() {}
			
			public: constexpr
			VecBase(reference_to<ctrX> auto&& x, reference_to<ctrY> auto&& y, reference_to<ctrZ> auto&& z)
			noexcept(noexcept(base0(x), base1(y), base2(z)))
			requires(Meta::template REQUIRE_ORDER<Tag::X, Tag::Y, Tag::Z>)
			: base0(x), base1(y), base2(z) {}
			
			public: constexpr
			VecBase(reference_to<ctrX> auto&& x, reference_to<ctrY> auto&& y, reference_to<ctrZ> auto&& z)
			noexcept(noexcept(base0(x), base1(z), base2(y)))
			requires(Meta::template REQUIRE_ORDER<Tag::X, Tag::Z, Tag::Y>)
			: base0(x), base1(z), base2(y) {}
			
			public: constexpr
			VecBase(reference_to<ctrX> auto&& x, reference_to<ctrY> auto&& y, reference_to<ctrZ> auto&& z)
			noexcept(noexcept(base0(y), base1(x), base2(z)))
			requires(Meta::template REQUIRE_ORDER<Tag::Y, Tag::X, Tag::Z>)
			: base0(y), base1(x), base2(z) {}
			
			public: constexpr
			VecBase(reference_to<ctrX> auto&& x, reference_to<ctrY> auto&& y, reference_to<ctrZ> auto&& z)
			noexcept(noexcept(base0(y), base1(z), base2(x)))
			requires(Meta::template REQUIRE_ORDER<Tag::Y, Tag::Z, Tag::X>)
			: base0(y), base1(z), base2(x) {}
			
			public: constexpr
			VecBase(reference_to<ctrX> auto&& x, reference_to<ctrY> auto&& y, reference_to<ctrZ> auto&& z)
			noexcept(noexcept(base0(z), base1(x), base2(y)))
			requires(Meta::template REQUIRE_ORDER<Tag::Z, Tag::X, Tag::Y>)
			: base0(z), base1(x), base2(y) {}
			
			public: constexpr
			VecBase(reference_to<ctrX> auto&& x, reference_to<ctrY> auto&& y, reference_to<ctrZ> auto&& z)
			noexcept(noexcept(base0(z), base1(y), base2(x)))
			requires(Meta::template REQUIRE_ORDER<Tag::Z, Tag::Y, Tag::X>)
			: base0(z), base1(y), base2(x) {}
			
		};
		
		constexpr VecBase<int, float, char> b{1,2,3};
		
	}
}

#endif
