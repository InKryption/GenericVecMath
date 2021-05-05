#include <stdio.h>
#include <cstdlib>
#include <tuple>
#include <type_traits>
#include <cmath>

namespace ink {
	
	namespace detail {
		
		template<typename T, typename U>
		struct SameTemplate_t: std::false_type {};
		
		template<template<typename...> typename Template, typename... T, typename... U>
		struct SameTemplate_t<Template<T...>, Template<U...>>: std::true_type {};
		
		template<typename T, typename U>
		concept SameTemplate = SameTemplate_t<std::remove_cvref_t<T>, std::remove_cvref_t<U>>::value;
		
	}
	
	using detail::SameTemplate;
	
	template<typename X, typename Y, typename Z> class Vec;
	
	namespace detail {
		
		enum class XYZ { X , Y , Z };
		
		template<typename T, XYZ tag> struct Axis;
		
		template<class T> struct Axis<T, XYZ::X>
		{ T x; constexpr Axis(auto&& x): x(x) {} constexpr Axis(std::nullptr_t = {}) requires(std::default_initializable<T>): x() {} };
		
		template<class T> struct Axis<T, XYZ::Y>
		{ T y; constexpr Axis(auto&& y): y(y) {} constexpr Axis(std::nullptr_t = {}) requires(std::default_initializable<T>): y() {} };
		
		template<class T> struct Axis<T, XYZ::Z>
		{ T z; constexpr Axis(auto&& z): z(z) {} constexpr Axis(std::nullptr_t = {}) requires(std::default_initializable<T>): z() {} };
		
		struct Empty {};
		
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
				template<typename ox, typename oy, typename oz> friend struct AxisGroup<ox,oy,oz>::VecBase;
				
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
		
		template<auto Op, bool PreferVoid = false>
		static constexpr decltype(auto)
		binary_op(auto&& lhs, auto&& rhs) {
			using Lhs = std::remove_cvref_t<decltype(lhs)>;
			using Rhs = std::remove_cvref_t<decltype(rhs)>;
			
			constexpr bool
				lEmpty = std::same_as<detail::Empty, Lhs>,
				rEmpty = std::same_as<detail::Empty, Rhs>;
			
			if constexpr(		(lEmpty && rEmpty)
			|| (PreferVoid &&	(lEmpty || rEmpty)))	{ return nullptr; }
			else if constexpr(!lEmpty && !rEmpty)		{ return Op(lhs, rhs); }
			else if constexpr(!lEmpty)					{ return lhs; }
			else if constexpr(!rEmpty)					{ return rhs; }
		}
		
		template<auto Op>
		static constexpr decltype(auto)
		unary_op(auto&& v) {
			using V = std::remove_cvref_t<decltype(v)>;
			constexpr bool
				vEmpty = std::same_as<detail::Empty, V>;
			if constexpr(vEmpty) { return nullptr; }
			else return Op(v);
		}
		
	}
	
	template<typename X = void, typename Y = X, typename Z = void> class Vec;
	
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
		
	template<typename X, typename Y, typename Z>
	class Vec: public detail::AxisGroup<X, Y, Z>::VecBase {
		
		private: using XYZ = detail::XYZ;
		private: using meta = detail::AxisGroup<X, Y, Z>;
		private: using base = typename meta::VecBase;
		
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
		
		
		
		public: template<typename OX, typename OY, typename OZ>
		friend constexpr decltype(auto)
		operator*(Vec const& lhs, Vec<OX, OY, OZ> const& rhs) {
			constexpr decltype(auto) mul =
			[](auto&& lhs, auto&& rhs) constexpr { return lhs * rhs; };
			return ink::Vec(
				binary_op<mul>(lhs.x, rhs.x),
				binary_op<mul>(lhs.y, rhs.y),
				binary_op<mul>(lhs.z, rhs.z)
			);
		}
		
		public: template<typename T>
		friend constexpr decltype(auto)
		operator*(Vec const& lhs, T const& rhs) {
			constexpr decltype(auto) mul =
			[](auto&& lhs, auto&& rhs) constexpr { return lhs * rhs; };
			return ink::Vec(
				binary_op<mul, true>(lhs.x, rhs),
				binary_op<mul, true>(lhs.y, rhs),
				binary_op<mul, true>(lhs.z, rhs)
			);
		}
		
		public: template<typename T>
		friend constexpr decltype(auto)
		operator*(T const& lhs, Vec const& rhs) {
			constexpr decltype(auto) mul =
			[](auto&& lhs, auto&& rhs) constexpr { return lhs * rhs; };
			return ink::Vec(
				binary_op<mul, true>(lhs, rhs.x),
				binary_op<mul, true>(lhs, rhs.y),
				binary_op<mul, true>(lhs, rhs.z)
			);
		}
		
		
		
		public: template<typename OX, typename OY, typename OZ>
		friend constexpr decltype(auto)
		operator/(Vec const& lhs, Vec<OX, OY, OZ> const& rhs) {
			constexpr decltype(auto) div =
			[](auto&& lhs, auto&& rhs) constexpr { return lhs / rhs; };
			return ink::Vec(
				binary_op<div>(lhs.x, rhs.x),
				binary_op<div>(lhs.y, rhs.y),
				binary_op<div>(lhs.z, rhs.z)
			);
		}
		
		public: template<typename T>
		friend constexpr decltype(auto)
		operator/(Vec const& lhs, T const& rhs) {
			constexpr decltype(auto) div =
			[](auto&& lhs, auto&& rhs) constexpr { return lhs / rhs; };
			return ink::Vec(
				binary_op<div, true>(lhs.x, rhs),
				binary_op<div, true>(lhs.y, rhs),
				binary_op<div, true>(lhs.z, rhs)
			);
		}
		
		public: template<typename T>
		friend constexpr decltype(auto)
		operator/(T const& lhs, Vec const& rhs) {
			constexpr decltype(auto) div =
			[](auto&& lhs, auto&& rhs) constexpr { return lhs / rhs; };
			return ink::Vec(
				detail::binary_op<div, true>(lhs, rhs.x),
				detail::binary_op<div, true>(lhs, rhs.y),
				detail::binary_op<div, true>(lhs, rhs.z)
			);
		}
		
		
		
		public: template<typename OX, typename OY, typename OZ>
		friend constexpr decltype(auto)
		operator%(Vec const& lhs, Vec<OX, OY, OZ> const& rhs) {
			constexpr decltype(auto) mod =
			[](auto&& lhs, auto&& rhs) constexpr { return lhs % rhs; };
			return ink::Vec(
				binary_op<mod>(lhs.x, rhs.x),
				binary_op<mod>(lhs.y, rhs.y),
				binary_op<mod>(lhs.z, rhs.z)
			);
		}
		
		public: template<typename T> requires(!SameTemplate<T, Vec>)
		friend constexpr decltype(auto)
		operator%(Vec const& lhs, T const& rhs) {
			constexpr decltype(auto) mod =
			[](auto&& lhs, auto&& rhs) constexpr { return lhs % rhs; };
			return ink::Vec(
				binary_op<mod, true>(lhs.x, rhs),
				binary_op<mod, true>(lhs.y, rhs),
				binary_op<mod, true>(lhs.z, rhs)
			);
		}
		
		public: template<typename T> requires(!SameTemplate<T, Vec>)
		friend constexpr decltype(auto)
		operator%(T const& lhs, Vec const& rhs) {
			constexpr decltype(auto) mod =
			[](auto&& lhs, auto&& rhs) constexpr { return lhs % rhs; };
			return ink::Vec(
				binary_op<mod, true>(lhs, rhs.x),
				binary_op<mod, true>(lhs, rhs.y),
				binary_op<mod, true>(lhs, rhs.z)
			);
		}
		
		
		
		public: template<typename OX, typename OY, typename OZ>
		friend constexpr decltype(auto)
		operator+(Vec const& lhs, Vec<OX, OY, OZ> const& rhs) {
			constexpr decltype(auto) add =
			[](auto&& lhs, auto&& rhs) constexpr { return lhs + rhs; };
			return ink::Vec(
				binary_op<add>(lhs.x, rhs.x),
				binary_op<add>(lhs.y, rhs.y),
				binary_op<add>(lhs.z, rhs.z)
			);
		}
		
		public: friend constexpr decltype(auto)
		operator+(Vec const& vec) {
			constexpr decltype(auto) plus =
			[](auto&& v) constexpr { return +v; };
			return ink::Vec(
				unary_op<plus>(vec.x),
				unary_op<plus>(vec.y),
				unary_op<plus>(vec.z)
			);
		}
		
		
		
		public: template<typename OX, typename OY, typename OZ>
		friend constexpr decltype(auto)
		operator-(Vec const& lhs, Vec<OX, OY, OZ> const& rhs) {
			constexpr decltype(auto) sub =
			[](auto&& lhs, auto&& rhs) constexpr { return lhs - rhs; };
			return ink::Vec(
				binary_op<sub>(lhs.x, rhs.x),
				binary_op<sub>(lhs.y, rhs.y),
				binary_op<sub>(lhs.z, rhs.z)
			);
		}
		
		public: friend constexpr decltype(auto)
		operator-(Vec const& vec) {
			constexpr decltype(auto) minus =
			[](auto&& v) constexpr { return -v; };
			return ink::Vec(
				unary_op<minus>(vec.x),
				unary_op<minus>(vec.y),
				unary_op<minus>(vec.z)
			);
		}
		
	};
	
	static constexpr decltype(auto)
	fmod(auto&& lhs, auto&& rhs) requires(!SameTemplate<Vec<void>,decltype(lhs)> && !SameTemplate<Vec<void>,decltype(rhs)>)
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
	
}

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr ink::Vec vec1(2.5, 7.5);
	constexpr ink::Vec vec2(3, 5, 55);
	
}
