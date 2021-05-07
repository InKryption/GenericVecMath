#include <stdio.h>
#include <cstdlib>
#include <cstdint>
#include <concepts>
#include <numbers>
#include <numeric>
#include <tuple>
#include <memory>
#include <type_traits>
#include <compare>
#include <cmath>
#include <string>

namespace ink {
	
	namespace concepts {
		
		template<typename T, typename U>
		struct same_template_t: std::false_type {};
		
		template<template<typename...> typename Template, typename... T, typename... U>
		struct same_template_t<Template<T...>, Template<U...>>: std::true_type {};
		
		template<typename T, typename U>
		concept same_template = same_template_t<T, U>::value;
		
		
		
		template<typename T, bool MustBeNoexcept = false>
		struct can_postfix_inc_t:
		std::bool_constant<(requires(T t) { {t++}; } && !MustBeNoexcept) || requires(T t) { {t++} noexcept; }> {};
		
		template<typename T, bool MustBeNoexcept = false>
		concept can_postfix_inc = can_postfix_inc_t<T, MustBeNoexcept>::value;
		
		
		
		template<typename T, bool MustBeNoexcept = false>
		struct can_postfix_dec_t:
		std::bool_constant<(requires(T t) { {t--}; } && !MustBeNoexcept) || requires(T t) { {t--} noexcept; }> {};
		
		template<typename T, bool MustBeNoexcept = false>
		concept can_postfix_dec = can_postfix_dec_t<T, MustBeNoexcept>::value;
		
		
		
		template<typename T, bool MustBeNoexcept = false>
		struct can_prefix_inc_t:
		std::bool_constant<(requires(T t) { {++t}; } && !MustBeNoexcept) || requires(T t) { {++t} noexcept; }> {};
		
		template<typename T, bool MustBeNoexcept = false>
		concept can_prefix_inc = can_prefix_inc_t<T, MustBeNoexcept>::value;
		
		
		
		template<typename T, bool MustBeNoexcept = false>
		struct can_prefix_dec_t:
		std::bool_constant<(requires(T t) { {--t}; } && !MustBeNoexcept) || requires(T t) { {--t} noexcept; }> {};
		
		template<typename T, bool MustBeNoexcept = false>
		concept can_prefix_dec = can_prefix_dec_t<T, MustBeNoexcept>::value;
		
		
		
		template<typename T, bool MustBeNoexcept = false>
		struct can_bitwise_not_t:
		std::bool_constant<(requires(T t) { {~t}; } && !MustBeNoexcept) || requires(T t) { {~t} noexcept; }> {};
		
		template<typename T, bool MustBeNoexcept = false>
		concept can_bitwise_not = can_bitwise_not_t<T, MustBeNoexcept>::value;
		
		
		
		template<typename T, bool MustBeNoexcept = false>
		struct can_logical_not_t:
		std::bool_constant<(requires(T t) { {!t}; } && !MustBeNoexcept) || requires(T t) { {!t} noexcept; }> {};
		
		template<typename T, bool MustBeNoexcept = false>
		concept can_logical_not = can_logical_not_t<T, MustBeNoexcept>::value;
		
		
		
		template<typename T, bool MustBeNoexcept = false>
		struct can_unary_add_t:
		std::bool_constant<(requires(T t) { {+t}; } && !MustBeNoexcept) || requires(T t) { {+t} noexcept; }> {};
		
		template<typename T, bool MustBeNoexcept = false>
		concept can_unary_add = can_unary_add_t<T, MustBeNoexcept>::value;
		
		
		
		template<typename T, bool MustBeNoexcept = false>
		struct can_unary_sub_t:
		std::bool_constant<(requires(T t) { {-t}; } && !MustBeNoexcept) || requires(T t) { {-t} noexcept; }> {};
		
		template<typename T, bool MustBeNoexcept = false>
		concept can_unary_sub = can_unary_sub_t<T, MustBeNoexcept>::value;
		
		
		
		template<typename T, bool MustBeNoexcept = false>
		struct can_ref_t:
		std::bool_constant<(requires(T t) { {&t}; } && !MustBeNoexcept) || requires(T t) { {&t} noexcept; }> {};
		
		template<typename T, bool MustBeNoexcept = false>
		concept can_ref = can_ref_t<T, MustBeNoexcept>::value;
		
		
		
		template<typename T, bool MustBeNoexcept = false>
		struct can_deref_t:
		std::bool_constant<(requires(T t) { {*t}; } && !MustBeNoexcept) || requires(T t) { {*t} noexcept; }> {};
		
		template<typename T, bool MustBeNoexcept = false>
		concept can_deref = can_deref_t<T, MustBeNoexcept>::value;
		
		
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		struct can_mul_t:
		std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs * rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs * rhs} noexcept; }> {};
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept can_mul = can_mul_t<Lhs, Rhs, MustBeNoexcept>::value;
		
		
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		struct can_div_t:
		std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs / rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs / rhs} noexcept; }> {};
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept can_div = can_div_t<Lhs, Rhs, MustBeNoexcept>::value;
		
		
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		struct can_mod_t:
		std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs % rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs % rhs} noexcept; }> {};
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept can_mod = can_mod_t<Lhs, Rhs, MustBeNoexcept>::value;
		
		
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		struct can_add_t:
		std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs + rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs + rhs} noexcept; }> {};
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept can_add = can_add_t<Lhs, Rhs, MustBeNoexcept>::value;
		
		
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		struct can_sub_t:
		std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs - rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs - rhs} noexcept; }> {};
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept can_sub = can_sub_t<Lhs, Rhs, MustBeNoexcept>::value;
		
		
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		struct can_shift_left_t:
		std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs << rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs << rhs} noexcept; }> {};
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept can_shift_left = can_shift_left_t<Lhs, Rhs, MustBeNoexcept>::value;
		
		
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		struct can_shift_right_t:
		std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs >> rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs >> rhs} noexcept; }> {};
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept can_shift_right = can_shift_right_t<Lhs, Rhs, MustBeNoexcept>::value;
		
		
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		struct can_cmp_threeway_t:
		std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs <=> rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs <=> rhs} noexcept; }> {};
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept can_cmp_threeway = can_cmp_threeway_t<Lhs, Rhs, MustBeNoexcept>::value;
		
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		struct can_cmp_less_t:
		std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs < rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs < rhs} noexcept; }> {};
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept can_cmp_less = can_cmp_less_t<Lhs, Rhs, MustBeNoexcept>::value;
		
		
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		struct can_cmp_greater_t:
		std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs > rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs > rhs} noexcept; }> {};
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept can_cmp_greater = can_cmp_greater_t<Lhs, Rhs, MustBeNoexcept>::value;
		
		
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		struct can_cmp_less_eq_t:
		std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs <= rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs <= rhs} noexcept; }> {};
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept can_cmp_less_eq = can_cmp_less_eq_t<Lhs, Rhs, MustBeNoexcept>::value;
		
		
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		struct can_cmp_greater_eq_t:
		std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs >= rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs >= rhs} noexcept; }> {};
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept can_cmp_greater_eq = can_cmp_greater_eq_t<Lhs, Rhs, MustBeNoexcept>::value;
		
		
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		struct can_cmp_eq_t:
		std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs == rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs == rhs} noexcept; }> {};
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept can_cmp_eq = can_cmp_eq_t<Lhs, Rhs, MustBeNoexcept>::value;
		
		
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		struct can_cmp_neq_t:
		std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs != rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs != rhs} noexcept; }> {};
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept can_cmp_neq = can_cmp_neq_t<Lhs, Rhs, MustBeNoexcept>::value;
		
		
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		struct can_bitwise_and_t:
		std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs & rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs & rhs} noexcept; }> {};
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept can_bitwise_and = can_bitwise_and_t<Lhs, Rhs, MustBeNoexcept>::value;
		
		
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		struct can_bitwise_xor_t:
		std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs ^ rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs ^ rhs} noexcept; }> {};
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept can_bitwise_xor = can_bitwise_xor_t<Lhs, Rhs, MustBeNoexcept>::value;
		
		
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		struct can_bitwise_or_t:
		std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs | rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs | rhs} noexcept; }> {};
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept can_bitwise_or = can_bitwise_or_t<Lhs, Rhs, MustBeNoexcept>::value;
		
		
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		struct can_logical_and_t:
		std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs && rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs && rhs} noexcept; }> {};
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept can_logical_and = can_logical_and_t<Lhs, Rhs, MustBeNoexcept>::value;
		
		
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		struct can_logical_or_t:
		std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs || rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs || rhs} noexcept; }> {};
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept can_logical_or = can_logical_or_t<Lhs, Rhs, MustBeNoexcept>::value;
		
	}
	
	template<typename, typename, typename> class Vec;
	namespace generic_vec {
		
		// Empty struct which acts as a 'null' type in the Vec class, and can be targeted to achieve customized behaviour for other types.
		struct Empty {
			public: constexpr
			Empty(std::nullptr_t = {})
			noexcept {}
		};
		
		[[maybe_unused]] static constexpr decltype(auto)
		operator+(Empty, auto v) noexcept
		requires(std::is_arithmetic_v<std::remove_cvref_t<decltype(v)>>)
		{ return 0 + v; }
		
		[[maybe_unused]] static constexpr decltype(auto)
		operator+(auto v, Empty) noexcept
		requires(std::is_arithmetic_v<std::remove_cvref_t<decltype(v)>>)
		{ return v + 0; }
		
		[[maybe_unused]] static constexpr decltype(auto)
		operator+(Empty, Empty) noexcept { return Empty(); }
		
		
		
		[[maybe_unused]] static constexpr decltype(auto)
		operator-(Empty, auto v) noexcept
		requires(std::is_arithmetic_v<std::remove_cvref_t<decltype(v)>>)
		{ return 0 - v; }
		
		[[maybe_unused]] static constexpr decltype(auto)
		operator-(auto v, Empty) noexcept
		requires(std::is_arithmetic_v<std::remove_cvref_t<decltype(v)>>)
		{ return v - 0; }
		
		[[maybe_unused]] static constexpr decltype(auto)
		operator-(Empty, Empty) noexcept { return Empty(); }
		
		
		
		[[maybe_unused]] static constexpr decltype(auto)
		operator*(Empty, auto v) noexcept
		requires(std::is_arithmetic_v<std::remove_cvref_t<decltype(v)>>)
		{ return 0 * v; }
		
		[[maybe_unused]] static constexpr decltype(auto)
		operator*(auto v, Empty) noexcept
		requires(std::is_arithmetic_v<std::remove_cvref_t<decltype(v)>>)
		{ return v * 0; }
		
		[[maybe_unused]] static constexpr decltype(auto)
		operator*(Empty, Empty) noexcept { return Empty(); }
		
		
		
		[[maybe_unused]] static constexpr decltype(auto)
		operator/(Empty, auto v) noexcept
		requires(std::is_arithmetic_v<std::remove_cvref_t<decltype(v)>>)
		{ return 0 / v; }
		
		[[maybe_unused]] static constexpr decltype(auto)
		operator/(Empty, Empty) noexcept { return Empty(); }
		
		
		
		[[maybe_unused]] static constexpr decltype(auto)
		operator%(Empty, auto v) noexcept
		requires(std::is_arithmetic_v<std::remove_cvref_t<decltype(v)>>)
		{ return 0 % v; }
		
		[[maybe_unused]] static constexpr decltype(auto)
		operator%(Empty, Empty) noexcept { return Empty(); }
		
		
		
		namespace detail {
			template<typename T> struct AxisX {
				
				constexpr AxisX(auto&& v)
				noexcept(noexcept(T(v)))
				requires(std::constructible_from<T, decltype(v)>)
				: x(static_cast<T>(v)) {}
				
				constexpr AxisX(Empty = {})
				noexcept(noexcept(T()))
				requires(std::default_initializable<T>)
				: x() {}
				
				T x;
				
			};
			
			template<typename T> struct AxisY {
				
				constexpr AxisY(auto&& v)
				noexcept(noexcept(T(v)))
				requires(std::constructible_from<T, decltype(v)>)
				: y(static_cast<T>(v)) {}
				
				constexpr AxisY(Empty = {})
				noexcept(noexcept(T()))
				requires(std::default_initializable<T>)
				: y() {}
				
				T y;
				
			};
			
			template<typename T> struct AxisZ {
				
				constexpr AxisZ(auto&& v)
				noexcept(noexcept(T(v)))
				requires(std::constructible_from<T, decltype(v)>)
				: z(static_cast<T>(v)) {}
				
				constexpr AxisZ(Empty = {})
				noexcept(noexcept(T()))
				requires(std::default_initializable<T>)
				: z() {}
				
				T z;
				
			};
			
			template<> struct AxisX<void> { constexpr AxisX(Empty = Empty{}) noexcept {} static constexpr Empty x{}; };
			template<> struct AxisY<void> { constexpr AxisY(Empty = Empty{}) noexcept {} static constexpr Empty y{}; };
			template<> struct AxisZ<void> { constexpr AxisZ(Empty = Empty{}) noexcept {} static constexpr Empty z{}; };
			
			template<size_t i, typename X, typename Y, typename Z>
			using aligned_axis_at =
				std::conditional_t<( i == (2 - ((alignof(AxisX<X>) >= alignof(AxisY<Y>)) + (alignof(AxisX<X>) >= alignof(AxisZ<Z>)))) ), AxisX<X>,
				std::conditional_t<( i == (2 - ((alignof(AxisY<Y>) >  alignof(AxisX<X>)) + (alignof(AxisY<Y>) >= alignof(AxisZ<Z>)))) ), AxisY<Y>, AxisZ<Z>
				>
			>;
			
			template<typename X, typename Y, typename Z>
			struct VecBase:
				aligned_axis_at<0, X, Y, Z>,
				aligned_axis_at<1, X, Y, Z>,
				aligned_axis_at<2, X, Y, Z>
			{
				private: using base0 = aligned_axis_at<0, X, Y, Z>;
				private: using base1 = aligned_axis_at<1, X, Y, Z>;
				private: using base2 = aligned_axis_at<2, X, Y, Z>;
				
				protected: using baseX = AxisX<X>;
				protected: using baseY = AxisY<Y>;
				protected: using baseZ = AxisZ<Z>;
				
				/**
				 * This constructor accepts arguments in the order (X,Y,Z).
				 * It initializes all bases in order (base0,base1,base2).
				 * It determines, for each base, the appropriate argument to supply,
				 * based on whether a base is equal to (baseX), (baseY), or (baseZ).
				 */
				public: template<typename OX, typename OY, typename OZ>
				constexpr
				VecBase(OX&& vx, OY&& vy, OZ&& vz)
				noexcept(
						noexcept(baseX(std::declval<OX>()))
					&&	noexcept(baseY(std::declval<OY>()))
					&&	noexcept(baseZ(std::declval<OZ>()))
				)
				requires(
						std::constructible_from<baseX, OX>
					&&	std::constructible_from<baseY, OY>
					&&	std::constructible_from<baseZ, OZ>
				)
				:	base0([&]() constexpr -> decltype(auto) {
						if		constexpr(std::same_as<base0, baseX>)	{ return std::forward<OX>(vx); }
						else if	constexpr(std::same_as<base0, baseY>)	{ return std::forward<OY>(vy); }
						else if	constexpr(std::same_as<base0, baseZ>)	{ return std::forward<OZ>(vz); }
					}()),
					base1([&]() constexpr -> decltype(auto) {
						if		constexpr(std::same_as<base1, baseX>)	{ return std::forward<OX>(vx); }
						else if	constexpr(std::same_as<base1, baseY>)	{ return std::forward<OY>(vy); }
						else if	constexpr(std::same_as<base1, baseZ>)	{ return std::forward<OZ>(vz); }
					}()),
					base2([&]() constexpr -> decltype(auto) {
						if		constexpr(std::same_as<base2, baseX>)	{ return std::forward<OX>(vx); }
						else if	constexpr(std::same_as<base2, baseY>)	{ return std::forward<OY>(vy); }
						else if	constexpr(std::same_as<base2, baseZ>)	{ return std::forward<OZ>(vz); }
					}())
				{}
				
			};
			
		}
		
	}
	
	using generic_vec::Empty;
	
	// Flexible, multi-dimensional, templated math vector class.
	template<typename X, typename Y = X, typename Z = void>
	class Vec: public generic_vec::detail::VecBase<X, Y, Z> {
		
		private: using base = generic_vec::detail::VecBase<X, Y, Z>;
		
		public: using value_type_x = decltype(Vec::x);
		public: using value_type_y = decltype(Vec::y);
		public: using value_type_z = decltype(Vec::z);
		
		
		
		public: constexpr
		Vec()
		noexcept(noexcept(base(nullptr, nullptr, nullptr)))
		requires(std::constructible_from<base, Empty, Empty, Empty>)
		: base(nullptr, nullptr, nullptr) {}
		
		
		
		public: template<typename OX, typename OY, typename OZ>
		constexpr
		Vec(Vec<OX, OY, OZ> && other)
		noexcept(noexcept(base(other.x, other.y, other.z)))
		requires(std::constructible_from<base, decltype(other.x), decltype(other.y), decltype(other.z)>)
		: base(static_cast<value_type_x>(other.x), static_cast<value_type_y>(other.y), static_cast<value_type_z>(other.z)) {}
		
		public: template<typename OX, typename OY, typename OZ>
		constexpr
		Vec(Vec<OX, OY, OZ> const& other)
		noexcept(noexcept(base(other.x, other.y, other.z)))
		requires(std::constructible_from<base, decltype(other.x), decltype(other.y), decltype(other.z)>)
		: base(static_cast<value_type_x>(other.x), static_cast<value_type_y>(other.y), static_cast<value_type_z>(other.z)) {}
		
		
		
		public: template<typename OX, typename OY, typename OZ>
		constexpr
		Vec(OX&& vx, OY&& vy, OZ&& vz)
		noexcept(noexcept(base(std::declval<OX>(), std::declval<OY>(), std::declval<OZ>())))
		requires(std::constructible_from<base, OX, OY, OZ>)
		: base(std::forward<OX>(vx), std::forward<OY>(vy), std::forward<OZ>(vz)) {}
		
		
		
		public: template<typename OX, typename OY>
		constexpr
		Vec(OX&& vx, OY&& vy)
		noexcept(noexcept(base(std::declval<OX>(), std::declval<OY>(), nullptr)))
		requires(std::constructible_from<base, OX, OY, Empty> && std::is_void_v<Z>)
		: base(std::forward<OX>(vx), std::forward<OY>(vy), nullptr) {}
		
		public: template<typename OX, typename OZ>
		constexpr
		Vec(OX&& vx, OZ&& vz)
		noexcept(noexcept(base(std::declval<OX>(), nullptr, std::declval<OZ>())))
		requires(std::constructible_from<base, OX, Empty, OZ> && std::is_void_v<Y>)
		: base(std::forward<OX>(vx), nullptr, std::forward<OZ>(vz)) {}
		
		public: template<typename OY, typename OZ>
		constexpr
		Vec(OY&& vy, OZ&& vz)
		noexcept(noexcept(base(nullptr, std::declval<OY>(), std::declval<OZ>())))
		requires(std::constructible_from<base, Empty, OY, OZ> && std::is_void_v<X>)
		: base(nullptr, std::forward<OY>(vy), std::forward<OZ>(vz)) {}
		
	};
	
	template<typename X, typename Y, typename Z>
	Vec(X,Y,Z) -> Vec<
		std::conditional_t<(std::is_null_pointer_v<X> || std::same_as<X, Empty>), void, X>,
		std::conditional_t<(std::is_null_pointer_v<Y> || std::same_as<Y, Empty>), void, Y>,
		std::conditional_t<(std::is_null_pointer_v<Z> || std::same_as<Z, Empty>), void, Z>>;
	
	template<typename X, typename Y>
	Vec(X,Y) -> Vec<
		std::conditional_t<(std::is_null_pointer_v<X> || std::same_as<X, Empty>), void, X>,
		std::conditional_t<(std::is_null_pointer_v<Y> || std::same_as<Y, Empty>), void, Y>, void>;
	
	template<typename X, typename Y, typename Z>
	Vec(Vec<X, Y, Z>) -> Vec<X, Y, Z>;
	
}

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	
	
	return 0;
}
