#ifndef INK_MATH_VECTOR_HEADER_FILE_GUARD
#define INK_MATH_VECTOR_HEADER_FILE_GUARD

#include <cstdlib>
#include <cstdint>
#include <utility>
#include <type_traits>
#include <cmath>
#include <concepts>
#include <tuple>

namespace ink {
	
	namespace Ops {
		#define DEF_LAMBDA_OP(name, vars, req_vars, expr)	\
		static constexpr decltype(auto) name = [] vars constexpr noexcept(noexcept(expr)) requires requires req_vars { {expr}; } { return expr; };
		
		DEF_LAMBDA_OP(POSTFIX_INC, (auto&& v), (decltype(v) v), v++)
		DEF_LAMBDA_OP(POSTFIX_DEC, (auto&& v), (decltype(v) v), v--)
		
		DEF_LAMBDA_OP(PREFIX_INC, (auto&& v), (decltype(v) v), ++v)
		DEF_LAMBDA_OP(PREFIX_DEC, (auto&& v), (decltype(v) v), --v)
		
		DEF_LAMBDA_OP(BITWISE_NOT, (auto&& v), (decltype(v) v), ~v)
		DEF_LAMBDA_OP(LOGICAL_NOT, (auto&& v), (decltype(v) v), !v)
		
		DEF_LAMBDA_OP(UNARY_ADD, (auto&& v), (decltype(v) v), +v)
		DEF_LAMBDA_OP(UNARY_SUB, (auto&& v), (decltype(v) v), -v)
		
		DEF_LAMBDA_OP(MUL, (auto&& lhs, auto&& rhs), (decltype(lhs) lhs, decltype(rhs) rhs), lhs * rhs)
		DEF_LAMBDA_OP(DIV, (auto&& lhs, auto&& rhs), (decltype(lhs) lhs, decltype(rhs) rhs), lhs / rhs)
		DEF_LAMBDA_OP(MOD, (auto&& lhs, auto&& rhs), (decltype(lhs) lhs, decltype(rhs) rhs), lhs % rhs)
		
		DEF_LAMBDA_OP(ADD, (auto&& lhs, auto&& rhs), (decltype(lhs) lhs, decltype(rhs) rhs), lhs + rhs)
		DEF_LAMBDA_OP(SUB, (auto&& lhs, auto&& rhs), (decltype(lhs) lhs, decltype(rhs) rhs), lhs - rhs)
		
		DEF_LAMBDA_OP(SHL, (auto&& lhs, auto&& rhs), (decltype(lhs) lhs, decltype(rhs) rhs), lhs << rhs)
		DEF_LAMBDA_OP(SHR, (auto&& lhs, auto&& rhs), (decltype(lhs) lhs, decltype(rhs) rhs), lhs >> rhs)
		
		DEF_LAMBDA_OP(CMP_SPACESHIP, (auto&& lhs, auto&& rhs), (decltype(lhs) lhs, decltype(rhs) rhs), lhs <=> rhs)
		
		DEF_LAMBDA_OP(CMP_LT, (auto&& lhs, auto&& rhs), (decltype(lhs) lhs, decltype(rhs) rhs), lhs < rhs)
		DEF_LAMBDA_OP(CMP_GT, (auto&& lhs, auto&& rhs), (decltype(lhs) lhs, decltype(rhs) rhs), lhs > rhs)
		
		DEF_LAMBDA_OP(CMP_LTEQ, (auto&& lhs, auto&& rhs), (decltype(lhs) lhs, decltype(rhs) rhs), lhs <= rhs)
		DEF_LAMBDA_OP(CMP_GTEQ, (auto&& lhs, auto&& rhs), (decltype(lhs) lhs, decltype(rhs) rhs), lhs >= rhs)
		
		DEF_LAMBDA_OP(CMP_EQ, (auto&& lhs, auto&& rhs), (decltype(lhs) lhs, decltype(rhs) rhs), lhs == rhs)
		DEF_LAMBDA_OP(CMP_NEQ, (auto&& lhs, auto&& rhs), (decltype(lhs) lhs, decltype(rhs) rhs), lhs != rhs)
		
		DEF_LAMBDA_OP(BITWISE_AND, (auto&& lhs, auto&& rhs), (decltype(lhs) lhs, decltype(rhs) rhs), lhs & rhs)
		DEF_LAMBDA_OP(BITWISE_XOR, (auto&& lhs, auto&& rhs), (decltype(lhs) lhs, decltype(rhs) rhs), lhs ^ rhs)
		DEF_LAMBDA_OP(BITWISE_OR, (auto&& lhs, auto&& rhs), (decltype(lhs) lhs, decltype(rhs) rhs), lhs | rhs)
		
		DEF_LAMBDA_OP(LOGICAL_AND, (auto&& lhs, auto&& rhs), (decltype(lhs) lhs, decltype(rhs) rhs), lhs && rhs)
		DEF_LAMBDA_OP(LOGICAL_OR, (auto&& lhs, auto&& rhs), (decltype(lhs) lhs, decltype(rhs) rhs), lhs || rhs)
		
		#undef DEF_LAMBDA_OP
	}
	
	namespace detail {
		
		template<typename T, typename U>
		struct SameTemplate_t: std::false_type {};
		
		template<template<typename...> typename Template, typename... T, typename... U>
		struct SameTemplate_t<Template<T...>, Template<U...>>: std::true_type {};
		
		template<typename T, typename U>
		concept SameTemplate = SameTemplate_t<std::remove_cvref_t<T>, std::remove_cvref_t<U>>::value;
		
	}
	
	using detail::SameTemplate;
	
	template<typename X, typename Y = X, typename Z = void> class Vec;
	
	namespace detail {
		
		enum class XYZ { X , Y , Z };
		
		template<typename T, XYZ tag> struct Axis;
		
		template<class T> struct Axis<T, XYZ::X>
		{ T x; constexpr Axis(auto&& x): x(x) {} constexpr Axis(std::nullptr_t = {}) requires(std::default_initializable<T>): x() {} };
		
		template<class T> struct Axis<T, XYZ::Y>
		{ T y; constexpr Axis(auto&& y): y(y) {} constexpr Axis(std::nullptr_t = {}) requires(std::default_initializable<T>): y() {} };
		
		template<class T> struct Axis<T, XYZ::Z>
		{ T z; constexpr Axis(auto&& z): z(z) {} constexpr Axis(std::nullptr_t = {}) requires(std::default_initializable<T>): z() {} };
		
		struct Empty { constexpr Empty() {} };
		
		template<> struct Axis<void, XYZ::X> { constexpr Axis(std::nullptr_t = {}) {} static constexpr Empty x{}; };
		template<> struct Axis<void, XYZ::Y> { constexpr Axis(std::nullptr_t = {}) {} static constexpr Empty y{}; };
		template<> struct Axis<void, XYZ::Z> { constexpr Axis(std::nullptr_t = {}) {} static constexpr Empty z{}; };
		
		template<typename X, typename Y, typename Z>
		struct AxisGroup {
			
			template<typename, typename, typename> friend class Vec;
			template<typename, typename, typename> friend struct VecBase;
			template<typename, typename, typename> friend struct AxisGroup;
			
			public: using XYZ = XYZ;
			
			public: template<XYZ tag>
			using value_type = std::conditional_t<(tag == XYZ::X), X, std::conditional_t<(tag == XYZ::Y), Y, Z>>;
			
			public: template<XYZ tag>
			using real_type = std::conditional_t<
			(std::same_as<void, value_type<tag>>),
				decltype(nullptr),
				value_type<tag>
			>;
			
			template<XYZ tag> using Axis = Axis<value_type<tag>, tag>;
			
			private: static constexpr size_t
				sx = [] () { if constexpr(std::is_void_v<X>) { return 1; } else { return sizeof(X); } }(),
				sy = [] () { if constexpr(std::is_void_v<Y>) { return 1; } else { return sizeof(Y); } }(),
				sz = [] () { if constexpr(std::is_void_v<Z>) { return 1; } else { return sizeof(Z); } }();
			private: static constexpr auto
				XY = sx <=> sy,
				XZ = sx <=> sz,
				YZ = sy <=> sz;
			
			private: template<XYZ tag> static constexpr size_t
			IndexOfAxis =	(XYZ::X == tag) ? 2 - ( (XY >= 0) + (XZ >= 0) ) :
							(XYZ::Y == tag) ? 2 - ( (XY <  0) + (YZ >= 0) ) :
							(XYZ::Z == tag) ? 2 - ( (XZ <  0) + (YZ <  0) ) : -1;
			
			private: template<size_t i> static constexpr XYZ
			AxisAtIndex =	(IndexOfAxis<XYZ::X> == i) ? XYZ::X :
							(IndexOfAxis<XYZ::Y> == i) ? XYZ::Y :
							(IndexOfAxis<XYZ::Z> == i) ? XYZ::Z : static_cast<XYZ>(-1);
			
			private: template<size_t i> static constexpr decltype(auto)
			fwd_value(auto&& x, auto&& y, auto&& z) {
				if constexpr(XYZ::X == AxisAtIndex<i>) return x;
				if constexpr(XYZ::Y == AxisAtIndex<i>) return y;
				if constexpr(XYZ::Z == AxisAtIndex<i>) return z;
			}
			
			private: using base0 = Axis<AxisAtIndex<0>>;
			private: using base1 = Axis<AxisAtIndex<1>>;
			private: using base2 = Axis<AxisAtIndex<2>>;
			
			private: using baseX = Axis<XYZ::X>;
			private: using baseY = Axis<XYZ::Y>;
			private: using baseZ = Axis<XYZ::Z>;
			
			public: struct VecBase:
				public base0,
				public base1,
				public base2
			{
				
				template<typename, typename, typename> friend class Vec;
				template<typename, typename, typename> friend struct AxisGroup;
				
				public: constexpr
				VecBase()
				noexcept( noexcept(base0()) && noexcept(base1()) && noexcept(base2()) )
				requires(std::default_initializable<base0> && std::default_initializable<base1> && std::default_initializable<base2>)
				:	base0(),
					base1(),
					base2() {}
				
				public: constexpr
				VecBase(auto&& x, auto&& y, auto&& z)
				noexcept( noexcept(baseX(x)) && noexcept(baseY(y)) && noexcept(baseZ(z)) )
				requires(std::constructible_from<baseX, decltype(x)> && std::constructible_from<baseY, decltype(y)> && std::constructible_from<baseZ, decltype(z)>)
				:	base0(fwd_value<0>(x, y, z)),
					base1(fwd_value<1>(x, y, z)),
					base2(fwd_value<2>(x, y, z)) {}
				
			};
			
		};
		
		template<typename Op, bool PreferVoid = false, typename If_Void = std::nullptr_t, typename T, typename U>
		static constexpr decltype(auto)
		binary_op(T&& lhs, U&& rhs) {
			using Lhs = std::remove_cvref_t<T>;
			using Rhs = std::remove_cvref_t<U>;
			
			constexpr bool
				lEmpty = std::same_as<detail::Empty, Lhs>,
				rEmpty = std::same_as<detail::Empty, Rhs>;
			
			if constexpr(		(lEmpty && rEmpty)
			|| (PreferVoid &&	(lEmpty || rEmpty)))	{ return If_Void(); }
			else if constexpr(!lEmpty && !rEmpty)		{ return Op{}(std::forward<T>(lhs), std::forward<U>(rhs)); }
			else if constexpr(!lEmpty)					{ return lhs; }
			else if constexpr(!rEmpty)					{ return rhs; }
		}
		
		template<typename Op>
		static constexpr decltype(auto)
		unary_op(auto&& v) {
			using V = std::remove_cvref_t<decltype(v)>;
			constexpr bool
				vEmpty = std::same_as<detail::Empty, V>;
			if constexpr(vEmpty) { return nullptr; }
			else return Op{}(v);
		}
		
	}
	
	template<typename X, typename Y, typename Z>
	class Vec: public detail::AxisGroup<X, Y, Z>::VecBase {
		
		private: using XYZ = detail::XYZ;
		private: using meta = detail::AxisGroup<X, Y, Z>;
		private: using base = typename meta::VecBase;
		
		public: constexpr
		Vec(SameTemplate<Vec> auto&& other)
		: base(other.x, other.y, other.z) {}
		
		public: constexpr
		Vec() noexcept(noexcept(base())) requires(std::default_initializable<base>)
		: base() {}
		
		public: constexpr
		Vec(auto&& x, auto&& y, auto&& z)
		noexcept(noexcept(base(x, y, z)))
		: base(x, y, z) {}
		
		public: constexpr
		Vec(auto&& x, auto&& y)
		noexcept( noexcept(base(x, y, nullptr)) )
		requires(std::is_void_v<Z> || std::default_initializable<Z>)
		: base(x, y, nullptr) {}
		
		public: constexpr
		Vec(auto&& x, auto&& z)
		noexcept( noexcept(base(x, nullptr, z)) )
		requires(!std::is_void_v<X> && std::is_void_v<Y> && !std::is_void_v<Z>)
		: base(x, nullptr, z) {}
		
		public: constexpr
		Vec(auto&& y, auto&& z)
		noexcept( noexcept(base(nullptr, y, z)) )
		requires(std::is_void_v<X> && !std::is_void_v<Y> && !std::is_void_v<Z>)
		: base(nullptr, y, z) {}
		
		public: constexpr explicit
		Vec(auto&& x)
		noexcept( noexcept(base(x, nullptr, nullptr)) )
		requires( (std::is_void_v<Y> || std::default_initializable<Y>) && (std::is_void_v<Z> || std::default_initializable<Z>) )
		: base(x, nullptr, nullptr) {}
		
		public: constexpr explicit
		Vec(auto&& y)
		noexcept( noexcept(base(nullptr, y, nullptr)) )
		requires( std::is_void_v<X> && !std::is_void_v<Y> && (std::is_void_v<Z> || std::default_initializable<Z>) )
		: base(nullptr, y, nullptr) {}
		
		public: constexpr explicit
		Vec(auto&& z)
		noexcept( noexcept(base(nullptr, nullptr, z)) )
		requires( std::is_void_v<X> && std::is_void_v<Y> && !std::is_void_v<Z> )
		: base(nullptr, nullptr, z) {}
		
		
		
		// Returns the dot product of the two given vectors.
		public: template<typename OX, typename OY, typename OZ>
		friend constexpr decltype(auto)
		dot(Vec const& lhs, Vec<OX, OY, OZ> const& rhs) {
			decltype(auto) x = detail::binary_op<Ops::MUL, false, detail::Empty>(lhs.x, rhs.x);
			decltype(auto) y = detail::binary_op<Ops::MUL, false, detail::Empty>(lhs.y, rhs.y);
			decltype(auto) z = detail::binary_op<Ops::MUL, false, detail::Empty>(lhs.z, rhs.z);
			
			decltype(auto) xy = detail::binary_op<Ops::ADD>(x, y);
			const auto xyz = detail::binary_op<Ops::ADD>(xy, z);
			
			return xyz;
		}
		
		// Returns the cross vector product of the two given vectors.
		public: template<typename OX, typename OY, typename OZ>
		friend constexpr decltype(auto)
		cross(Vec const& lhs, Vec<OX, OY, OZ> const& rhs) {
			
			using detail::binary_op;
			
			decltype(auto) x_out = detail::binary_op<decltype(Ops::SUB), true>(
				detail::binary_op<decltype(Ops::MUL), false, detail::Empty>(lhs.y, rhs.z),
				detail::binary_op<decltype(Ops::MUL), false, detail::Empty>(lhs.z, rhs.y));
			decltype(auto) y_out = detail::binary_op<decltype(Ops::SUB), true>(
				detail::binary_op<decltype(Ops::MUL), false, detail::Empty>(lhs.z, rhs.x),
				detail::binary_op<decltype(Ops::MUL), false, detail::Empty>(lhs.x, rhs.z));
			decltype(auto) z_out = detail::binary_op<decltype(Ops::SUB), true>(
				detail::binary_op<decltype(Ops::MUL), false, detail::Empty>(lhs.x, rhs.y),
				detail::binary_op<decltype(Ops::MUL), false, detail::Empty>(lhs.y, rhs.x));
			return ink::Vec(x_out, y_out, z_out);
		}
		
		// Returns the value of the magnitude quared. This can be passed into whatever square root function which is desired.
		public: friend constexpr decltype(auto)
		mag2(Vec const& v)
		{ return dot(v, v);	}
		
		
	};
	
	template<typename X, typename Y, typename Z> Vec(Vec<X,Y,Z>) -> Vec<X, Y, Z>;
	
	Vec() -> Vec<void, void, void>;
	template<typename X, typename Y, typename Z> Vec(X, Y, Z) -> Vec<
		std::conditional_t<(std::same_as<X, std::nullptr_t>), void, X>,
		std::conditional_t<(std::same_as<Y, std::nullptr_t>), void, Y>,
		std::conditional_t<(std::same_as<Z, std::nullptr_t>), void, Z>	>;
	
	template<typename X, typename Y> Vec(X, Y) -> Vec<
		std::conditional_t<(std::same_as<X, std::nullptr_t>), void, X>,
		std::conditional_t<(std::same_as<Y, std::nullptr_t>), void, Y>,
		void	>;
	
	template<typename X> Vec(X) -> Vec<
		std::conditional_t<(std::same_as<X, std::nullptr_t>), void, X>,
		void, void	>;
	
	namespace detail {
		
		template<typename X, typename Y, typename Z, typename OX, typename OY, typename OZ>
		static constexpr decltype(auto)
		operator*(Vec<X, Y, Z> const& lhs, Vec<OX, OY, OZ> const& rhs) {
			return ink::Vec(
				detail::binary_op<decltype(Ops::MUL)>(lhs.x, rhs.x),
				detail::binary_op<decltype(Ops::MUL)>(lhs.y, rhs.y),
				detail::binary_op<decltype(Ops::MUL)>(lhs.z, rhs.z)
			);
		}
		
		template<typename X, typename Y, typename Z, typename T>
		static constexpr decltype(auto)
		operator*(Vec<X, Y, Z> const& lhs, T const& rhs) {
			return ink::Vec(
				detail::binary_op<decltype(Ops::MUL), true>(lhs.x, rhs),
				detail::binary_op<decltype(Ops::MUL), true>(lhs.y, rhs),
				detail::binary_op<decltype(Ops::MUL), true>(lhs.z, rhs)
			);
		}
		
		template<typename X, typename Y, typename Z, typename T>
		static constexpr decltype(auto)
		operator*(T const& lhs, Vec<X, Y, Z> const& rhs) {
			return ink::Vec(
				detail::binary_op<decltype(Ops::MUL), true>(lhs, rhs.x),
				detail::binary_op<decltype(Ops::MUL), true>(lhs, rhs.y),
				detail::binary_op<decltype(Ops::MUL), true>(lhs, rhs.z)
			);
		}
		
		
		
		template<typename X, typename Y, typename Z, typename OX, typename OY, typename OZ>
		static constexpr decltype(auto)
		operator/(Vec<X, Y, Z> const& lhs, Vec<OX, OY, OZ> const& rhs) {
			return ink::Vec(
				detail::binary_op<decltype(Ops::DIV)>(lhs.x, rhs.x),
				detail::binary_op<decltype(Ops::DIV)>(lhs.y, rhs.y),
				detail::binary_op<decltype(Ops::DIV)>(lhs.z, rhs.z)
			);
		}
		
		template<typename X, typename Y, typename Z, typename T>
		static constexpr decltype(auto)
		operator/(Vec<X, Y, Z> const& lhs, T const& rhs) {
			return ink::Vec(
				detail::binary_op<decltype(Ops::DIV), true>(lhs.x, rhs),
				detail::binary_op<decltype(Ops::DIV), true>(lhs.y, rhs),
				detail::binary_op<decltype(Ops::DIV), true>(lhs.z, rhs)
			);
		}
		
		template<typename X, typename Y, typename Z, typename T>
		static constexpr decltype(auto)
		operator/(T const& lhs, Vec<X, Y, Z> const& rhs) {
			return ink::Vec(
				detail::binary_op<decltype(Ops::DIV), true>(lhs, rhs.x),
				detail::binary_op<decltype(Ops::DIV), true>(lhs, rhs.y),
				detail::binary_op<decltype(Ops::DIV), true>(lhs, rhs.z)
			);
		}
		
		
		
		template<typename X, typename Y, typename Z, typename OX, typename OY, typename OZ>
		static constexpr decltype(auto)
		operator%(Vec<X, Y, Z> const& lhs, Vec<OX, OY, OZ> const& rhs) {
			constexpr decltype(auto) mod =
			[](auto&& lhs, auto&& rhs) constexpr { return lhs % rhs; };
			return ink::Vec(
				detail::binary_op<decltype(Ops::MOD)>(lhs.x, rhs.x),
				detail::binary_op<decltype(Ops::MOD)>(lhs.y, rhs.y),
				detail::binary_op<decltype(Ops::MOD)>(lhs.z, rhs.z)
			);
		}
		
		template<typename X, typename Y, typename Z, typename T> requires(!SameTemplate<T, Vec<void>>)
		static constexpr decltype(auto)
		operator%(Vec<X, Y, Z> const& lhs, T const& rhs) {
			return ink::Vec(
				detail::binary_op<decltype(Ops::MOD), true>(lhs.x, rhs),
				detail::binary_op<decltype(Ops::MOD), true>(lhs.y, rhs),
				detail::binary_op<decltype(Ops::MOD), true>(lhs.z, rhs)
			);
		}
		
		template<typename X, typename Y, typename Z, typename T> requires(!SameTemplate<T, Vec<void>>)
		static constexpr decltype(auto)
		operator%(T const& lhs, Vec<X, Y, Z> const& rhs) {
			return ink::Vec(
				detail::binary_op<decltype(Ops::MOD), true>(lhs, rhs.x),
				detail::binary_op<decltype(Ops::MOD), true>(lhs, rhs.y),
				detail::binary_op<decltype(Ops::MOD), true>(lhs, rhs.z)
			);
		}
		
		
		
		template<typename X, typename Y, typename Z, typename OX, typename OY, typename OZ>
		static constexpr decltype(auto)
		operator+(Vec<X, Y, Z> const& lhs, Vec<OX, OY, OZ> const& rhs) {
			return ink::Vec(
				detail::binary_op<decltype(Ops::ADD)>(lhs.x, rhs.x),
				detail::binary_op<decltype(Ops::ADD)>(lhs.y, rhs.y),
				detail::binary_op<decltype(Ops::ADD)>(lhs.z, rhs.z)
			);
		}
		
		template<typename X, typename Y, typename Z>
		static constexpr decltype(auto)
		operator+(Vec<X, Y, Z> const& vec) {
			return ink::Vec(
				detail::unary_op<decltype(Ops::UNARY_ADD)>(vec.x),
				detail::unary_op<decltype(Ops::UNARY_ADD)>(vec.y),
				detail::unary_op<decltype(Ops::UNARY_ADD)>(vec.z)
			);
		}
		
		
		
		template<typename X, typename Y, typename Z, typename OX, typename OY, typename OZ>
		static constexpr decltype(auto)
		operator-(Vec<X, Y, Z> const& lhs, Vec<OX, OY, OZ> const& rhs) {
			return ink::Vec(
				detail::binary_op<decltype(Ops::SUB)>(lhs.x, rhs.x),
				detail::binary_op<decltype(Ops::SUB)>(lhs.y, rhs.y),
				detail::binary_op<decltype(Ops::SUB)>(lhs.z, rhs.z)
			);
		}
		
		template<typename X, typename Y, typename Z>
		static constexpr decltype(auto)
		operator-(Vec<X, Y, Z> const& vec) {
			return ink::Vec(
				detail::unary_op<decltype(Ops::UNARY_SUB)>(vec.x),
				detail::unary_op<decltype(Ops::UNARY_SUB)>(vec.y),
				detail::unary_op<decltype(Ops::UNARY_SUB)>(vec.z)
			);
		}
		
	}
	
	template<typename T, typename U>
	requires(!SameTemplate<Vec<void>, T> && !SameTemplate<Vec<void>, U>)
	static constexpr decltype(auto)
	fmod(T&& lhs, U&& rhs)
	{ return lhs - (rhs * static_cast<size_t>(lhs/rhs)); }
	
	static constexpr decltype(auto)
	fmod(SameTemplate<Vec<void>> auto&& lhs, SameTemplate<Vec<void>> auto&& rhs) {
		constexpr decltype(auto) mod =
		[](auto&& lhs, auto&& rhs) constexpr
		{ return lhs - (rhs * static_cast<size_t>(lhs/rhs)); };
		return ink::Vec(
			detail::binary_op<mod>(lhs.x, rhs.x),
			detail::binary_op<mod>(lhs.y, rhs.y),
			detail::binary_op<mod>(lhs.z, rhs.z)
		);
	}
	
	template<typename T> requires(!SameTemplate<T, Vec<void>>)
	static constexpr decltype(auto)
	fmod(SameTemplate<Vec<void>> auto&& lhs, T const& rhs) {
		constexpr decltype(auto) mod =
		[](auto&& lhs, auto&& rhs) constexpr
		{ return lhs - (rhs * static_cast<size_t>(lhs/rhs)); };
		return ink::Vec(
			detail::binary_op<mod, true>(lhs.x, rhs),
			detail::binary_op<mod, true>(lhs.y, rhs),
			detail::binary_op<mod, true>(lhs.z, rhs)
		);
	}
	
	template<typename T> requires(!SameTemplate<T, Vec<void>>)
	static constexpr decltype(auto)
	fmod(T const& lhs, SameTemplate<Vec<void>> auto&& rhs) {
		constexpr decltype(auto) mod =
		[](auto&& lhs, auto&& rhs) constexpr
		{ return lhs - (rhs * static_cast<size_t>(lhs/rhs)); };
		return ink::Vec(
			detail::binary_op<mod, true>(lhs, rhs.x),
			detail::binary_op<mod, true>(lhs, rhs.y),
			detail::binary_op<mod, true>(lhs, rhs.z)
		);
	}
	
	
	
	// Ripped code from the wikipedia page on Q_rsqrt.
	inline float Q_rsqrt( float number )
	{
		constexpr float threehalfs = 1.5F;
		const float x2 = number * 0.5F;
		
		union { float f; uint32_t i; }
		conv  = { .f = number };
		
		conv.i = 0x5f3759df - ( conv.i >> 1 );
		conv.f *= threehalfs - ( x2 * conv.f * conv.f );
		return conv.f;
	}
	
	// Double overload utilising the double-specific magic number.
	inline double Q_rsqrt( double number )
	{
		constexpr double threehalfs = 1.5;
		const float x2 = number * 0.5;
		
		union { double f; uint64_t i; }
		conv  = { .f = number };
		
		conv.i = 0x5fe6eb50c7b537a9 - ( conv.i >> 1 );
		conv.f *= threehalfs - ( x2 * conv.f * conv.f );
		return conv.f;
	}
	
}

#endif
