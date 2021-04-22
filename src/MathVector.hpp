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
		
		
		
		template<auto CMP> struct Switch {
			
			using cmp_t = decltype(CMP);
			
			template<cmp_t V, typename T> struct Case
			{ constexpr Case(cmp_t = V, T = T()) {} };
			
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
	
	namespace detail {
		
		enum class AxisTag { x , y , z };
		
		template<typename T, AxisTag at> struct Axis;
		
		template<typename T> requires(!std::same_as<T, void>)
		struct Axis<T, AxisTag::x>
		{
			T x;
			protected:
			auto& data() noexcept		{ return x; }
			auto& data() const noexcept	{ return x; }
		};
		
		template<typename T> requires(!std::same_as<T, void>)
		struct Axis<T, AxisTag::y>
		{
			T y;
			protected:
			auto& data() noexcept		{ return y; }
			auto& data() const noexcept	{ return y; }
		};
		
		template<typename T> requires(!std::same_as<T, void>)
		struct Axis<T, AxisTag::z>
		{
			T z;
			protected:
			auto& data() noexcept		{ return z; }
			auto& data() const noexcept	{ return z; }
		};
		
		template<AxisTag at>
		struct Axis<void, at>
		{ constexpr Axis(std::nullptr_t = nullptr) {} };
		
		template<typename T> using XAxis = Axis<T, AxisTag::x>;
		template<typename T> using YAxis = Axis<T, AxisTag::y>;
		template<typename T> using ZAxis = Axis<T, AxisTag::z>;
		
		template<typename X, typename Y, typename Z>
		class Vec:
		public	internal::AlignTypes_t<XAxis<X>, YAxis<Y>, ZAxis<Z>>::template get<0>,
		public	internal::AlignTypes_t<XAxis<X>, YAxis<Y>, ZAxis<Z>>::template get<1>,
		public	internal::AlignTypes_t<XAxis<X>, YAxis<Y>, ZAxis<Z>>::template get<2>
		{
			
			private: template<size_t Idx>
			using base = internal::AlignTypes_t<XAxis<X>, YAxis<Y>, ZAxis<Z>>::template get<Idx>;
			using base_x = Axis<X, AxisTag::x>;
			using base_y = Axis<Y, AxisTag::y>;
			using base_z = Axis<Z, AxisTag::z>;
			
			public: constexpr
			Vec() noexcept(noexcept(base<0>()) && noexcept(base<1>()) && noexcept(base<2>()))
			requires(std::default_initializable<base<0>> && std::default_initializable<base<1>> && std::default_initializable<base<2>>):
				base<0>(),
				base<1>(),
				base<2>()
			{}
			
			// public: constexpr
			// Vec()
			
		};
		
		void test() {
			
			constexpr
			Vec<int, char, void> f;
			
		}
		
	}
	
}

#endif
