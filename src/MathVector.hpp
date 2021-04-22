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
	
	// Namespace of lambdas that represent all of the "arithmetic" operators
	namespace Operators {
		
		static constexpr auto
		ADD				= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs + rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs + rhs; } { return lhs + rhs; };
		
		static constexpr auto
		SUB				= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs - rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs - rhs; } { return lhs - rhs; };
		
		static constexpr auto
		UNARY_MINUS		= [](auto&& v) constexpr noexcept(noexcept(-v)) -> decltype(auto)
		requires requires(decltype(v) v) { -v; } { return -v; };
		
		static constexpr auto
		UNARY_PLUS		= [](auto&& v) constexpr noexcept(noexcept(+v)) -> decltype(auto)
		requires requires(decltype(v) v) { +v; } { return +v; };
		
		static constexpr auto
		MUL				= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs * rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs * rhs; } { return lhs * rhs; };
		
		static constexpr auto
		DIV				= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs / rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs / rhs; } { return lhs / rhs; };
		
		static constexpr auto
		ADDRESS_OF		= [](auto&& v) constexpr noexcept(noexcept(&v)) -> decltype(auto)
		requires requires(decltype(v) v) { &v; } { return &v; };
		
		static constexpr auto
		DEREF			= [](auto&& v) constexpr noexcept(noexcept(*v)) -> decltype(auto)
		requires requires(decltype(v) v) { *v; } { return *v; };
		
		static constexpr auto
		BIT_AND			= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs & rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs & rhs; } { return lhs & rhs; };
		
		static constexpr auto
		BIT_XOR			= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs ^ rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs ^ rhs; } { return lhs ^ rhs; };
		
		static constexpr auto
		BIT_OR			= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs | rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs | rhs; } { return lhs | rhs; };
		
		static constexpr auto
		BIT_NOT			= [](auto&& v) constexpr noexcept(noexcept(~v)) -> decltype(auto)
		requires requires(decltype(v) v) { ~v; } { return ~v; };
		
		static constexpr auto
		LOGICAL_AND		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs && rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs && rhs; } { return lhs && rhs; };
		
		static constexpr auto
		LOGICAL_OR		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs || rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs || rhs; } { return lhs || rhs; };
		
		static constexpr auto
		CMP_LT			= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs < rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs < rhs; } { return lhs < rhs; };
		
		static constexpr auto
		CMP_GT			= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs > rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs > rhs; } { return lhs > rhs; };
		
		static constexpr auto
		CMP_LT_EQ		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs <= rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs <= rhs; } { return lhs <= rhs; };
		
		static constexpr auto
		CMP_GT_EQ		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs >= rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs >= rhs; } { return lhs >= rhs; };
		
		static constexpr auto
		CMP_EQ			= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs == rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs == rhs}; } { return lhs == rhs; };
		
		static constexpr auto
		CMP_NEQ			= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs != rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs != rhs}; } { return lhs != rhs; };
		
		static constexpr auto
		LOGICAL_NOT		= [](auto&& v) constexpr noexcept(noexcept(!v)) -> decltype(auto)
		requires requires(decltype(v) v) { !v; } { return !v; };
		
		static constexpr auto
		MOD				= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs % rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs % rhs; } { return lhs % rhs; };
		
		static constexpr auto
		SBL				= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs << rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs << rhs; } { return lhs << rhs; };
		
		static constexpr auto
		SBR				= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs >> rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { lhs >> rhs; } { return lhs >> rhs; };
		
		static constexpr auto
		POSTFIX_INC		= [](auto&& v) constexpr noexcept(noexcept(v++)) -> decltype(auto)
		requires requires(decltype(v) v) { v++; } { return v++; };
		
		static constexpr auto
		POSTFIX_DEC		= [](auto&& v) constexpr noexcept(noexcept(v--)) -> decltype(auto)
		requires requires(decltype(v) v) { v--; } { return v--; };
		
		static constexpr auto
		PREFIX_INC		= [](auto&& v) constexpr noexcept(noexcept(++v)) -> decltype(auto)
		requires requires(decltype(v) v) { ++v; } { return ++v; }; 
		
		static constexpr auto
		PREFIX_DEC		= [](auto&& v) constexpr noexcept(noexcept(--v)) -> decltype(auto)
		requires requires(decltype(v) v) { --v; } { return --v; };
		
		static constexpr auto
		FUNC_CALL		= [](auto&& f, auto&&... args) constexpr noexcept(noexcept(f(std::forward<decltype(args)>(args)...))) -> decltype(auto)
		requires requires(decltype(f) f, decltype(args)... args) { f(std::forward<decltype(args)>(args)...); } { return f(std::forward<decltype(args)>(args)...); };
		
		static constexpr auto
		SUBSCRIPT		= [](auto&& obj, auto&& i) constexpr noexcept(requires(decltype(obj) obj, decltype(i) i) { {obj[i]} noexcept; }) -> decltype(auto)
		requires requires(decltype(obj) obj, decltype(i) i) { obj[i]; } { return obj[i]; };
		
		static constexpr auto
		ASSIGN_EQ		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs = rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs = rhs}; } { return lhs = rhs; };
		
		static constexpr auto
		ASSIGN_MUL		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs *= rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs *= rhs}; } { return lhs *= rhs; };
		
		static constexpr auto
		ASSIGN_DIV		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs /= rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs /= rhs}; } { return lhs /= rhs; };
		
		static constexpr auto
		ASSIGN_MOD		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs %= rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs %= rhs}; } { return lhs %= rhs; };
		
		static constexpr auto
		ASSIGN_ADD		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs += rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs += rhs}; } { return lhs += rhs; };
		
		static constexpr auto
		ASSIGN_SUB		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs -= rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs -= rhs}; } { return lhs -= rhs; };
		
		static constexpr auto
		ASSIGN_SBR		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs >>= rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs >>= rhs}; } { return lhs >>= rhs; };
		
		static constexpr auto
		ASSIGN_SBL		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs <<= rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs <<= rhs}; } { return lhs <<= rhs; };
		
		static constexpr auto
		ASSIGN_BIT_AND	= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs &= rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs &= rhs}; } { return lhs &= rhs; };
		
		static constexpr auto
		ASSIGN_BIT_OR	= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs |= rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs |= rhs}; } { return lhs |= rhs; };
		
		static constexpr auto
		ASSIGN_BIT_XOR	= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs ^= rhs)) -> decltype(auto)
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs) { {lhs ^= rhs}; } { return lhs ^= rhs; };
		
	}
	
	
	namespace detail {
		
		enum class AxisTag { x , y , z };
		
		template<typename T> struct XAxis
		{
			auto& data() noexcept		{ return x; }
			auto& data() const noexcept	{ return x; }
			T x;
		};
		
		template<typename T> struct YAxis
		{
			auto& data() noexcept		{ return y; }
			auto& data() const noexcept	{ return y; }
			T y;
		};
		
		template<typename T> struct ZAxis
		{
			auto& data() noexcept		{ return z; }
			auto& data() const noexcept	{ return z; }
			T z;
		};
		
		template<typename T, AxisTag tag> using AxisBase =
		typename internal::Switch<tag>::template In<
			typename internal::Switch<tag>:: Case<AxisTag::x, XAxis<T>>,
			typename internal::Switch<tag>:: Case<AxisTag::y, YAxis<T>>,
			typename internal::Switch<tag>:: Case<AxisTag::z, ZAxis<T>>
		>;
		
		AxisBase<int, AxisTag::x> f;
		template<typename T, AxisTag tag>
		class Axis;
		
		template<typename T, AxisTag tag>
		requires(!std::same_as<void,T>)
		class Axis<T, tag>: public AxisBase<T, tag> {
			private:
			using base = AxisBase<T, tag>;
			
			protected:
			using
			value_type = T;					
			
			using
			ctr_arg = value_type;
			
			static constexpr bool
			is_void = false;
			
			constexpr auto&
			data() const noexcept
			{ return static_cast<base const*>(this)->data(); }
			
			constexpr auto&
			data() noexcept
			{ return static_cast<base const*>(this)->data();; }
						
			public:
			constexpr
			Axis() noexcept(noexcept(T{})) requires(std::default_initializable<T>):
			base{} {}
			
			constexpr
			Axis(std::convertible_to<T> auto&& v) noexcept(noexcept(base{v})):
			base{v} {}
			
			template<typename U> friend constexpr decltype(auto)
			operator+(Axis&& lhs, Axis<U, tag>&& rhs) noexcept(noexcept(Operators::ADD(lhs, rhs)))
			{ return Operators::ADD(lhs, rhs); }
			
			template<typename U> friend constexpr decltype(auto)
			operator-(Axis&& lhs, Axis<U, tag>&& rhs) noexcept(noexcept(Operators::SUB(lhs, rhs)))
			{ return Operators::SUB(lhs, rhs); }
			
		};
		
		// Empty / Void axis; empty value.
		template<AxisTag tag>
		class Axis<void, tag> {
			public:
			constexpr Axis(std::nullptr_t = nullptr) noexcept {}
			
			template<typename U> friend constexpr decltype(auto)
			operator+(Axis&& lhs, Axis<U, tag>&& rhs) noexcept(noexcept(+rhs.data()))
			requires requires(decltype(rhs) rhs) { {+rhs.data()}; }
			{ return Axis<U, tag>(+rhs.data()); }
			
			template<typename U> friend constexpr decltype(auto)
			operator-(Axis&& lhs, Axis<U, tag>&& rhs) noexcept(noexcept(-rhs.data()))
			requires requires(decltype(rhs) rhs) { {-rhs.data()}; }
			{ return Axis<U, tag>(-rhs.data()); }
			
			protected:
			using value_type = void;
			using ctr_arg = std::nullptr_t;
			static constexpr bool is_void = true;
			constexpr auto& data() const	noexcept { return *this; }
			constexpr auto& data()			noexcept { return *this; }
		};
		
		
		
		
		template<typename A1, typename A2, typename A3>
		struct Axies;
		
		template<typename T1, AxisTag t1, typename T2, AxisTag t2, typename T3, AxisTag t3>
		struct Axies
				<	Axis<T1, t1> , Axis<T2, t2> , Axis<T3, t3>	>:
		public		Axis<T1, t1> , Axis<T2, t2> , Axis<T3, t3>
		{
			
			protected:
			template<AxisTag at> static constexpr auto&
			raw_get_axis(Axies* axies) noexcept {
				if constexpr(at == t1) return *static_cast<Axis<T1, t1>*>(axies);
				if constexpr(at == t2) return *static_cast<Axis<T2, t2>*>(axies);
				if constexpr(at == t3) return *static_cast<Axis<T3, t3>*>(axies);
			}
			
			template<AxisTag at> static constexpr auto const&
			raw_get_axis(Axies const* axies) noexcept
			{ return const_cast<Axies*>(axies)->template raw_get_axis<at>(); }
			
			template<AxisTag at> using
			get_axis_t = typename internal::Switch<at>::template In<
				typename internal::Switch<at>::Case<t1, Axis<T1, t1>>,
				typename internal::Switch<at>::Case<t2, Axis<T2, t2>>,
				typename internal::Switch<at>::Case<t3, Axis<T3, t3>>
			>;
			
			using Axis1 = get_axis_t<t1>;
			using Axis2 = get_axis_t<t2>;
			using Axis3 = get_axis_t<t3>;
			
			using AxisX = get_axis_t<AxisTag::x>;
			using AxisY = get_axis_t<AxisTag::y>;
			using AxisZ = get_axis_t<AxisTag::z>;
			
			template<AxisTag at> static constexpr auto&&
			forward_correct_axis(
				std::convertible_to<typename AxisX::ctr_arg> auto&& x,
				std::convertible_to<typename AxisY::ctr_arg> auto&& y,
				std::convertible_to<typename AxisZ::ctr_arg> auto&& z
			) noexcept {
				if constexpr(at == AxisTag::x) return x;
				if constexpr(at == AxisTag::y) return y;
				if constexpr(at == AxisTag::z) return z;
			}
			
			public:
			constexpr
			Axies() noexcept
			( noexcept(Axis1()) && noexcept(Axis2()) && noexcept(Axis3()) )
			requires(	std::default_initializable<Axis1>
			&&			std::default_initializable<Axis2>
			&&			std::default_initializable<Axis3>
			):
				Axis1(),
				Axis2(),
				Axis3()
			{}
			
			constexpr
			Axies(	std::convertible_to<typename AxisX::ctr_arg> auto&& x,
					std::convertible_to<typename AxisY::ctr_arg> auto&& y,
					std::convertible_to<typename AxisZ::ctr_arg> auto&& z)
			noexcept(	noexcept(AxisX(x))	&&	noexcept(AxisY(y))	&&	noexcept(AxisZ(z))	)
			requires(requires(decltype(x) xarg, decltype(y) yarg, decltype(z) zarg) { AxisX(xarg); AxisY(yarg); AxisZ(zarg); }):
				Axis1(forward_correct_axis<t1>(x, y, z)),
				Axis2(forward_correct_axis<t2>(x, y, z)),
				Axis3(forward_correct_axis<t3>(x, y, z))
			{}
			
			constexpr
			Axies(	std::convertible_to<typename AxisX::ctr_arg> auto&& x,
					std::convertible_to<typename AxisY::ctr_arg> auto&& y)
			noexcept(	noexcept(AxisX(x))	&&	noexcept(AxisY(y))	)
			requires(requires(decltype(x) xarg, decltype(y) yarg) { AxisX(xarg); AxisY(yarg); } && !AxisX::is_void && !AxisY::is_void && AxisZ::is_void):
				Axis1(forward_correct_axis<t1>(x, y, nullptr)),
				Axis2(forward_correct_axis<t2>(x, y, nullptr)),
				Axis3(forward_correct_axis<t3>(x, y, nullptr))
			{}
			
			constexpr
			Axies(	std::convertible_to<typename AxisX::ctr_arg> auto&& x,
					std::convertible_to<typename AxisZ::ctr_arg> auto&& z)
			noexcept(	noexcept(AxisX(x))	&&	noexcept(AxisZ(z))	)
			requires(requires(decltype(x) xarg, decltype(z) zarg) { AxisX(xarg); AxisZ(zarg); } && !AxisX::is_void && AxisY::is_void && !AxisZ::is_void):
				Axis1(forward_correct_axis<t1>(x, nullptr, z)),
				Axis2(forward_correct_axis<t2>(x, nullptr, z)),
				Axis3(forward_correct_axis<t3>(x, nullptr, z))
			{}
			
			constexpr
			Axies(	std::convertible_to<typename AxisY::ctr_arg> auto&& y,
					std::convertible_to<typename AxisZ::ctr_arg> auto&& z)
			noexcept(	noexcept(AxisY(y))	&&	noexcept(AxisZ(z))	)
			requires(requires(decltype(y) yarg, decltype(z) zarg) { AxisY(yarg); AxisZ(zarg); } && AxisX::is_void && !AxisY::is_void && !AxisZ::is_void):
				Axis1(forward_correct_axis<t1>(nullptr, y, z)),
				Axis2(forward_correct_axis<t2>(nullptr, y, z)),
				Axis3(forward_correct_axis<t3>(nullptr, y, z))
			{}
			
			constexpr
			Axies(std::convertible_to<typename AxisX::ctr_arg> auto&& x)
			noexcept( noexcept(AxisX(x)) )
			requires(requires(decltype(x) xarg) { AxisX(xarg); } && !AxisX::is_void && AxisY::is_void && AxisZ::is_void ):
				AxisX(x)
			{}
			
			constexpr
			Axies(std::convertible_to<typename AxisY::ctr_arg> auto&& y)
			noexcept( noexcept(AxisY(y)) )
			requires(requires(decltype(y) yarg) { AxisX(yarg); } && AxisX::is_void && !AxisY::is_void && AxisZ::is_void ):
				AxisY(y)
			{}
			
			constexpr
			Axies(std::convertible_to<typename AxisZ::ctr_arg> auto&& z)
			noexcept( noexcept(AxisZ(z)) )
			requires(requires(decltype(z) zarg) { AxisZ(zarg); } && AxisX::is_void && AxisY::is_void && !AxisZ::is_void ):
				AxisZ(z)
			{}
			
			constexpr
			Axies([[maybe_unused]] std::nullptr_t x, [[maybe_unused]] std::nullptr_t y = nullptr, [[maybe_unused]] std::nullptr_t z = nullptr) noexcept
			requires(AxisX::is_void && AxisY::is_void && AxisZ::is_void) {}
			
		};
		
		template<typename X, typename Y = X, typename Z = Y>
		using Vec = internal::AlignTypes_t<Axis<X, AxisTag::x>, Axis<Y, AxisTag::y>, Axis<Z, AxisTag::z>>::template rebind<Axies>;
		
	}
	
}

#endif
