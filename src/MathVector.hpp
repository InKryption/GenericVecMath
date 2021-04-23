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
			using get = std::tuple_element_t<ElmntIdx, std::tuple<T...>>;
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
	
	
	// // Namespace of lambdas that represent all of the "arithmetic" operators
	// namespace Operators {
	// 	
	// 	static constexpr auto
	// 	ADD				= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs + rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs + rhs; } { return lhs + rhs; };
	// 	
	// 	static constexpr auto
	// 	SUB				= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs - rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs - rhs; } { return lhs - rhs; };
	// 	
	// 	static constexpr auto
	// 	UNARY_MINUS		= [](auto&& v) constexpr noexcept(noexcept(-v)) -> decltype(auto)
	// 	requires requires(decltype(v) v) { -v; } { return -v; };
	// 	
	// 	static constexpr auto
	// 	UNARY_PLUS		= [](auto&& v) constexpr noexcept(noexcept(+v)) -> decltype(auto)
	// 	requires requires(decltype(v) v) { +v; } { return +v; };
	// 	
	// 	static constexpr auto
	// 	MUL				= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs * rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs * rhs; } { return lhs * rhs; };
	// 	
	// 	static constexpr auto
	// 	DIV				= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs / rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs / rhs; } { return lhs / rhs; };
	// 	
	// 	static constexpr auto
	// 	ADDRESS_OF		= [](auto&& v) constexpr noexcept(noexcept(&v)) -> decltype(auto)
	// 	requires requires(decltype(v) v) { &v; } { return &v; };
	// 	
	// 	static constexpr auto
	// 	DEREF			= [](auto&& v) constexpr noexcept(noexcept(*v)) -> decltype(auto)
	// 	requires requires(decltype(v) v) { *v; } { return *v; };
	// 	
	// 	static constexpr auto
	// 	BIT_AND			= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs & rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs & rhs; } { return lhs & rhs; };
	// 	
	// 	static constexpr auto
	// 	BIT_XOR			= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs ^ rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs ^ rhs; } { return lhs ^ rhs; };
	// 	
	// 	static constexpr auto
	// 	BIT_OR			= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs | rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs | rhs; } { return lhs | rhs; };
	// 	
	// 	static constexpr auto
	// 	BIT_NOT			= [](auto&& v) constexpr noexcept(noexcept(~v)) -> decltype(auto)
	// 	requires requires(decltype(v) v) { ~v; } { return ~v; };
	// 	
	// 	static constexpr auto
	// 	LOGICAL_AND		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs && rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs && rhs; } { return lhs && rhs; };
	// 	
	// 	static constexpr auto
	// 	LOGICAL_OR		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs || rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs || rhs; } { return lhs || rhs; };
	// 	
	// 	static constexpr auto
	// 	CMP_LT			= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs < rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs < rhs; } { return lhs < rhs; };
	// 	
	// 	static constexpr auto
	// 	CMP_GT			= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs > rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs > rhs; } { return lhs > rhs; };
	// 	
	// 	static constexpr auto
	// 	CMP_LT_EQ		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs <= rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs <= rhs; } { return lhs <= rhs; };
	// 	
	// 	static constexpr auto
	// 	CMP_GT_EQ		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs >= rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs >= rhs; } { return lhs >= rhs; };
	// 	
	// 	static constexpr auto
	// 	CMP_EQ			= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs == rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs == rhs}; } { return lhs == rhs; };
	// 	
	// 	static constexpr auto
	// 	CMP_NEQ			= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs != rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs != rhs}; } { return lhs != rhs; };
	// 	
	// 	static constexpr auto
	// 	LOGICAL_NOT		= [](auto&& v) constexpr noexcept(noexcept(!v)) -> decltype(auto)
	// 	requires requires(decltype(v) v) { !v; } { return !v; };
	// 	
	// 	static constexpr auto
	// 	MOD				= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs % rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs % rhs; } { return lhs % rhs; };
	// 	
	// 	static constexpr auto
	// 	SBL				= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs << rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs << rhs; } { return lhs << rhs; };
	// 	
	// 	static constexpr auto
	// 	SBR				= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs >> rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs >> rhs; } { return lhs >> rhs; };
	// 	
	// 	static constexpr auto
	// 	POSTFIX_INC		= [](auto&& v) constexpr noexcept(noexcept(v++)) -> decltype(auto)
	// 	requires requires(decltype(v) v) { v++; } { return v++; };
	// 	
	// 	static constexpr auto
	// 	POSTFIX_DEC		= [](auto&& v) constexpr noexcept(noexcept(v--)) -> decltype(auto)
	// 	requires requires(decltype(v) v) { v--; } { return v--; };
	// 	
	// 	static constexpr auto
	// 	PREFIX_INC		= [](auto&& v) constexpr noexcept(noexcept(++v)) -> decltype(auto)
	// 	requires requires(decltype(v) v) { ++v; } { return ++v; }; 
	// 	
	// 	static constexpr auto
	// 	PREFIX_DEC		= [](auto&& v) constexpr noexcept(noexcept(--v)) -> decltype(auto)
	// 	requires requires(decltype(v) v) { --v; } { return --v; };
	// 	
	// 	static constexpr auto
	// 	FUNC_CALL		= [](auto&& f, auto&&... args) constexpr noexcept(noexcept(f(std::forward<decltype(args)>(args)...))) -> decltype(auto)
	// 	requires requires(decltype(f) f, decltype(args)... args) { f(std::forward<decltype(args)>(args)...); } { return f(std::forward<decltype(args)>(args)...); };
	// 	
	// 	static constexpr auto
	// 	SUBSCRIPT		= [](auto&& obj, auto&& i) constexpr noexcept(requires(decltype(obj) obj, decltype(i) i) { {obj[i]} noexcept; }) -> decltype(auto)
	// 	requires requires(decltype(obj) obj, decltype(i) i) { obj[i]; } { return obj[i]; };
	// 	
	// 	static constexpr auto
	// 	ASSIGN_EQ		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs = rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs = rhs}; } { return lhs = rhs; };
	// 	
	// 	static constexpr auto
	// 	ASSIGN_MUL		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs *= rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs *= rhs}; } { return lhs *= rhs; };
	// 	
	// 	static constexpr auto
	// 	ASSIGN_DIV		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs /= rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs /= rhs}; } { return lhs /= rhs; };
	// 	
	// 	static constexpr auto
	// 	ASSIGN_MOD		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs %= rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs %= rhs}; } { return lhs %= rhs; };
	// 	
	// 	static constexpr auto
	// 	ASSIGN_ADD		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs += rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs += rhs}; } { return lhs += rhs; };
	// 	
	// 	static constexpr auto
	// 	ASSIGN_SUB		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs -= rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs -= rhs}; } { return lhs -= rhs; };
	// 	
	// 	static constexpr auto
	// 	ASSIGN_SBR		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs >>= rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs >>= rhs}; } { return lhs >>= rhs; };
	// 	
	// 	static constexpr auto
	// 	ASSIGN_SBL		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs <<= rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs <<= rhs}; } { return lhs <<= rhs; };
	// 	
	// 	static constexpr auto
	// 	ASSIGN_BIT_AND	= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs &= rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs &= rhs}; } { return lhs &= rhs; };
	// 	
	// 	static constexpr auto
	// 	ASSIGN_BIT_OR	= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs |= rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs |= rhs}; } { return lhs |= rhs; };
	// 	
	// 	static constexpr auto
	// 	ASSIGN_BIT_XOR	= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs ^= rhs)) -> decltype(auto)
	// 	requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs ^= rhs}; } { return lhs ^= rhs; };
	// 	
	// }
	// 
	
	// 
	// namespace detail_2 {
	// 	
	// 	enum class AxisTag { x , y , z };
	// 	
	// 	template<typename T, AxisTag at> struct Axis;
	// 	
	// 	template<typename T> requires(!std::same_as<T, void>)
	// 	struct Axis<T, AxisTag::x>
	// 	{
	// 		T x;
	// 		protected:
	// 		auto& data() noexcept		{ return x; }
	// 		auto& data() const noexcept	{ return x; }
	// 	};
	// 	
	// 	template<typename T> requires(!std::same_as<T, void>)
	// 	struct Axis<T, AxisTag::y>
	// 	{
	// 		T y;
	// 		protected:
	// 		auto& data() noexcept		{ return y; }
	// 		auto& data() const noexcept	{ return y; }
	// 	};
	// 	
	// 	template<typename T> requires(!std::same_as<T, void>)
	// 	struct Axis<T, AxisTag::z>
	// 	{
	// 		T z;
	// 		protected:
	// 		auto& data() noexcept		{ return z; }
	// 		auto& data() const noexcept	{ return z; }
	// 	};
	// 	
	// 	template<AxisTag at>
	// 	struct Axis<void, at>
	// 	{ constexpr Axis(std::nullptr_t = nullptr) {} };
	// 	
	// 	template<typename T> using XAxis = Axis<T, AxisTag::x>;
	// 	template<typename T> using YAxis = Axis<T, AxisTag::y>;
	// 	template<typename T> using ZAxis = Axis<T, AxisTag::z>;
	// 	
	// 	template<typename X, typename Y, typename Z>
	// 	class Vec:
	// 	public	internal::AlignTypes_t<XAxis<X>, YAxis<Y>, ZAxis<Z>>::template get<0>,
	// 	public	internal::AlignTypes_t<XAxis<X>, YAxis<Y>, ZAxis<Z>>::template get<1>,
	// 	public	internal::AlignTypes_t<XAxis<X>, YAxis<Y>, ZAxis<Z>>::template get<2>
	// 	{
	// 		
	// 		private: template<size_t Idx>
	// 		using base = internal::AlignTypes_t<XAxis<X>, YAxis<Y>, ZAxis<Z>>::template get<Idx>;
	// 		using base_x = Axis<X, AxisTag::x>;
	// 		using base_y = Axis<Y, AxisTag::y>;
	// 		using base_z = Axis<Z, AxisTag::z>;
	// 		
	// 		public: constexpr
	// 		Vec() noexcept(noexcept(base<0>()) && noexcept(base<1>()) && noexcept(base<2>()))
	// 		requires(std::default_initializable<base<0>> && std::default_initializable<base<1>> && std::default_initializable<base<2>>):
	// 			base<0>(),
	// 			base<1>(),
	// 			base<2>()
	// 		{}
	// 		
	// 		// public: constexpr
	// 		// Vec()
	// 		
	// 	};
	// 	
	// 	void test() {
	// 		
	// 		constexpr
	// 		Vec<int, char, void> f;
	// 		
	// 	}
	// 	
	// }
	// 
	
	namespace detail {
		
		template<typename T>
		concept non_void = (!std::same_as<void, T>);
		
		enum class XYZ { X , Y , Z };
		template<typename T, XYZ tag> struct AxisBase;
		
		template<non_void T> struct AxisBase<T, XYZ::X> { using ctr_arg = T; T x; };
		template<non_void T> struct AxisBase<T, XYZ::Y> { using ctr_arg = T; T y; };
		template<non_void T> struct AxisBase<T, XYZ::Z> { using ctr_arg = T; T z; };
		
		template<XYZ tag> struct AxisBase<void, tag> {
			constexpr AxisBase(std::nullptr_t = nullptr) noexcept {}
			constexpr operator std::nullptr_t() noexcept { return nullptr; }
			
			protected:
			using value_type = void;
			using ctr_arg = std::nullptr_t;
		};
		
		
		
		template<typename T, XYZ tag>
		struct Axis:
		public AxisBase<T, tag> {
			protected: using
			base = AxisBase<T, tag>;
			
			protected: using
			value_type = T;
			
			protected: using
			ctr_arg = typename base::ctr_arg;
			
			private: template<typename... Arg> static constexpr bool
			NOEXCEPT_CTR = noexcept(base{std::forward<Arg>(std::declval<Arg>())...});
			
			protected: constexpr decltype(auto)
			get() noexcept
			{
				if constexpr(std::same_as<void, T>) return *static_cast<base*>(this);
				else {
					if constexpr(tag == XYZ::X) return *std::addressof(this->x);
					if constexpr(tag == XYZ::Y) return *std::addressof(this->y);
					if constexpr(tag == XYZ::Z) return *std::addressof(this->z);
				}
			}
			
			protected: constexpr decltype(auto)
			get() const noexcept
			{
				
				if constexpr(std::same_as<void, T>) return *static_cast<base const*>(this);
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
			
		};
		
		template<typename T> using XAxis = Axis<T, XYZ::X>;
		template<typename T> using YAxis = Axis<T, XYZ::Y>;
		template<typename T> using ZAxis = Axis<T, XYZ::Z>;
		
		template<size_t Which, typename X, typename Y, typename Z>
		using AxisByAlignment = internal::AlignTypes_t<XAxis<X>, YAxis<Y>, ZAxis<Z>>::template get<Which>;
		
		template<typename X, typename Y, typename Z>
		class Vec: public
			AxisByAlignment<0, X, Y, Z>,
			AxisByAlignment<1, X, Y, Z>,
			AxisByAlignment<2, X, Y, Z>
		{
			private: template<size_t Idx>
			using base_bo = AxisByAlignment<Idx, X, Y, Z>;
			
			private: template<XYZ tag>
			using base_bt = internal::
			Switch_t<tag,
				typename internal::Switch<>::	Case<(XYZ::X),	Axis<X, XYZ::X> > ,
				typename internal::Switch<>::	Case<(XYZ::Y),	Axis<Y, XYZ::Y> > ,
				typename internal::Switch<>::	Case<(XYZ::Z),	Axis<Z, XYZ::Z> >
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
			
			private: template<typename Non_Void1, typename Non_Void2, typename Discard> static constexpr bool
			DISCARD_ONE = (!std::same_as<Non_Void1,void>) && (!std::same_as<Non_Void2,void>) && (std::same_as<Discard,void>);
			
			private: template<typename Non_Void, typename Discard1, typename Discard2> static constexpr bool
			USE_ONLY_ONE = (!std::same_as<Non_Void,void>) && (std::same_as<Discard1,void>) && (std::same_as<Discard2,void>);
			
			/* Default constructor */
			
			public: constexpr
			Vec()
				noexcept( NOEXCEPT_CTR<base_X> && NOEXCEPT_CTR<base_Y> && NOEXCEPT_CTR<base_Z> )
				requires( std::default_initializable<base_X> && std::default_initializable<base_Y> && std::default_initializable<base_Z> ):
				base_bo<0>(), base_bo<1>(), base_bo<2>()
			{}
			
			/* All Three Arguments passed*/
			
			public: constexpr
			Vec(std::convertible_to<typename base_X::ctr_arg> auto&& x,
				std::convertible_to<typename base_Y::ctr_arg> auto&& y,
				std::convertible_to<typename base_Z::ctr_arg> auto&& z
			)	
				noexcept( NOEXCEPT_CTR<base_X, decltype(x)> && NOEXCEPT_CTR<base_Y, decltype(y)> && NOEXCEPT_CTR<base_Z, decltype(z)> ):
				base_bo<0>( forward_param<0>(std::forward<decltype(x)>(x), std::forward<decltype(y)>(y), std::forward<decltype(z)>(z)) ),
				base_bo<1>( forward_param<1>(std::forward<decltype(x)>(x), std::forward<decltype(y)>(y), std::forward<decltype(z)>(z)) ),
				base_bo<2>( forward_param<2>(std::forward<decltype(x)>(x), std::forward<decltype(y)>(y), std::forward<decltype(z)>(z)) )
			{}
			
			/* Only Two Arguments required */
			
			public: constexpr
			Vec(std::convertible_to<typename base_X::ctr_arg> auto&& x, std::convertible_to<typename base_Y::ctr_arg> auto&& y)
				noexcept( NOEXCEPT_CTR<base_X, decltype(x)> && NOEXCEPT_CTR<base_Y, decltype(y)> )
				requires( DISCARD_ONE<X, Y, Z> ):
				base_bo<0>( forward_param<0>(std::forward<decltype(x)>(x), std::forward<decltype(y)>(y), nullptr) ),
				base_bo<1>( forward_param<1>(std::forward<decltype(x)>(x), std::forward<decltype(y)>(y), nullptr) ),
				base_bo<2>( forward_param<2>(std::forward<decltype(x)>(x), std::forward<decltype(y)>(y), nullptr) )
			{}
			
			public: constexpr
			Vec(std::convertible_to<typename base_X::ctr_arg> auto&& x, std::convertible_to<typename base_Z::ctr_arg> auto&& z)
				noexcept( NOEXCEPT_CTR<base_X, decltype(x)> && NOEXCEPT_CTR<base_Z, decltype(z)> )
				requires( DISCARD_ONE<X, Z, Y>):
				base_bo<0>( forward_param<0>(std::forward<decltype(x)>(x), nullptr, std::forward<decltype(z)>(z)) ),
				base_bo<1>( forward_param<1>(std::forward<decltype(x)>(x), nullptr, std::forward<decltype(z)>(z)) ),
				base_bo<2>( forward_param<2>(std::forward<decltype(x)>(x), nullptr, std::forward<decltype(z)>(z)) )
			{}
			
			public: constexpr
			Vec(std::convertible_to<typename base_Y::ctr_arg> auto&& y, std::convertible_to<typename base_Z::ctr_arg> auto&& z)
				noexcept( NOEXCEPT_CTR<base_Y, decltype(y)> && NOEXCEPT_CTR<base_Z, decltype(z)> )
				requires( DISCARD_ONE<Y, Z, X>):
				base_bo<0>( forward_param<0>(nullptr, std::forward<decltype(y)>(y), std::forward<decltype(z)>(z)) ),
				base_bo<1>( forward_param<1>(nullptr, std::forward<decltype(y)>(y), std::forward<decltype(z)>(z)) ),
				base_bo<2>( forward_param<2>(nullptr, std::forward<decltype(y)>(y), std::forward<decltype(z)>(z)) )
			{}
			
			/* Only One Argument required */
			
			public: constexpr
			Vec(std::convertible_to<typename base_bt<XYZ::X>::ctr_arg> auto&& x)
				noexcept( NOEXCEPT_CTR<base_bt<XYZ::X>, decltype(x)> )
				requires( USE_ONLY_ONE<X, Y, Z> ):
				base_bt<XYZ::X>( std::forward<decltype(x)>(x) )
			{}
			
			public: constexpr
			Vec(std::convertible_to<typename base_bt<XYZ::Y>::ctr_arg> auto&& y)
				noexcept( NOEXCEPT_CTR<base_bt<XYZ::Y>, decltype(y)> )
				requires( USE_ONLY_ONE<Y, X, Z> ):
				base_bt<XYZ::Z>( std::forward<decltype(y)>(y) )
			{}
			
			public: constexpr
			Vec(std::convertible_to<typename base_bt<XYZ::Z>::ctr_arg> auto&& z)
				noexcept( NOEXCEPT_CTR<base_bt<XYZ::Z>, decltype(z)> )
				requires( USE_ONLY_ONE<Z, X, Y> ):
				base_bt<XYZ::Z>( std::forward<decltype(z)>(z) )
			{}
			
			
			
			public: template<size_t Idx> friend constexpr decltype(auto)
			get(Vec& v) {
				if constexpr(Idx == 0) return (&v)->base_X::get();
				if constexpr(Idx == 1) return (&v)->base_Y::get();
				if constexpr(Idx == 2) return (&v)->base_Z::get();
			}
			
			public: template<size_t Idx> friend constexpr decltype(auto)
			get(Vec&& v) {
				if constexpr(Idx == 0) return (&v)->base_X::get();
				if constexpr(Idx == 1) return (&v)->base_Y::get();
				if constexpr(Idx == 2) return (&v)->base_Z::get();
			}
			
			public: template<size_t Idx> friend constexpr decltype(auto)
			get(Vec const& v) {
				if constexpr(Idx == 0) return (&v)->base_X::get();
				if constexpr(Idx == 1) return (&v)->base_Y::get();
				if constexpr(Idx == 2) return (&v)->base_Z::get();
			}
			
		};
		
		void test() {
			
			
			
		}
		
	}
	
}

#endif
