#ifndef INK_GENERIC_VEC_LIB_FILE_GUARD
#define INK_GENERIC_VEC_LIB_FILE_GUARD

#include <concepts>
#include <utility>
#include <tuple>
#include <cmath>
#include <functional>

namespace ink {
	
	namespace concepts {
		
		/*
		 * Evaluates to a true type if types T and U are instances of the same template.
		 * Limited support for templates using literal arguments
		 * Implemented variations are: 
		 * - '<typename...>'
		 * - '<auto...>'
		 * - '<typename, auto...>'
		 * - '<auto, typename...>'
		 * 
		 * Add specializations as needed, if needed, something like:
		 * template<template<(auto/typename)...> typename Template, (auto/typename)... T, (auto/typename)... U>
		 * struct same_template_t<Template<T...>, Template<U...>>: std::true_type {};
		 */
		template<typename T, typename U>
		struct same_template_t: std::false_type {};
		
		template<template<typename...> typename Template, typename... T, typename... U>
		struct same_template_t<Template<T...>, Template<U...>>: std::true_type {};
		
		template<template<auto...> typename Template, auto... T, auto... U>
		struct same_template_t<Template<T...>, Template<U...>>: std::true_type {};
		
		template<template<typename, auto...> typename Template, typename T1, typename U1, auto... T, auto... U>
		struct same_template_t<Template<T1, T...>, Template<U1, U...>>: std::true_type {};
		
		template<template<auto, typename...> typename Template, auto T1, auto U1, typename... T, typename... U>
		struct same_template_t<Template<T1, T...>, Template<U1, U...>>: std::true_type {};
		
		/**
		 * Conceptualization of same_template_t<>
		 */
		template<typename T, typename U>
		concept same_template = same_template_t<T, U>::value;
		
		
		
		template<typename T>
		concept std_bool_constant =
		std::same_as<T, std::true_type> || std::same_as<T, std::false_type>;
		
		
		
		namespace operator_constraints {
			
			template<typename T, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_postfix_inc_t:
			std::bool_constant<(requires(T t) { {t++}; } && !MustBeNoexcept{}) || requires(T t) { {t++} noexcept; }> {};
			
			template<typename T, typename MustBeNoexcept = std::false_type>
			concept can_postfix_inc = can_postfix_inc_t<T, MustBeNoexcept>::value;
			
			
			
			template<typename T, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_postfix_dec_t:
			std::bool_constant<(requires(T t) { {t--}; } && !MustBeNoexcept{}) || requires(T t) { {t--} noexcept; }> {};
			
			template<typename T, typename MustBeNoexcept = std::false_type>
			concept can_postfix_dec = can_postfix_dec_t<T, MustBeNoexcept>::value;
			
			
			
			template<typename T, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_prefix_inc_t:
			std::bool_constant<(requires(T t) { {++t}; } && !MustBeNoexcept{}) || requires(T t) { {++t} noexcept; }> {};
			
			template<typename T, typename MustBeNoexcept = std::false_type>
			concept can_prefix_inc = can_prefix_inc_t<T, MustBeNoexcept>::value;
			
			
			
			template<typename T, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_prefix_dec_t:
			std::bool_constant<(requires(T t) { {--t}; } && !MustBeNoexcept{}) || requires(T t) { {--t} noexcept; }> {};
			
			template<typename T, typename MustBeNoexcept = std::false_type>
			concept can_prefix_dec = can_prefix_dec_t<T, MustBeNoexcept>::value;
			
			
			
			template<typename T, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_bitwise_not_t:
			std::bool_constant<(requires(T t) { {~t}; } && !MustBeNoexcept{}) || requires(T t) { {~t} noexcept; }> {};
			
			template<typename T, typename MustBeNoexcept = std::false_type>
			concept can_bitwise_not = can_bitwise_not_t<T, MustBeNoexcept>::value;
			
			
			
			template<typename T, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_logical_not_t:
			std::bool_constant<(requires(T t) { {!t}; } && !MustBeNoexcept{}) || requires(T t) { {!t} noexcept; }> {};
			
			template<typename T, typename MustBeNoexcept = std::false_type>
			concept can_logical_not = can_logical_not_t<T, MustBeNoexcept>::value;
			
			
			
			template<typename T, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_unary_add_t:
			std::bool_constant<(requires(T t) { {+t}; } && !MustBeNoexcept{}) || requires(T t) { {+t} noexcept; }> {};
			
			template<typename T, typename MustBeNoexcept = std::false_type>
			concept can_unary_add = can_unary_add_t<T, MustBeNoexcept>::value;
			
			
			
			template<typename T, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_unary_sub_t:
			std::bool_constant<(requires(T t) { {-t}; } && !MustBeNoexcept{}) || requires(T t) { {-t} noexcept; }> {};
			
			template<typename T, typename MustBeNoexcept = std::false_type>
			concept can_unary_sub = can_unary_sub_t<T, MustBeNoexcept>::value;
			
			
			
			template<typename T, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_ref_t:
			std::bool_constant<(requires(T t) { {&t}; } && !MustBeNoexcept{}) || requires(T t) { {&t} noexcept; }> {};
			
			template<typename T, typename MustBeNoexcept = std::false_type>
			concept can_ref = can_ref_t<T, MustBeNoexcept>::value;
			
			
			
			template<typename T, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_deref_t:
			std::bool_constant<(requires(T t) { {*t}; } && !MustBeNoexcept{}) || requires(T t) { {*t} noexcept; }> {};
			
			template<typename T, typename MustBeNoexcept = std::false_type>
			concept can_deref = can_deref_t<T, MustBeNoexcept>::value;
			
			
			
			template<typename Lhs, typename Rhs = Lhs, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_mul_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs * rhs}; } && !MustBeNoexcept{}) || requires(Lhs lhs, Rhs rhs) { {lhs * rhs} noexcept; }> {};
			
			template<typename Lhs, typename Rhs = Lhs, typename MustBeNoexcept = std::false_type>
			concept can_mul = can_mul_t<Lhs, Rhs, MustBeNoexcept>::value;
			
			
			
			template<typename Lhs, typename Rhs = Lhs, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_div_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs / rhs}; } && !MustBeNoexcept{}) || requires(Lhs lhs, Rhs rhs) { {lhs / rhs} noexcept; }> {};
			
			template<typename Lhs, typename Rhs = Lhs, typename MustBeNoexcept = std::false_type>
			concept can_div = can_div_t<Lhs, Rhs, MustBeNoexcept>::value;
			
			
			
			template<typename Lhs, typename Rhs = Lhs, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_mod_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs % rhs}; } && !MustBeNoexcept{}) || requires(Lhs lhs, Rhs rhs) { {lhs % rhs} noexcept; }> {};
			
			template<typename Lhs, typename Rhs = Lhs, typename MustBeNoexcept = std::false_type>
			concept can_mod = can_mod_t<Lhs, Rhs, MustBeNoexcept>::value;
			
			
			
			template<typename Lhs, typename Rhs = Lhs, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_add_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs + rhs}; } && !MustBeNoexcept{}) || requires(Lhs lhs, Rhs rhs) { {lhs + rhs} noexcept; }> {};
			
			template<typename Lhs, typename Rhs = Lhs, typename MustBeNoexcept = std::false_type>
			concept can_add = can_add_t<Lhs, Rhs, MustBeNoexcept>::value;
			
			
			
			template<typename Lhs, typename Rhs = Lhs, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_sub_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs - rhs}; } && !MustBeNoexcept{}) || requires(Lhs lhs, Rhs rhs) { {lhs - rhs} noexcept; }> {};
			
			template<typename Lhs, typename Rhs = Lhs, typename MustBeNoexcept = std::false_type>
			concept can_sub = can_sub_t<Lhs, Rhs, MustBeNoexcept>::value;
			
			
			
			template<typename Lhs, typename Rhs = Lhs, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_shift_left_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs << rhs}; } && !MustBeNoexcept{}) || requires(Lhs lhs, Rhs rhs) { {lhs << rhs} noexcept; }> {};
			
			template<typename Lhs, typename Rhs = Lhs, typename MustBeNoexcept = std::false_type>
			concept can_shift_left = can_shift_left_t<Lhs, Rhs, MustBeNoexcept>::value;
			
			
			
			template<typename Lhs, typename Rhs = Lhs, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_shift_right_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs >> rhs}; } && !MustBeNoexcept{}) || (requires(Lhs lhs, Rhs rhs) { {lhs >> rhs} noexcept; })> {};
			
			template<typename Lhs, typename Rhs = Lhs, typename MustBeNoexcept = std::false_type>
			concept can_shift_right = can_shift_right_t<Lhs, Rhs, MustBeNoexcept>::value;
			
			
			
			template<typename Lhs, typename Rhs = Lhs, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_cmp_threeway_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs <=> rhs}; } && !MustBeNoexcept{}) || (requires(Lhs lhs, Rhs rhs) { {lhs <=> rhs} noexcept; })> {};
			
			template<typename Lhs, typename Rhs = Lhs, typename MustBeNoexcept = std::false_type>
			concept can_cmp_threeway = can_cmp_threeway_t<Lhs, Rhs, MustBeNoexcept>::value;
			
			
			template<typename Lhs, typename Rhs = Lhs, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_cmp_less_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs < rhs}; } && !MustBeNoexcept{}) || (requires(Lhs lhs, Rhs rhs) { {lhs < rhs} noexcept; })> {};
			
			template<typename Lhs, typename Rhs = Lhs, typename MustBeNoexcept = std::false_type>
			concept can_cmp_less = can_cmp_less_t<Lhs, Rhs, MustBeNoexcept>::value;
			
			
			
			template<typename Lhs, typename Rhs = Lhs, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_cmp_greater_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs > rhs}; } && !MustBeNoexcept{}) || (requires(Lhs lhs, Rhs rhs) { {lhs > rhs} noexcept; })> {};
			
			template<typename Lhs, typename Rhs = Lhs, typename MustBeNoexcept = std::false_type>
			concept can_cmp_greater = can_cmp_greater_t<Lhs, Rhs, MustBeNoexcept>::value;
			
			
			
			template<typename Lhs, typename Rhs = Lhs, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_cmp_less_eq_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs <= rhs}; } && !MustBeNoexcept{}) || (requires(Lhs lhs, Rhs rhs) { {lhs <= rhs} noexcept; })> {};
			
			template<typename Lhs, typename Rhs = Lhs, typename MustBeNoexcept = std::false_type>
			concept can_cmp_less_eq = can_cmp_less_eq_t<Lhs, Rhs, MustBeNoexcept>::value;
			
			
			
			template<typename Lhs, typename Rhs = Lhs, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_cmp_greater_eq_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs >= rhs}; } && !MustBeNoexcept{}) || (requires(Lhs lhs, Rhs rhs) { {lhs >= rhs} noexcept; })> {};
			
			template<typename Lhs, typename Rhs = Lhs, typename MustBeNoexcept = std::false_type>
			concept can_cmp_greater_eq = can_cmp_greater_eq_t<Lhs, Rhs, MustBeNoexcept>::value;
			
			
			
			template<typename Lhs, typename Rhs = Lhs, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_cmp_eq_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs == rhs}; } && !MustBeNoexcept{}) || requires(Lhs lhs, Rhs rhs) { {lhs == rhs} noexcept; }> {};
			
			template<typename Lhs, typename Rhs = Lhs, typename MustBeNoexcept = std::false_type>
			concept can_cmp_eq = can_cmp_eq_t<Lhs, Rhs, MustBeNoexcept>::value;
			
			
			
			template<typename Lhs, typename Rhs = Lhs, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_cmp_neq_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs != rhs}; } && !MustBeNoexcept{}) || requires(Lhs lhs, Rhs rhs) { {lhs != rhs} noexcept; }> {};
			
			template<typename Lhs, typename Rhs = Lhs, typename MustBeNoexcept = std::false_type>
			concept can_cmp_neq = can_cmp_neq_t<Lhs, Rhs, MustBeNoexcept>::value;
			
			
			
			template<typename Lhs, typename Rhs = Lhs, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_bitwise_and_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs & rhs}; } && !MustBeNoexcept{}) || requires(Lhs lhs, Rhs rhs) { {lhs & rhs} noexcept; }> {};
			
			template<typename Lhs, typename Rhs = Lhs, typename MustBeNoexcept = std::false_type>
			concept can_bitwise_and = can_bitwise_and_t<Lhs, Rhs, MustBeNoexcept>::value;
			
			
			
			template<typename Lhs, typename Rhs = Lhs, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_bitwise_xor_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs ^ rhs}; } && !MustBeNoexcept{}) || requires(Lhs lhs, Rhs rhs) { {lhs ^ rhs} noexcept; }> {};
			
			template<typename Lhs, typename Rhs = Lhs, typename MustBeNoexcept = std::false_type>
			concept can_bitwise_xor = can_bitwise_xor_t<Lhs, Rhs, MustBeNoexcept>::value;
			
			
			
			template<typename Lhs, typename Rhs = Lhs, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_bitwise_or_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs | rhs}; } && !MustBeNoexcept{}) || requires(Lhs lhs, Rhs rhs) { {lhs | rhs} noexcept; }> {};
			
			template<typename Lhs, typename Rhs = Lhs, typename MustBeNoexcept = std::false_type>
			concept can_bitwise_or = can_bitwise_or_t<Lhs, Rhs, MustBeNoexcept>::value;
			
			
			
			template<typename Lhs, typename Rhs = Lhs, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_logical_and_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs && rhs}; } && !MustBeNoexcept{}) || requires(Lhs lhs, Rhs rhs) { {lhs && rhs} noexcept; }> {};
			
			template<typename Lhs, typename Rhs = Lhs, typename MustBeNoexcept = std::false_type>
			concept can_logical_and = can_logical_and_t<Lhs, Rhs, MustBeNoexcept>::value;
			
			
			
			template<typename Lhs, typename Rhs = Lhs, std_bool_constant MustBeNoexcept = std::false_type>
			struct can_logical_or_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs || rhs}; } && !MustBeNoexcept{}) || requires(Lhs lhs, Rhs rhs) { {lhs || rhs} noexcept; }> {};
			
			template<typename Lhs, typename Rhs = Lhs, typename MustBeNoexcept = std::false_type>
			concept can_logical_or = can_logical_or_t<Lhs, Rhs, MustBeNoexcept>::value;
		}
		
		using namespace operator_constraints;
		
	}
	
	namespace generic_vec {
		
		/**
		 * Phony target for various operations.
		 * Should act as a compile-time entity, and should have no runtime state.
		 */
		struct NoState {
			
			template<typename,typename,typename> friend class Vec;
			
			public: static constexpr auto
			value = 0;
			
			public: template<typename T>
			static constexpr auto
			value_as = static_cast<T>(value);
			
			public: using
			value_type = std::remove_cvref_t<decltype(value)>;
			
			public: constexpr
			NoState()
			noexcept {}
			
			public: constexpr
			NoState(std::nullptr_t)
			noexcept {}
			
			public: constexpr
			NoState(NoState const&)
			noexcept {}
			
			public: template<typename T> constexpr explicit
			NoState(T)
			noexcept {}
			
			public: template<std::default_initializable T>
			constexpr explicit
			operator T() const
			noexcept(noexcept(T{})) { return T{}; }
			
			
			
			public: friend constexpr decltype(auto)
			operator++(NoState, int)
			noexcept { return NoState(); }
			
			public: friend constexpr decltype(auto)
			operator--(NoState, int)
			noexcept { return NoState(); }
			
			public: friend constexpr decltype(auto)
			operator++(NoState)
			noexcept { return NoState(); }
			
			public: friend constexpr decltype(auto)
			operator--(NoState)
			noexcept { return NoState(); }
			
			
			
			public: friend constexpr decltype(auto)
			operator~(NoState)
			noexcept { return NoState(); }
			
			public: friend constexpr decltype(auto)
			operator!(NoState)
			noexcept { return NoState(); }
			
			
			
			public: friend constexpr decltype(auto)
			operator+(NoState)
			noexcept { return NoState(); }
			
			public: friend constexpr decltype(auto)
			operator-(NoState)
			noexcept { return NoState(); }
			
			
			
			public: template<typename T> requires(std::is_arithmetic_v<T>)
			friend constexpr decltype(auto)
			operator*(NoState, T v)
			noexcept { return value * v; }
			
			public: template<typename T> requires(std::is_arithmetic_v<T>)
			friend constexpr decltype(auto)
			operator*(T v, NoState)
			noexcept { return v * value; }
			
			public: friend constexpr decltype(auto)
			operator*(NoState, NoState)
			noexcept { return NoState(); }
			
			
			
			public: template<typename T> requires(std::is_arithmetic_v<T>)
			friend constexpr decltype(auto)
			operator/(NoState, T v)
			noexcept { return value / v; }
			
			public: template<std::integral T>
			friend constexpr decltype(auto)
			operator/(T, NoState) noexcept = delete;
			
			public: template<std::floating_point T>
			friend constexpr decltype(auto)
			operator/(T v, NoState)
			noexcept(noexcept(v / (value_as<T>)))
			{ return v / (value_as<T>); }
			
			public: friend constexpr decltype(auto)
			operator/(NoState, NoState)
			noexcept { return NoState(); }
			
			
			
			public: template<std::integral T>
			friend constexpr decltype(auto)
			operator%(NoState, T v)
			noexcept { return value % v; }
			
			public: template<std::integral T>
			friend constexpr decltype(auto)
			operator%(T, NoState) noexcept = delete;
			
			public: friend constexpr decltype(auto)
			operator%(NoState, NoState)
			noexcept { return NoState(); }
			
			
			
			public: template<typename T> requires(std::is_arithmetic_v<T>)
			friend constexpr decltype(auto)
			operator+(NoState, T v)
			noexcept { return value + v; }
			
			public: template<typename T> requires(std::is_arithmetic_v<T>)
			friend constexpr decltype(auto)
			operator+(T v, NoState)
			noexcept { return v + value; }
			
			public: friend constexpr decltype(auto)
			operator+(NoState, NoState)
			noexcept { return NoState(); }
			
			
			
			public: template<typename T> requires(std::is_arithmetic_v<T>)
			friend constexpr decltype(auto)
			operator-(NoState, T v)
			noexcept { return value - v; }
			
			public: template<typename T> requires(std::is_arithmetic_v<T>)
			friend constexpr decltype(auto)
			operator-(T v, NoState)
			noexcept { return v - value; }
			
			public: friend constexpr decltype(auto)
			operator-(NoState, NoState)
			noexcept { return NoState(); }
			
			
			
			public: template<std::integral T>
			friend constexpr decltype(auto)
			operator<<(NoState, T)
			noexcept { return NoState(); }
			
			public: template<std::integral T>
			friend constexpr decltype(auto)
			operator<<(T v, NoState)
			noexcept { return v << value; }
			
			public: template<std::integral T>
			friend constexpr decltype(auto)
			operator<<(NoState, NoState)
			noexcept { return NoState(); }
			
			
			
			public: template<std::integral T>
			friend constexpr decltype(auto)
			operator>>(NoState, T)
			noexcept { return NoState(); }
			
			public: template<std::integral T>
			friend constexpr decltype(auto)
			operator>>(T v, NoState)
			noexcept { return v >> value; }
			
			public: template<std::integral T>
			friend constexpr decltype(auto)
			operator>>(NoState, NoState)
			noexcept { return NoState(); }
			
			
			
			public: template<typename T> requires(std::is_arithmetic_v<T>)
			friend constexpr decltype(auto)
			operator<=>(NoState, T v)
			noexcept { return value <=> v; }
			
			public: template<typename T> requires(std::is_arithmetic_v<T>)
			friend constexpr decltype(auto)
			operator<=>(T v, NoState)
			noexcept { return v <=> value; }
			
			public: friend constexpr decltype(auto)
			operator<=>(NoState, NoState)
			noexcept { return std::strong_ordering::equal; }
			
			
			
			public: template<typename T> requires(std::is_arithmetic_v<T>)
			friend constexpr decltype(auto)
			operator==(NoState, T v)
			noexcept { return value == v; }
			
			public: template<typename T> requires(std::is_arithmetic_v<T>)
			friend constexpr decltype(auto)
			operator==(T v, NoState)
			noexcept { return v == value; }
			
			public: friend constexpr decltype(auto)
			operator==(NoState, NoState)
			noexcept { return true; }
			
			
			
			public: template<typename T> requires(std::is_arithmetic_v<T>)
			friend constexpr decltype(auto)
			operator!=(NoState, T v)
			noexcept { return value != v; }
			
			public: template<typename T> requires(std::is_arithmetic_v<T>)
			friend constexpr decltype(auto)
			operator!=(T v, NoState)
			noexcept { return v != value; }
			
			public: friend constexpr decltype(auto)
			operator!=(NoState, NoState)
			noexcept { return false; }
			
			
			
			public: template<std::integral T>
			friend constexpr decltype(auto)
			operator&(NoState, T v)
			noexcept { return value & v; }
			
			public: template<std::integral T>
			friend constexpr decltype(auto)
			operator&(T v, NoState)
			noexcept { return v & value; }
			
			public: friend constexpr decltype(auto)
			operator&(NoState, NoState)
			noexcept { return NoState(); }
			
			
			
			public: template<std::integral T>
			friend constexpr decltype(auto)
			operator^(NoState, T v)
			noexcept { return value ^ v; }
			
			public: template<std::integral T>
			friend constexpr decltype(auto)
			operator^(T v, NoState)
			noexcept { return v ^ value; }
			
			public: friend constexpr decltype(auto)
			operator^(NoState, NoState)
			noexcept { return NoState(); }
			
			
			
			public: template<std::integral T>
			friend constexpr decltype(auto)
			operator|(NoState, T v)
			noexcept { return value | v; }
			
			public: template<std::integral T>
			friend constexpr decltype(auto)
			operator|(T v, NoState)
			noexcept { return v | value; }
			
			public: friend constexpr decltype(auto)
			operator|(NoState, NoState)
			noexcept { return NoState(); }
			
			
			
			public: template<typename T> requires(std::is_arithmetic_v<T>)
			friend constexpr decltype(auto)
			operator&&(NoState, T v)
			noexcept { return value && v; }
			
			public: template<typename T> requires(std::is_arithmetic_v<T>)
			friend constexpr decltype(auto)
			operator&&(T v, NoState)
			noexcept { return v && value; }
			
			public: friend constexpr decltype(auto)
			operator&&(NoState, NoState)
			noexcept { return NoState(); }
			
			
			
			public: template<typename T> requires(std::is_arithmetic_v<T>)
			friend constexpr decltype(auto)
			operator||(NoState, T v)
			noexcept { return value || v; }
			
			public: template<typename T> requires(std::is_arithmetic_v<T>)
			friend constexpr decltype(auto)
			operator||(T v, NoState)
			noexcept { return v || value; }
			
			public: friend constexpr decltype(auto)
			operator||(NoState, NoState)
			noexcept { return NoState(); }
			
			
			
			public: constexpr decltype(auto)
			operator=(auto&&) const
			noexcept { return *this; }
			
			public: constexpr decltype(auto)
			operator*=(auto&&) const
			noexcept { return *this; }
			
			public: constexpr decltype(auto)
			operator/=(auto&&) const
			noexcept { return *this; }
			
			public: constexpr decltype(auto)
			operator%=(auto&&) const
			noexcept { return *this; }
			
			public: constexpr decltype(auto)
			operator+=(auto&&) const
			noexcept { return *this; }
			
			public: constexpr decltype(auto)
			operator-=(auto&&) const
			noexcept { return *this; }
			
			public: constexpr decltype(auto)
			operator>>=(auto&&) const
			noexcept { return *this; }
			
			public: constexpr decltype(auto)
			operator<<=(auto&&) const
			noexcept { return *this; }
			
			public: constexpr decltype(auto)
			operator&=(auto&&) const
			noexcept { return *this; }
			
			public: constexpr decltype(auto)
			operator^=(auto&&) const
			noexcept { return *this; }
			
			public: constexpr decltype(auto)
			operator|=(auto&&) const
			noexcept { return *this; }
			
		};
		
		namespace detail {
			
			enum class XYZ: size_t
			{ X , Y , Z };
			
			template<typename T, XYZ tag> struct Member;
			template<typename T> struct Member<T, XYZ::X> { T x; };
			template<typename T> struct Member<T, XYZ::Y> { T y; };
			template<typename T> struct Member<T, XYZ::Z> { T z; };
			
			template<> struct Member<void, XYZ::X> { static constexpr NoState x{}; };
			template<> struct Member<void, XYZ::Y> { static constexpr NoState y{}; };
			template<> struct Member<void, XYZ::Z> { static constexpr NoState z{}; };
			
			template<typename T, XYZ tag>
			class Axis:
				public Member<T, tag>
			{
				
				private:
				using base = Member<T, tag>;
				
				public: constexpr explicit
				Axis(NoState)
				noexcept( noexcept(base{}) )
				requires( std::default_initializable<base> )
				: base{} {}
				
				public: template<typename U>
				requires (!std::convertible_to<std::remove_cvref_t<U>, NoState>) &&
					requires (U u) { {base{u}}; } 
				constexpr explicit
				Axis(U&& u)
				noexcept( noexcept(base{std::declval<U>()}) )
				: base{std::forward<U>(u)} {}
				
			};
			
			template<std::size_t i, typename X, typename Y, typename Z>
			using aligned_axis_at =
				std::conditional_t<( i == (2 - ((alignof(Axis<X, XYZ::X>) >= alignof(Axis<Y, XYZ::Y>)) + (alignof(Axis<X, XYZ::X>) >= alignof(Axis<Z, XYZ::Z>)))) ),
					Axis<X, XYZ::X>,
				std::conditional_t<( i == (2 - ((alignof(Axis<Y, XYZ::Y>) >  alignof(Axis<X, XYZ::X>)) + (alignof(Axis<Y, XYZ::Y>) >= alignof(Axis<Z, XYZ::Z>)))) ),
					Axis<Y, XYZ::Y>,
					Axis<Z, XYZ::Z>
				>
			>;
			
			template<typename X, typename Y, typename Z>
			class VecBase:
				public aligned_axis_at<0, X, Y, Z>,
				public aligned_axis_at<1, X, Y, Z>,
				public aligned_axis_at<2, X, Y, Z>
			{
				
				protected: using baseX = Axis<X, XYZ::X>;
				protected: using baseY = Axis<Y, XYZ::Y>;
				protected: using baseZ = Axis<Z, XYZ::Z>;
				
				protected: using value_type_x = decltype(baseX::x);
				protected: using value_type_y = decltype(baseX::x);
				protected: using value_type_z = decltype(baseX::x);
				
				private: using base0 = aligned_axis_at<0, X, Y, Z>;
				private: using base1 = aligned_axis_at<1, X, Y, Z>;
				private: using base2 = aligned_axis_at<2, X, Y, Z>;
				
				private: template<typename TargetBase, typename OX, typename OY, typename OZ>
				static constexpr decltype(auto)
				M_fwd_param(OX&& ox, OY&& oy, OZ&& oz) noexcept {
					if		constexpr(std::same_as<baseX, TargetBase>) return std::forward<OX>(ox);
					else if	constexpr(std::same_as<baseY, TargetBase>) return std::forward<OY>(oy);
					else if	constexpr(std::same_as<baseZ, TargetBase>) return std::forward<OZ>(oz);
				}
				
				public:
				template<typename OX, typename OY, typename OZ>
				constexpr explicit
				VecBase(OX&& vx, OY&& vy, OZ&& vz)
				noexcept( noexcept(baseX{std::declval<OX>()}) && noexcept(baseY{std::declval<OY>()}) && noexcept(baseZ{std::declval<OZ>()}) )
				requires( requires(OX ox, OY oy, OZ oz) { baseX{ox}; baseY{oy}; baseZ{oz}; } )
				:	base0(M_fwd_param<base0>(vx, vy, vz)),
					base1(M_fwd_param<base1>(vx, vy, vz)),
					base2(M_fwd_param<base2>(vx, vy, vz))
				{}
				
			};
			
		}
		
	}
	
	namespace generic_vec {
		
		template<typename X, typename Y, typename Z>
		class Vec;
		
		template<typename Lhs, typename Rhs>
		static constexpr decltype(auto)
		DefaultToNoState(Lhs&& lhs, Rhs&& rhs) noexcept {
			constexpr auto lNoState = std::same_as<std::remove_cvref_t<Lhs>, NoState>;
			constexpr auto rNoState = std::same_as<std::remove_cvref_t<Rhs>, NoState>;
			if constexpr(lNoState || rNoState)
			{ return std::make_tuple(NoState(), NoState()); }
			else
			{ return std::tie(std::forward<Lhs>(lhs), std::forward<Rhs>(rhs)); }
		}
		
		
		
		template<typename LVec, typename RVec>
		requires(requires(LVec l, RVec r)
		{ {(l.x * r.x) + (l.y * r.y) + (l.z * r.z)}; })
		static constexpr decltype(auto)
		dot(LVec const& lhs, RVec const& rhs)
		noexcept( requires(LVec l, RVec r) { {(l.x * r.x) + (l.y * r.y) + (l.z * r.z)} noexcept; } )
		{ return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z); }
		
		
		
		template<typename LVec, typename RVec>
		requires requires(LVec l, RVec r)
		{ {generic_vec::Vec{(l.y * r.z), (l.z * r.x), (l.x * r.y)}}; }
		static constexpr decltype(auto)
		cross(LVec const& lhs, RVec const& rhs)
		noexcept(noexcept( generic_vec::Vec(lhs.y * rhs.z, lhs.z * rhs.x, lhs.x * rhs.y) )) {
			auto&& x = (lhs.y * rhs.z) - (lhs.z * rhs.y);
			auto&& y = (lhs.x * rhs.z) - (lhs.z * rhs.x);
			auto&& z = (lhs.x * rhs.y) - (lhs.y * rhs.x);
			return generic_vec::Vec(x, -y, z);
		}
		
	}
	
	namespace generic_vec {
		
		// Flexible, multi-dimensional, templated math vector class.
		template<typename X, typename Y = X, typename Z = Y>
		class Vec: public generic_vec::detail::VecBase<X, Y, Z> {
			
			private: using base = generic_vec::detail::VecBase<X, Y, Z>;
			
			private: using baseX = typename base::baseX;
			private: using baseY = typename base::baseY;
			private: using baseZ = typename base::baseZ;
			
			public: using value_type_x = decltype(Vec::x);
			public: using value_type_y = decltype(Vec::y);
			public: using value_type_z = decltype(Vec::z);
			
			public: using NoState = generic_vec::NoState;
			
			
			
			public: constexpr
			Vec()
			noexcept(noexcept(base(NoState{}, NoState{}, NoState{})))
			requires(std::constructible_from<base, NoState, NoState, NoState>)
			: base(NoState{}, NoState{}, NoState{}) {}
			
			
			
			public: template<typename OX, typename OY, typename OZ>
			constexpr
			Vec(OX&& vx, OY&& vy, OZ&& vz)
			noexcept(noexcept(base(std::declval<OX>(), std::declval<OY>(), std::declval<OZ>())))
			requires(std::constructible_from<base, OX, OY, OZ>)
			: base(std::forward<OX>(vx), std::forward<OY>(vy), std::forward<OZ>(vz)) {}
			
			
			
			public: template<typename OX, typename OY, typename OZ = NoState>
			requires(std::constructible_from<base, OX, OY, OZ>)
			constexpr
			Vec(OX&& vx, OY&& vy)
			noexcept(noexcept(base(std::declval<OX>(), std::declval<OY>(), std::declval<OZ>())))
			: base(std::forward<OX>(vx), std::forward<OY>(vy), NoState{}) {}
			
			public: template<typename OX, typename OY = NoState, typename OZ>
			requires(std::constructible_from<base, OX, OY, OZ> && std::is_void_v<Y>)
			constexpr
			Vec(OX&& vx, OZ&& vz)
			noexcept(noexcept(base(std::declval<OX>(), std::declval<OY>(), std::declval<OZ>())))
			: base(std::forward<OX>(vx), NoState{}, std::forward<OZ>(vz)) {}
			
			public: template<typename OX = NoState, typename OY, typename OZ>
			requires(std::constructible_from<base, OX, OY, OZ> && std::is_void_v<X> && !std::is_void_v<Z>)
			constexpr
			Vec(OY&& vy, OZ&& vz)
			noexcept(noexcept(base(std::declval<OX>(), std::declval<OY>(), std::declval<OZ>())))
			: base(NoState{}, std::forward<OY>(vy), std::forward<OZ>(vz)) {}
			
			
			
			public: template<typename OX, typename OY, typename OZ>
			requires(
					(std::convertible_to<OX, value_type_x> || std::is_void_v<OX> || std::is_void_v<X>)
				&&	(std::convertible_to<OY, value_type_y> || std::is_void_v<OY> || std::is_void_v<Y>)
				&&	(std::convertible_to<OZ, value_type_z> || std::is_void_v<OZ> || std::is_void_v<Z>) )
			constexpr explicit
			Vec(Vec<OX, OY, OZ> const& ovec)
			noexcept(noexcept(base(static_cast<value_type_x>(ovec.x), static_cast<value_type_y>(ovec.y), static_cast<value_type_z>(ovec.z))))
			: base(static_cast<value_type_x>(ovec.x), static_cast<value_type_y>(ovec.y), static_cast<value_type_z>(ovec.z)) {}
			
			
			
			template<typename OX, typename OY, typename OZ>
			friend constexpr decltype(auto)
			dot(Vec const& lhs, Vec<OX, OY, OZ> const& rhs) noexcept(noexcept(generic_vec::dot(lhs, rhs)))
			{ return ink::generic_vec::dot(lhs, rhs); }
			
			template<typename OX, typename OY, typename OZ>
			friend constexpr decltype(auto)
			cross(Vec const& lhs, Vec<OX, OY, OZ> const& rhs) noexcept(noexcept(generic_vec::dot(lhs, rhs)))
			{ return ink::generic_vec::cross(lhs, rhs); }
			
			
			
			public: template<typename OX, typename OY, typename OZ>
			constexpr decltype(auto)
			operator=(Vec<OX, OY, OZ> && other)
			requires(std::is_reference_v<X> || std::is_reference_v<Y> || std::is_reference_v<Z>)
			{
				this->x = static_cast<std::remove_reference_t<value_type_x>>(other.x);
				this->y = static_cast<std::remove_reference_t<value_type_y>>(other.y);
				this->z = static_cast<std::remove_reference_t<value_type_z>>(other.z);
			}
			
			public: friend constexpr decltype(auto)
			operator+(Vec const& vec)
			noexcept( noexcept(+vec.x) && noexcept(+vec.y) && noexcept(+vec.z) )
			requires(concepts::can_unary_add<value_type_x> && concepts::can_unary_add<value_type_y> && concepts::can_unary_add<value_type_z>)
			{ return generic_vec::Vec(+vec.x, +vec.y, +vec.z); }
			
			public: friend constexpr decltype(auto)
			operator-(Vec const& vec)
			noexcept( noexcept(-vec.x) && noexcept(-vec.y) && noexcept(-vec.z) )
			requires(concepts::can_unary_sub<value_type_x> && concepts::can_unary_sub<value_type_y> && concepts::can_unary_sub<value_type_z>)
			{ return generic_vec::Vec(-vec.x, -vec.y, -vec.z); }
			
			public: friend constexpr decltype(auto)
			operator!(Vec const& vec)
			noexcept( noexcept(!vec.x) && noexcept(!vec.y) && noexcept(!vec.z) )
			requires(concepts::can_logical_not<value_type_x> && concepts::can_logical_not<value_type_y> && concepts::can_logical_not<value_type_z>)
			{ return generic_vec::Vec(!vec.x, !vec.y, !vec.z); }
			
		};
		
		template<typename X, typename Y, typename Z>
		Vec(X, Y, Z) -> Vec<
			std::conditional_t<(std::is_null_pointer_v<X> || std::same_as<NoState, X>), void, X>,
			std::conditional_t<(std::is_null_pointer_v<Y> || std::same_as<NoState, Y>), void, Y>,
			std::conditional_t<(std::is_null_pointer_v<Z> || std::same_as<NoState, Z>), void, Z>
		>;
		
		template<typename X, typename Y>
		Vec(X, Y) -> Vec<
			std::conditional_t<(std::is_null_pointer_v<X> || std::same_as<NoState, X>), void, X>,
			std::conditional_t<(std::is_null_pointer_v<Y> || std::same_as<NoState, Y>), void, Y>, void
		>;
		
		template<typename X, typename Y, typename Z>
		Vec(Vec<X, Y, Z>) -> Vec<X, Y, Z>;
		
	}
	
	using generic_vec::Vec;
	using generic_vec::NoState;
	
	using generic_vec::dot;
	using generic_vec::cross;
	
	namespace generic_vec {
		
		template<template<typename...> typename Constraint, typename LHS, typename RHS, bool MustBeNoexcept = false>
		struct OpConstraint_t: std::bool_constant<(Constraint<LHS, RHS, std::bool_constant<MustBeNoexcept> >{}())> {};
		
		// Vectorial Operation.
		template<typename LX, typename LY, typename LZ, typename RX, typename RY, typename RZ,
		template<typename...> typename Constraint, bool MustBeNoexcept>
		struct OpConstraint_t<Constraint, Vec<LX, LY, LZ>, Vec<RX, RY, RZ>, MustBeNoexcept >:
		std::bool_constant<(
				Constraint<typename Vec<LX, LY, LZ>::value_type_x, typename Vec<RX, RY, RZ>::value_type_x, std::bool_constant<MustBeNoexcept>>{}()
			&&	Constraint<typename Vec<LX, LY, LZ>::value_type_y, typename Vec<RX, RY, RZ>::value_type_y, std::bool_constant<MustBeNoexcept>>{}()
			&&	Constraint<typename Vec<LX, LY, LZ>::value_type_z, typename Vec<RX, RY, RZ>::value_type_z, std::bool_constant<MustBeNoexcept>>{}()
		)> {};
		
		
		// Scalar Operation. Scalar Right Hand Side.
		template<typename LX, typename LY, typename LZ, typename RHS,
		template<typename...> typename Constraint, bool MustBeNoexcept>
		requires(!concepts::same_template<RHS, Vec<void>>)
		struct OpConstraint_t<Constraint, Vec<LX, LY, LZ>, RHS, MustBeNoexcept>:
		std::bool_constant<(
				Constraint<typename Vec<LX, LY, LZ>::value_type_x, RHS, std::bool_constant<MustBeNoexcept> >{}()
			&&	Constraint<typename Vec<LX, LY, LZ>::value_type_y, RHS, std::bool_constant<MustBeNoexcept> >{}()
			&&	Constraint<typename Vec<LX, LY, LZ>::value_type_z, RHS, std::bool_constant<MustBeNoexcept> >{}()
		)> {};
		
		// Scalar Operation. Scalar Left Hand Side.
		template<typename RX, typename RY, typename RZ, typename LHS,
		template<typename...> typename Constraint, bool MustBeNoexcept>
		requires(!concepts::same_template<LHS, Vec<void>>)
		struct OpConstraint_t<Constraint, LHS, Vec<RX, RY, RZ>, MustBeNoexcept>:
		std::bool_constant<(
				Constraint<LHS, typename Vec<RX, RY, RZ>::value_type_x, std::bool_constant<MustBeNoexcept> >{}()
			&&	Constraint<LHS, typename Vec<RX, RY, RZ>::value_type_y, std::bool_constant<MustBeNoexcept> >{}()
			&&	Constraint<LHS, typename Vec<RX, RY, RZ>::value_type_z, std::bool_constant<MustBeNoexcept> >{}()
		)> {};
		
		
		
		template<typename X, typename Y, typename Z, typename T, typename OVec = Vec<X, Y, Z>>
		requires( OpConstraint_t<concepts::can_mul_t, OVec, T>{}() )
		static constexpr decltype(auto)
		operator*(Vec<X, Y, Z> const& lhs, T const& rhs)
		noexcept(OpConstraint_t<concepts::can_mul_t, OVec, T, true>{}()) {
			auto&& [lx, rx] = DefaultToNoState(lhs.x, rhs);
			auto&& [ly, ry] = DefaultToNoState(lhs.y, rhs);
			auto&& [lz, rz] = DefaultToNoState(lhs.z, rhs);
			return ink::Vec(lx * rx, ly * ry, lz * rz);
		}
		
		template<typename X, typename Y, typename Z, typename T, typename OVec = Vec<X, Y, Z>>
		requires( OpConstraint_t<concepts::can_mul_t, T, OVec>{}() )
		static constexpr decltype(auto)
		operator*(T const& lhs, Vec<X, Y, Z> const& rhs)
		noexcept(OpConstraint_t<concepts::can_mul_t, T, OVec, true>{}()) {
			auto&& [lx, rx] = DefaultToNoState(lhs, rhs.x);
			auto&& [ly, ry] = DefaultToNoState(lhs, rhs.y);
			auto&& [lz, rz] = DefaultToNoState(lhs, rhs.z);
			return ink::Vec(lx * rx, ly * ry, lz * rz);
		}
		
		template<typename LX, typename LY, typename LZ, typename RX, typename RY, typename RZ,
			typename LVec = Vec<LX, LY, LZ>,
			typename RVec = Vec<RX, RY, RZ>>
		requires( OpConstraint_t<concepts::can_mul_t, LVec, RVec>{}() )
		static constexpr decltype(auto)
		operator*(Vec<LX, LY, LZ> const& lhs, Vec<RX, RY, RZ> const& rhs)
		noexcept(OpConstraint_t<concepts::can_mul_t, LVec, RVec, true>{}())
		{ return ink::Vec(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z); }
		
		
		
		template<typename X, typename Y, typename Z, typename T, typename OVec = Vec<X, Y, Z>>
		requires( OpConstraint_t<concepts::can_div_t, OVec, T>{}() )
		static constexpr decltype(auto)
		operator/(Vec<X, Y, Z> const& lhs, T const& rhs)
		noexcept(OpConstraint_t<concepts::can_div_t, OVec, T, true>{}()) {
			auto&& [lx, rx] = DefaultToNoState(lhs.x, rhs);
			auto&& [ly, ry] = DefaultToNoState(lhs.y, rhs);
			auto&& [lz, rz] = DefaultToNoState(lhs.z, rhs);
			return ink::Vec(lx / rx, ly / ry, lz / rz);
		}
		
		template<typename X, typename Y, typename Z, typename T, typename OVec = Vec<X, Y, Z>>
		requires( OpConstraint_t<concepts::can_div_t, T, OVec>{}() )
		static constexpr decltype(auto)
		operator/(T const& lhs, Vec<X, Y, Z> const& rhs)
		noexcept(OpConstraint_t<concepts::can_div_t, T, OVec, true>{}()) {
			auto&& [lx, rx] = DefaultToNoState(lhs, rhs.x);
			auto&& [ly, ry] = DefaultToNoState(lhs, rhs.y);
			auto&& [lz, rz] = DefaultToNoState(lhs, rhs.z);
			return ink::Vec(lx / rx, ly / ry, lz / rz);
		}
		
		template<typename LX, typename LY, typename LZ, typename RX, typename RY, typename RZ,
			typename LVec = Vec<LX, LY, LZ>,
			typename RVec = Vec<RX, RY, RZ>>
		requires( OpConstraint_t<concepts::can_div_t, LVec, RVec>{}() )
		static constexpr decltype(auto)
		operator/(Vec<LX, LY, LZ> const& lhs, Vec<RX, RY, RZ> const& rhs)
		noexcept(OpConstraint_t<concepts::can_div_t, LVec, RVec, true>{}())
		{ return ink::Vec(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z); }
		
		
		
		template<typename X, typename Y, typename Z, typename T, typename OVec = Vec<X, Y, Z>>
		requires( OpConstraint_t<concepts::can_mod_t, OVec, T>{}() )
		static constexpr decltype(auto)
		operator%(Vec<X, Y, Z> const& lhs, T const& rhs)
		noexcept(OpConstraint_t<concepts::can_mod_t, OVec, T, true>{}()) {
			auto&& [lx, rx] = DefaultToNoState(lhs.x, rhs);
			auto&& [ly, ry] = DefaultToNoState(lhs.y, rhs);
			auto&& [lz, rz] = DefaultToNoState(lhs.z, rhs);
			return ink::Vec(lx % rx, ly % ry, lz % rz);
		}
		
		template<typename X, typename Y, typename Z, typename T, typename OVec = Vec<X, Y, Z>>
		requires( OpConstraint_t<concepts::can_mod_t, T, OVec>{}() )
		static constexpr decltype(auto)
		operator%(T const& lhs, Vec<X, Y, Z> const& rhs)
		noexcept(OpConstraint_t<concepts::can_mod_t, T, OVec, true>{}()) {
			auto&& [lx, rx] = DefaultToNoState(lhs, rhs.x);
			auto&& [ly, ry] = DefaultToNoState(lhs, rhs.y);
			auto&& [lz, rz] = DefaultToNoState(lhs, rhs.z);
			return ink::Vec(lx % rx, ly % ry, lz % rz);
		}
		
		template<typename LX, typename LY, typename LZ, typename RX, typename RY, typename RZ,
			typename LVec = Vec<LX, LY, LZ>,
			typename RVec = Vec<RX, RY, RZ>>
		requires( OpConstraint_t<concepts::can_mod_t, LVec, RVec>{}() )
		static constexpr decltype(auto)
		operator%(Vec<LX, LY, LZ> const& lhs, Vec<RX, RY, RZ> const& rhs)
		noexcept(OpConstraint_t<concepts::can_mod_t, LVec, RVec, true>{}())
		{ return ink::Vec(lhs.x % rhs.x, lhs.y % rhs.y, lhs.z % rhs.z); }
		
		
		
		template<typename LX, typename LY, typename LZ, typename RX, typename RY, typename RZ,
			typename LVec = Vec<LX, LY, LZ>,
			typename RVec = Vec<RX, RY, RZ>>
		requires( OpConstraint_t<concepts::can_add_t, LVec, RVec>{}() )
		static constexpr decltype(auto)
		operator+(Vec<LX, LY, LZ> const& lhs, Vec<RX, RY, RZ> const& rhs)
		noexcept(OpConstraint_t<concepts::can_add_t, LVec, RVec, true>{}())
		{ return ink::Vec(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
		
		
		
		template<typename LX, typename LY, typename LZ, typename RX, typename RY, typename RZ,
			typename LVec = Vec<LX, LY, LZ>,
			typename RVec = Vec<RX, RY, RZ>>
		requires( OpConstraint_t<concepts::can_sub_t, LVec, RVec>{}() )
		static constexpr decltype(auto)
		operator-(Vec<LX, LY, LZ> const& lhs, Vec<RX, RY, RZ> const& rhs)
		noexcept(OpConstraint_t<concepts::can_sub_t, LVec, RVec, true>{}())
		{ return ink::Vec(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
		
		
		
		template<typename LX, typename LY, typename LZ, typename RX, typename RY, typename RZ,
			typename LVec = Vec<LX, LY, LZ>,
			typename RVec = Vec<RX, RY, RZ>>
		requires( OpConstraint_t<concepts::can_cmp_eq_t, LVec, RVec>{}() )
		static constexpr decltype(auto)
		operator==(Vec<LX, LY, LZ> const& lhs, Vec<RX, RY, RZ> const& rhs)
		noexcept(OpConstraint_t<concepts::can_cmp_eq_t, LVec, RVec, true>{}())
		{ return ink::Vec(lhs.x == rhs.x, lhs.y == rhs.y, lhs.z == rhs.z); }
		
		
		
		template<typename LX, typename LY, typename LZ, typename RX, typename RY, typename RZ,
			typename LVec = Vec<LX, LY, LZ>,
			typename RVec = Vec<RX, RY, RZ>>
		requires( OpConstraint_t<concepts::can_cmp_neq_t, LVec, RVec>{}() )
		static constexpr decltype(auto)
		operator!=(Vec<LX, LY, LZ> const& lhs, Vec<RX, RY, RZ> const& rhs)
		noexcept(OpConstraint_t<concepts::can_cmp_neq_t, LVec, RVec, true>{}())
		{ return ink::Vec(lhs.x != rhs.x, lhs.y != rhs.y, lhs.z != rhs.z); }
		
		
		
		template<typename LX, typename LY, typename LZ, typename RX, typename RY, typename RZ,
			typename LVec = Vec<LX, LY, LZ>,
			typename RVec = Vec<RX, RY, RZ>>
		requires( OpConstraint_t<concepts::can_cmp_threeway_t, LVec, RVec>{}() )
		static constexpr decltype(auto)
		operator<=>(Vec<LX, LY, LZ> const& lhs, Vec<RX, RY, RZ> const& rhs)
		noexcept(OpConstraint_t<concepts::can_cmp_threeway_t, LVec, RVec, true>{}())
		{ return ink::Vec(lhs.x <=> rhs.x, lhs.y <=> rhs.y, lhs.z <=> rhs.z); }
		
		
		
		template<typename LX, typename LY, typename LZ, typename RX, typename RY, typename RZ,
			typename LVec = Vec<LX, LY, LZ>,
			typename RVec = Vec<RX, RY, RZ>>
		requires( OpConstraint_t<concepts::can_cmp_greater_t, LVec, RVec>{}() )
		static constexpr decltype(auto)
		operator>(Vec<LX, LY, LZ> const& lhs, Vec<RX, RY, RZ> const& rhs)
		noexcept(OpConstraint_t<concepts::can_cmp_neq_t, LVec, RVec, true>{}())
		{ return ink::Vec(lhs.x > rhs.x, lhs.y > rhs.y, lhs.z > rhs.z); }
		
		
		
		template<typename LX, typename LY, typename LZ, typename RX, typename RY, typename RZ,
			typename LVec = Vec<LX, LY, LZ>,
			typename RVec = Vec<RX, RY, RZ>>
		requires( OpConstraint_t<concepts::can_cmp_less_t, LVec, RVec>{}() )
		static constexpr decltype(auto)
		operator<(Vec<LX, LY, LZ> const& lhs, Vec<RX, RY, RZ> const& rhs)
		noexcept(OpConstraint_t<concepts::can_cmp_less_t, LVec, RVec, true>{}())
		{ return ink::Vec(lhs.x < rhs.x, lhs.y < rhs.y, lhs.z < rhs.z); }
		
		
		
		template<typename LX, typename LY, typename LZ, typename RX, typename RY, typename RZ,
			typename LVec = Vec<LX, LY, LZ>,
			typename RVec = Vec<RX, RY, RZ>>
		requires( OpConstraint_t<concepts::can_cmp_greater_eq_t, LVec, RVec>{}() )
		static constexpr decltype(auto)
		operator>=(Vec<LX, LY, LZ> const& lhs, Vec<RX, RY, RZ> const& rhs)
		noexcept(OpConstraint_t<concepts::can_cmp_greater_eq_t, LVec, RVec, true>{}())
		{ return ink::Vec(lhs.x >= rhs.x, lhs.y >= rhs.y, lhs.z >= rhs.z); }
		
		
		
		template<typename LX, typename LY, typename LZ, typename RX, typename RY, typename RZ,
			typename LVec = Vec<LX, LY, LZ>,
			typename RVec = Vec<RX, RY, RZ>>
		requires( OpConstraint_t<concepts::can_cmp_less_eq_t, LVec, RVec>{}() )
		static constexpr decltype(auto)
		operator<=(Vec<LX, LY, LZ> const& lhs, Vec<RX, RY, RZ> const& rhs)
		noexcept(OpConstraint_t<concepts::can_cmp_less_eq_t, LVec, RVec, true>{}())
		{ return ink::Vec(lhs.x <= rhs.x, lhs.y <= rhs.y, lhs.z <= rhs.z); }
		
		
		
		template<typename LX, typename LY, typename LZ, typename RX, typename RY, typename RZ,
			typename LVec = Vec<LX, LY, LZ>,
			typename RVec = Vec<RX, RY, RZ>>
		requires( OpConstraint_t<concepts::can_logical_and_t, LVec, RVec>{}() )
		static constexpr decltype(auto)
		operator&&(Vec<LX, LY, LZ> const& lhs, Vec<RX, RY, RZ> const& rhs)
		noexcept(OpConstraint_t<concepts::can_logical_and_t, LVec, RVec, true>{}())
		{ return ink::Vec(lhs.x && rhs.x, lhs.y && rhs.y, lhs.z && rhs.z); }
		
		template<typename X, typename Y, typename Z, typename T, typename OVec = Vec<X, Y, Z>>
		requires( OpConstraint_t<concepts::can_logical_and_t, OVec, T>{}() )
		static constexpr decltype(auto)
		operator&&(Vec<X, Y, Z> const& lhs, T const& rhs)
		noexcept(OpConstraint_t<concepts::can_logical_and_t, OVec, T, true>{}())
		{ return ink::Vec(lhs.x && rhs, lhs.y && rhs, lhs.z && rhs); }
		
		template<typename X, typename Y, typename Z, typename T, typename OVec = Vec<X, Y, Z>>
		requires( OpConstraint_t<concepts::can_logical_and_t, T, OVec>{}() )
		static constexpr decltype(auto)
		operator&&(T const& lhs, Vec<X, Y, Z> const& rhs)
		noexcept(OpConstraint_t<concepts::can_logical_and_t, T, OVec, true>{}())
		{ return ink::Vec(lhs && rhs.x, lhs && rhs.y, lhs && rhs.z); }
		
		
		
		template<typename LX, typename LY, typename LZ, typename RX, typename RY, typename RZ,
			typename LVec = Vec<LX, LY, LZ>,
			typename RVec = Vec<RX, RY, RZ>>
		requires( OpConstraint_t<concepts::can_logical_or_t, LVec, RVec>{}() )
		static constexpr decltype(auto)
		operator||(Vec<LX, LY, LZ> const& lhs, Vec<RX, RY, RZ> const& rhs)
		noexcept(OpConstraint_t<concepts::can_logical_or_t, LVec, RVec, true>{}())
		{ return ink::Vec(lhs.x || rhs.x, lhs.y || rhs.y, lhs.z || rhs.z); }
		
		template<typename X, typename Y, typename Z, typename T, typename OVec = Vec<X, Y, Z>>
		requires( OpConstraint_t<concepts::can_logical_or_t, OVec, T>{}() )
		static constexpr decltype(auto)
		operator||(Vec<X, Y, Z> const& lhs, T const& rhs)
		noexcept(OpConstraint_t<concepts::can_logical_or_t, OVec, T, true>{}())
		{ return ink::Vec(lhs.x || rhs, lhs.y || rhs, lhs.z || rhs); }
		
		template<typename X, typename Y, typename Z, typename T, typename OVec = Vec<X, Y, Z>>
		requires( OpConstraint_t<concepts::can_logical_or_t, T, OVec>{}() )
		static constexpr decltype(auto)
		operator||(T const& lhs, Vec<X, Y, Z> const& rhs)
		noexcept(OpConstraint_t<concepts::can_logical_or_t, T, OVec, true>{}())
		{ return ink::Vec(lhs || rhs.x, lhs || rhs.y, lhs || rhs.z); }
		
	}
	
}

#endif
