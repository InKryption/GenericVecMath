#include <stdio.h>
#include <cstdlib>
#include <cstdint>
#include <numbers>
#include <numeric>
#include <tuple>
#include <memory>
#include <type_traits>
#include <compare>
#include <cmath>
#include <string>

namespace ink {
	
	namespace internal {
		template<typename T, typename U>
		struct SameTemplate_t: std::false_type {};
		
		template<template<typename...> typename Template, typename... T, typename... U>
		struct SameTemplate_t<Template<T...>, Template<U...>>: std::true_type {};
		
		template<typename T, typename U>
		concept SameTemplate = SameTemplate_t<T, U>::value;
	}
	
	// Empty struct which acts as a 'null' type in the Vec class, and can be targeted to achieve customized behaviour for other types.
	struct Empty {};
	
	namespace detail {
		
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
		
	}
	
	namespace detail {
		template<typename T> struct AxisX {
			
			constexpr AxisX(auto&& v)
			noexcept(noexcept(T(v)))
			requires(std::constructible_from<T, decltype(v)>)
			: x(static_cast<T>(v)) {}
			
			constexpr AxisX(std::nullptr_t = {})
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
			
			constexpr AxisY(std::nullptr_t = {})
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
			
			constexpr AxisZ(std::nullptr_t = {})
			noexcept(noexcept(T()))
			requires(std::default_initializable<T>)
			: z() {}
			
			T z;
			
		};
		
		template<> struct AxisX<void> { constexpr AxisX(auto&& = nullptr) noexcept {} static constexpr Empty x{}; };
		template<> struct AxisY<void> { constexpr AxisY(auto&& = nullptr) noexcept {} static constexpr Empty y{}; };
		template<> struct AxisZ<void> { constexpr AxisZ(auto&& = nullptr) noexcept {} static constexpr Empty z{}; };
		
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
			
			public: constexpr
			VecBase()
			noexcept(noexcept(base0()) && noexcept(base1()) && noexcept(base2()))
			requires(std::default_initializable<base0> && std::default_initializable<base1> && std::default_initializable<base2>)
			: base0(), base1(), base2() {}
			
			public: template<typename OX, typename OY, typename OZ> constexpr
			VecBase(OX&& vx, OY&& vy, OZ&& vz)
			noexcept(noexcept(baseX(std::declval<OX>())) && noexcept(baseY(std::declval<OY>())) && noexcept(baseZ(std::declval<OZ>())))
			requires(std::constructible_from<baseX, OX> && std::constructible_from<baseY, OY> && std::constructible_from<baseZ, OZ>)
			:
			base0([&]() constexpr -> decltype(auto) {
				if		constexpr(std::same_as<base0, baseX>)
				{ return std::forward<OX>(vx); }
				else if	constexpr(std::same_as<base0, baseY>)
				{ return std::forward<OY>(vy); }
				else
				{ return std::forward<OZ>(vz); } }()
			),
			base1([&]() constexpr -> decltype(auto) {
				if		constexpr(std::same_as<base1, baseX>)
				{ return std::forward<OX>(vx); }
				else if	constexpr(std::same_as<base1, baseY>)
				{ return std::forward<OY>(vy); }
				else
				{ return std::forward<OZ>(vz); } }()
			),
			base2([&]() constexpr -> decltype(auto) {
				if		constexpr(std::same_as<base2, baseX>)
				{ return std::forward<OX>(vx); }
				else if	constexpr(std::same_as<base2, baseY>)
				{ return std::forward<OY>(vy); }
				else
				{ return std::forward<OZ>(vz); } }()
			) {}
			
		};
		
	}
	
	// Flexible, anti-redundant templated math vector class.
	template<typename X, typename Y = X, typename Z = void>
	class Vec: public detail::VecBase<X, Y, Z> {
		
		private: using base = detail::VecBase<X, Y, Z>;
		
		public: using value_type_x = decltype(Vec::x);
		public: using value_type_y = decltype(Vec::y);
		public: using value_type_z = decltype(Vec::z);
		
		public: constexpr
		Vec()
		noexcept(noexcept(base()))
		requires(std::default_initializable<base>)
		: base() {}
		
		public: template<typename OX, typename OY, typename OZ> constexpr
		Vec(Vec<OX, OY, OZ> && other)
		noexcept(noexcept(base(other.x, other.y, other.z)))
		requires(std::constructible_from<base, decltype(other.x), decltype(other.y), decltype(other.z)>)
		: base(static_cast<decltype(this->x)>(other.x), static_cast<decltype(this->y)>(other.y), static_cast<decltype(this->z)>(other.z)) {}
		
		public: template<typename OX, typename OY, typename OZ> constexpr
		Vec(Vec<OX, OY, OZ> const& other)
		noexcept(noexcept(base(other.x, other.y, other.z)))
		requires(std::constructible_from<base, decltype(other.x), decltype(other.y), decltype(other.z)>)
		: base(static_cast<decltype(this->x)>(other.x), static_cast<decltype(this->y)>(other.y), static_cast<decltype(this->z)>(other.z)) {}
		
		public: template<typename OX, typename OY, typename OZ> constexpr
		Vec(Vec<OX, OY, OZ> const& other)
		: base(other.x, other.y, other.z) {}
		
		public: template<typename OX, typename OY, typename OZ> constexpr
		Vec(OX&& vx, OY&& vy, OZ&& vz)
		noexcept(noexcept(base(std::declval<OX>(), std::declval<OY>(), std::declval<OZ>())))
		requires(std::constructible_from<base, OX, OY, OZ>)
		: base(std::forward<OX>(vx), std::forward<OY>(vy), std::forward<OZ>(vz)) {}
		
		
		
		public: template<typename OX, typename OY> constexpr
		Vec(OX&& vx, OY&& vy)
		noexcept(noexcept(base(std::declval<OX>(), std::declval<OY>(), nullptr)))
		requires(
			std::constructible_from<base, OX, OY, std::nullptr_t>
		&&	(std::default_initializable<typename base::baseZ> || std::is_void_v<Z>))
		: base(std::forward<OX>(vx), std::forward<OY>(vy), nullptr) {}
		
		public: template<typename OX, typename OZ> constexpr
		Vec(OX&& vx, OZ&& vz)
		noexcept(noexcept(base(std::declval<OX>(), nullptr, std::declval<OZ>())))
		requires(
			std::constructible_from<base, OX, std::nullptr_t, OZ>
		&&	std::is_void_v<Y>)
		: base(std::forward<OX>(vx), nullptr, std::forward<OZ>(vz)) {}
		
		public: template<typename OY, typename OZ> constexpr
		Vec(OY&& vy, OZ&& vz)
		noexcept(noexcept(base(nullptr, std::declval<OY>(), std::declval<OZ>())))
		requires(
			std::constructible_from<base, std::nullptr_t, OY, OZ>
		&&	std::is_void_v<X>
		&&	!std::is_void_v<Y>
		&&	!std::is_void_v<Z>)
		: base(nullptr, std::forward<OY>(vy), std::forward<OZ>(vz)) {}
		
		
		
		public: template<typename OX> constexpr explicit
		Vec(OX&& vx)
		noexcept(noexcept(base(std::declval<OX>(), nullptr, nullptr)))
		requires(
			std::constructible_from<base, OX, std::nullptr_t, std::nullptr_t>
		&&	(std::default_initializable<typename base::baseY> || std::is_void_v<Y>)
		&&	(std::default_initializable<typename base::baseZ> || std::is_void_v<Z>))
		: base(std::forward<OX>(vx), nullptr, nullptr) {}
		
		public: template<typename OY> constexpr explicit
		Vec(OY&& vy)
		noexcept(noexcept(base(nullptr, std::declval<OY>(), nullptr)))
		requires(
			std::constructible_from<base, std::nullptr_t, OY, std::nullptr_t>
		&&	std::is_void_v<X>
		&&	std::default_initializable<typename base::baseZ>)
		: base(nullptr, std::forward<OY>(vy), nullptr) {}
		
		public: template<typename OZ> constexpr explicit
		Vec(OZ&& vz)
		noexcept(noexcept(base(nullptr, nullptr, std::declval<OZ>())))
		requires(
			std::constructible_from<base, std::nullptr_t, std::nullptr_t, OZ>
		&&	std::is_void_v<X>
		&&	std::is_void_v<Y>)
		: base(nullptr, nullptr, std::forward<OZ>(vz)) {}
		
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
	
	template<typename X>
	Vec(X) -> Vec<std::conditional_t<(std::is_null_pointer_v<X> || std::same_as<X, Empty>), void, X>, void, void>;
	
	template<typename X, typename Y, typename Z>
	Vec(Vec<X, Y, Z>) -> Vec<X, Y, Z>;
	
	namespace detail {
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept CanAdd = (requires(Lhs lhs, Rhs rhs) { {lhs + rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs + rhs} noexcept; };
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept CanSub = (requires(Lhs lhs, Rhs rhs) { {lhs - rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs - rhs} noexcept; };
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept CanMul = (requires(Lhs lhs, Rhs rhs) { {lhs * rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs * rhs} noexcept; };
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept CanDiv = (requires(Lhs lhs, Rhs rhs) { {lhs / rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs / rhs} noexcept; };
		
		template<typename Lhs, typename Rhs = Lhs, bool MustBeNoexcept = false>
		concept CanMod = (requires(Lhs lhs, Rhs rhs) { {lhs % rhs}; } && !MustBeNoexcept) || requires(Lhs lhs, Rhs rhs) { {lhs % rhs} noexcept; };
		
		template<typename Lhs, typename Rhs>
		concept Vec_CanAdd
		=	internal::SameTemplate<Vec<void>, Lhs>
		&&	internal::SameTemplate<Vec<void>, Rhs>
		&&	CanAdd<typename Lhs::value_type_x, typename Rhs::value_type_x>
		&&	CanAdd<typename Lhs::value_type_y, typename Rhs::value_type_y>
		&&	CanAdd<typename Lhs::value_type_z, typename Rhs::value_type_z>;
		
		
		
		
		template<typename LX, typename LY, typename LZ, typename RX, typename RY, typename RZ, class LVec = Vec<LX, LY, LZ>, class RVec = Vec<RX, RY, RZ>>
		requires(CanAdd<decltype(LVec::x),decltype(RVec::x)> && CanAdd<decltype(LVec::y),decltype(RVec::y)> && CanAdd<decltype(LVec::z),decltype(RVec::z)>)
		[[maybe_unused]] static constexpr decltype(auto)
		operator+(Vec<LX, LY, LZ> const& lhs, Vec<RX, RY, RZ> const& rhs)
		noexcept(CanAdd<decltype(lhs.x),decltype(rhs.x),true> && CanAdd<decltype(lhs.y),decltype(rhs.y),true> && CanAdd<decltype(lhs.z),decltype(rhs.z),true>)
		{ return ink::Vec(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
		
		
		
		template<typename LX, typename LY, typename LZ, typename RX, typename RY, typename RZ, class LVec = Vec<LX, LY, LZ>, class RVec = Vec<RX, RY, RZ>>
		requires(CanSub<decltype(LVec::x),decltype(RVec::x)> && CanSub<decltype(LVec::y),decltype(RVec::y)> && CanSub<decltype(LVec::z),decltype(RVec::z)>)
		[[maybe_unused]] static constexpr decltype(auto)
		operator-(Vec<LX, LY, LZ> const& lhs, Vec<RX, RY, RZ> const& rhs)
		noexcept(CanSub<decltype(lhs.x),decltype(rhs.x),true> && CanSub<decltype(lhs.y),decltype(rhs.y),true> && CanSub<decltype(lhs.z),decltype(rhs.z),true>)
		{ return ink::Vec(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
		
		
		
		template<typename LX, typename LY, typename LZ, typename RX, typename RY, typename RZ, class LVec = Vec<LX, LY, LZ>, class RVec = Vec<RX, RY, RZ>>
		requires(CanMul<decltype(LVec::x),decltype(RVec::x)> && CanMul<decltype(LVec::y),decltype(RVec::y)> && CanMul<decltype(LVec::z),decltype(RVec::z)>)
		[[maybe_unused]] static constexpr decltype(auto)
		operator*(Vec<LX, LY, LZ> const& lhs, Vec<RX, RY, RZ> const& rhs)
		noexcept(CanMul<decltype(lhs.x),decltype(rhs.x),true> && CanMul<decltype(lhs.y),decltype(rhs.y),true> && CanMul<decltype(lhs.z),decltype(rhs.z),true>)
		{ return ink::Vec(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z); }
		
		
		
		template<typename LX, typename LY, typename LZ, typename RX, typename RY, typename RZ, class LVec = Vec<LX, LY, LZ>, class RVec = Vec<RX, RY, RZ>>
		requires(CanDiv<decltype(LVec::x),decltype(RVec::x)> && CanDiv<decltype(LVec::y),decltype(RVec::y)> && CanDiv<decltype(LVec::z),decltype(RVec::z)>)
		[[maybe_unused]] static constexpr decltype(auto)
		operator/(Vec<LX, LY, LZ> const& lhs, Vec<RX, RY, RZ> const& rhs)
		noexcept(CanDiv<decltype(lhs.x),decltype(rhs.x),true> && CanDiv<decltype(lhs.y),decltype(rhs.y),true> && CanDiv<decltype(lhs.z),decltype(rhs.z),true>)
		{ return ink::Vec(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z); }
		
		
		
		template<typename LX, typename LY, typename LZ, typename RX, typename RY, typename RZ, class LVec = Vec<LX, LY, LZ>, class RVec = Vec<RX, RY, RZ>>
		requires(CanMod<decltype(LVec::x),decltype(RVec::x)> && CanMod<decltype(LVec::y),decltype(RVec::y)> && CanMod<decltype(LVec::z),decltype(RVec::z)>)
		[[maybe_unused]] static constexpr decltype(auto)
		operator%(Vec<LX, LY, LZ> const& lhs, Vec<RX, RY, RZ> const& rhs)
		noexcept(CanMod<decltype(lhs.x),decltype(rhs.x),true> && CanMod<decltype(lhs.y),decltype(rhs.y),true> && CanMod<decltype(lhs.z),decltype(rhs.z),true>)
		{ return ink::Vec(lhs.x % rhs.x, lhs.y % rhs.y, lhs.z % rhs.z); }
		
	}
	
}

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	
	
	return 0;
}
