#ifndef INK_CONCEPTS_LIB_HEADER_FILE_GUARD
#define INK_CONCEPTS_LIB_HEADER_FILE_GUARD

#include <concepts>

namespace ink::concepts {
	
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


#endif
