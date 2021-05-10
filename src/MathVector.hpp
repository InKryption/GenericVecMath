#ifndef INK_GENERIC_VEC_LIB_FILE_GUARD
#define INK_GENERIC_VEC_LIB_FILE_GUARD

#include <concepts>
#include <utility>
#include <cmath>

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
		
		
		
		namespace operator_constraints {
			
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
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs >> rhs}; } && !MustBeNoexcept) || (requires(Lhs lhs, Rhs rhs) { {lhs >> rhs} noexcept; })> {};
			
			template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
			concept can_shift_right = can_shift_right_t<Lhs, Rhs, MustBeNoexcept>::value;
			
			
			
			template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
			struct can_cmp_threeway_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs <=> rhs}; } && !MustBeNoexcept) || (requires(Lhs lhs, Rhs rhs) { {lhs <=> rhs} noexcept; })> {};
			
			template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
			concept can_cmp_threeway = can_cmp_threeway_t<Lhs, Rhs, MustBeNoexcept>::value;
			
			
			template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
			struct can_cmp_less_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs < rhs}; } && !MustBeNoexcept) || (requires(Lhs lhs, Rhs rhs) { {lhs < rhs} noexcept; })> {};
			
			template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
			concept can_cmp_less = can_cmp_less_t<Lhs, Rhs, MustBeNoexcept>::value;
			
			
			
			template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
			struct can_cmp_greater_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs > rhs}; } && !MustBeNoexcept) || (requires(Lhs lhs, Rhs rhs) { {lhs > rhs} noexcept; })> {};
			
			template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
			concept can_cmp_greater = can_cmp_greater_t<Lhs, Rhs, MustBeNoexcept>::value;
			
			
			
			template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
			struct can_cmp_less_eq_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs <= rhs}; } && !MustBeNoexcept) || (requires(Lhs lhs, Rhs rhs) { {lhs <= rhs} noexcept; })> {};
			
			template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
			concept can_cmp_less_eq = can_cmp_less_eq_t<Lhs, Rhs, MustBeNoexcept>::value;
			
			
			
			template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
			struct can_cmp_greater_eq_t:
			std::bool_constant<(requires(Lhs lhs, Rhs rhs) { {lhs >= rhs}; } && !MustBeNoexcept) || (requires(Lhs lhs, Rhs rhs) { {lhs >= rhs} noexcept; })> {};
			
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
		
		using namespace operator_constraints;
		
	}
	
	namespace generic_vec {
		
		struct Empty {
			
			template<typename,typename,typename> friend class Vec;
				
			public: constexpr
			Empty(std::nullptr_t = {})
			noexcept {}
			
			public: constexpr
			Empty(Empty const&)
			noexcept {}
			
			private: template<typename T>
			constexpr explicit
			Empty(T&&)
			noexcept {}
			
			public:
			template<std::default_initializable T>
			constexpr explicit
			operator T() const noexcept(noexcept(T())) { return T(); }
			
			private:
			constexpr decltype(auto)
			operator=(auto&&) const noexcept { return *this; }
			
			public: template<typename T>
			friend constexpr decltype(auto) operator/(T, Empty) = delete;
			
		};
		
		namespace detail {
			
			enum class XYZ: size_t
			{ X , Y , Z };
			
			template<typename T, XYZ tag> struct Member;
			template<typename T> struct Member<T, XYZ::X> { T x; };
			template<typename T> struct Member<T, XYZ::Y> { T y; };
			template<typename T> struct Member<T, XYZ::Z> { T z; };
			
			template<> struct Member<void, XYZ::X> { static constexpr Empty x{}; };
			template<> struct Member<void, XYZ::Y> { static constexpr Empty y{}; };
			template<> struct Member<void, XYZ::Z> { static constexpr Empty z{}; };
			
			template<typename T, XYZ tag>
			class Axis:
				public Member<T, tag>
			{
				
				private:
				using base = Member<T, tag>;
				
				public:
				constexpr explicit
				Axis(Empty)
				noexcept( noexcept(base{}) )
				requires( std::default_initializable<base> )
				: base{} {}
				
				public:
				template<typename U>
				requires(!std::convertible_to<std::remove_cvref_t<U>, Empty>)
				constexpr explicit
				Axis(U&& u)
				noexcept( noexcept(base{std::declval<U>()}) )
				requires( requires(U uv) { base{uv}; } )
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
			
			public: using Empty = generic_vec::Empty;
			
			
			
			public:
			constexpr
			Vec()
			noexcept(noexcept(base(Empty{}, Empty{}, Empty{})))
			requires(std::constructible_from<base, Empty, Empty, Empty>)
			: base(Empty{}, Empty{}, Empty{}) {}
			
			
			
			public:
			template<typename OX, typename OY, typename OZ>
			constexpr
			Vec(OX&& vx, OY&& vy, OZ&& vz)
			noexcept(noexcept(base(std::declval<OX>(), std::declval<OY>(), std::declval<OZ>())))
			requires(std::constructible_from<base, OX, OY, OZ>)
			: base(std::forward<OX>(vx), std::forward<OY>(vy), std::forward<OZ>(vz)) {}
			
			
			
			public:
			template<typename OX, typename OY, typename OZ = Empty>
			requires(std::constructible_from<base, OX, OY, OZ>)
			constexpr
			Vec(OX&& vx, OY&& vy)
			noexcept(noexcept(base(std::declval<OX>(), std::declval<OY>(), std::declval<OZ>())))
			: base(std::forward<OX>(vx), std::forward<OY>(vy), Empty{}) {}
			
			public:
			template<typename OX, typename OY = Empty, typename OZ>
			requires(std::constructible_from<base, OX, OY, OZ> && std::is_void_v<Y>)
			constexpr
			Vec(OX&& vx, OZ&& vz)
			noexcept(noexcept(base(std::declval<OX>(), std::declval<OY>(), std::declval<OZ>())))
			: base(std::forward<OX>(vx), Empty{}, std::forward<OZ>(vz)) {}
			
			public:
			template<typename OX = Empty, typename OY, typename OZ>
			requires(std::constructible_from<base, OX, OY, OZ> && std::is_void_v<X> && !std::is_void_v<Z>)
			constexpr
			Vec(OY&& vy, OZ&& vz)
			noexcept(noexcept(base(std::declval<OX>(), std::declval<OY>(), std::declval<OZ>())))
			: base(Empty{}, std::forward<OY>(vy), std::forward<OZ>(vz)) {}
			
			
			
			public:
			template<typename OX>
			requires(!std::is_void_v<X> && std::constructible_from<base, OX, Empty, Empty>)
			constexpr explicit
			Vec(OX&& vx)
			noexcept(noexcept(base(std::declval<OX>(), Empty{}, Empty{})))
			: base(std::forward<OX>(vx), Empty{}, Empty{}) {}
			
			public:
			template<typename OY>
			requires(std::is_void_v<X> && !std::is_void_v<Y> && std::constructible_from<base, Empty, OY, Empty>)
			constexpr explicit
			Vec(OY&& vy)
			noexcept(noexcept(base(Empty{}, std::declval<OY>(), Empty{})))
			: base(Empty{}, std::forward<OY>(vy), Empty{}) {}
			
			public:
			template<typename OZ>
			requires(std::is_void_v<X> && std::is_void_v<Y> && !std::is_void_v<Z> && std::constructible_from<base, Empty, Empty, OZ>)
			constexpr explicit
			Vec(OZ&& vz)
			noexcept(noexcept(base(Empty{}, Empty{}, std::declval<OZ>())))
			: base(Empty{}, Empty{}, std::forward<OZ>(vz)) {}
			
			
			
			public: template<typename OX, typename OY, typename OZ>
			constexpr decltype(auto)
			operator=(Vec<OX, OY, OZ> const& other)
			requires(std::is_reference_v<X> || std::is_reference_v<Y> || std::is_reference_v<Z>)
			{
				this->x = static_cast<std::remove_reference_t<value_type_x>>(other.x);
				this->y = static_cast<std::remove_reference_t<value_type_y>>(other.y);
				this->z = static_cast<std::remove_reference_t<value_type_z>>(other.z);
			}
			
		};
		
		template<typename X, typename Y, typename Z>
		Vec(X, Y, Z) -> Vec<
			std::conditional_t<(std::is_null_pointer_v<X> || std::same_as<Empty, X>), void, X>,
			std::conditional_t<(std::is_null_pointer_v<Y> || std::same_as<Empty, Y>), void, Y>,
			std::conditional_t<(std::is_null_pointer_v<Z> || std::same_as<Empty, Z>), void, Z>
		>;
		
		template<typename X, typename Y>
		Vec(X, Y) -> Vec<
			std::conditional_t<(std::is_null_pointer_v<X> || std::same_as<Empty, X>), void, X>,
			std::conditional_t<(std::is_null_pointer_v<Y> || std::same_as<Empty, Y>), void, Y>, void
		>;
		
		template<typename X>
		Vec(X) -> Vec<std::conditional_t<(std::is_null_pointer_v<X> || std::same_as<Empty, X>), void, X>, void, void>;
		
	}
	
	using generic_vec::Vec;
	using generic_vec::Empty;
	
	namespace generic_vec {
		
		template<bool PREFER_EMPTY = false, typename Op, typename Lhs, typename Rhs>
		requires(	((std::same_as<std::remove_cvref_t<Lhs>, Empty>) || (std::is_arithmetic_v<std::remove_cvref_t<Lhs>>))
		&&			((std::same_as<std::remove_cvref_t<Rhs>, Empty>) || (std::is_arithmetic_v<std::remove_cvref_t<Rhs>>)) )
		static constexpr decltype(auto)
		VecOpOut(Op op, Lhs&& lhs, Rhs&& rhs)
		{
			constexpr auto lArith = std::is_arithmetic_v<std::remove_cvref_t<Lhs>>;
			constexpr auto rArith = std::is_arithmetic_v<std::remove_cvref_t<Rhs>>;
			constexpr auto lEmpty = std::same_as<std::remove_cvref_t<Lhs>, Empty>;
			constexpr auto rEmpty = std::same_as<std::remove_cvref_t<Rhs>, Empty>;
			if		constexpr(lArith && rArith) { return op(std::forward<Lhs>(lhs), std::forward<Rhs>(rhs)); }
			else if	constexpr((lEmpty && rEmpty) || (PREFER_EMPTY && lArith) || (PREFER_EMPTY && rArith)) { return Empty{}; }
			else if	constexpr(lArith && rEmpty) { return op(std::forward<Lhs>(lhs), static_cast<Lhs>(0)); }
			else if	constexpr(lEmpty && rArith) { return op(static_cast<Lhs>(0), std::forward<Rhs>(rhs)); }
		}
		
	}
	
}

#endif
