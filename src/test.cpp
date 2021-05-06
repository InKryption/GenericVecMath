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

namespace ink {
	
	namespace detail {
		
		using Empty = unsigned char;
		
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
		
		template<> struct AxisX<void> { constexpr AxisX(std::nullptr_t = {}) noexcept {} static constexpr Empty x{}; };
		template<> struct AxisY<void> { constexpr AxisY(std::nullptr_t = {}) noexcept {} static constexpr Empty y{}; };
		template<> struct AxisZ<void> { constexpr AxisZ(std::nullptr_t = {}) noexcept {} static constexpr Empty z{}; };
		
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
			
			public: constexpr
			VecBase(auto&& vx, auto&& vy, auto&& vz)
			noexcept(noexcept(baseX(vx)) && noexcept(baseY(vy)) && noexcept(baseZ(vz)))
			requires(std::constructible_from<baseX, decltype(vx)> && std::constructible_from<baseY, decltype(vy)> && std::constructible_from<baseZ, decltype(vz)>)
			:
				base0([&]() constexpr { if constexpr(std::same_as<base0, baseX>) {return vx;} else if constexpr(std::same_as<base0, baseY>) {return vy;} else {return vz;} }()),
				base1([&]() constexpr { if constexpr(std::same_as<base1, baseX>) {return vx;} else if constexpr(std::same_as<base1, baseY>) {return vy;} else {return vz;} }()),
				base2([&]() constexpr { if constexpr(std::same_as<base2, baseX>) {return vx;} else if constexpr(std::same_as<base2, baseY>) {return vy;} else {return vz;} }()) {}
			
		};
		
	}
	
	template<typename X, typename Y, typename Z>
	class Vec: detail::VecBase<X, Y, Z> {
		
		private: using base = detail::VecBase<X, Y, Z>;
		
		public: constexpr
		Vec()
		noexcept(noexcept(base()))
		requires(std::default_initializable<base>)
		: base() {}
		
		public: template<typename OX, typename OY, typename OZ> constexpr
		Vec(OX&& vx, OY&& vy, OZ&& vz)
		noexcept(noexcept(base(std::declval<OX>(), std::declval<OY>(), std::declval<OZ>())))
		requires(std::constructible_from<base, OX, OY, OZ>)
		: base(std::forward<OX>(vx), std::forward<OY>(vy), std::forward<OZ>(vz)) {}
		
		
		
		public: template<typename OX, typename OY> constexpr
		Vec(OX&& vx, OY&& vy)
		noexcept(noexcept(base(std::declval<OX>(), std::declval<OY>(), nullptr)))
		requires(std::constructible_from<base, OX, OY, std::nullptr_t>
		&& std::default_initializable<typename base::baseZ>)
		: base(std::forward<OX>(vx), std::forward<OY>(vy), nullptr) {}
		
		public: template<typename OY, typename OZ> constexpr
		Vec(OY&& vy, OZ&& vz)
		noexcept(noexcept(base(nullptr, std::declval<OY>(), std::declval<OZ>())))
		requires(std::constructible_from<base, std::nullptr_t, OY, OZ>
		&& std::is_void_v<X>)
		: base(nullptr, std::forward<OY>(vy), std::forward<OZ>(vz)) {}
		
		public: template<typename OX, typename OZ> constexpr
		Vec(OX&& vx, OZ&& vz)
		noexcept(noexcept(base(std::declval<OX>(), nullptr, std::declval<OZ>())))
		requires(std::constructible_from<base, OX, std::nullptr_t, OZ>
		&& std::is_void_v<Y>)
		: base(std::forward<OX>(vx), nullptr, std::forward<OZ>(vz)) {}
		
		
		
		public: template<typename OX> constexpr explicit
		Vec(OX&& vx)
		noexcept(noexcept(base(std::declval<OX>(), nullptr, nullptr)))
		requires(std::constructible_from<base, OX, std::nullptr_t, std::nullptr_t>
		&& std::default_initializable<typename base::baseY>
		&& std::default_initializable<typename base::baseZ>)
		: base(std::forward<OX>(vx), nullptr, nullptr) {}
		
		public: template<typename OY> constexpr explicit
		Vec(OY&& vy)
		noexcept(noexcept(base(nullptr, std::declval<OY>(), nullptr)))
		requires(std::constructible_from<base, std::nullptr_t, OY, std::nullptr_t>
		&& std::is_void_v<X> && std::default_initializable<typename base::baseZ>)
		: base(nullptr, std::forward<OY>(vy), nullptr) {}
		
		public: template<typename OZ> constexpr explicit
		Vec(OZ&& vz)
		noexcept(noexcept(base(nullptr, nullptr, std::declval<OZ>())))
		requires(std::constructible_from<base, std::nullptr_t, std::nullptr_t, OZ>
		&& std::is_void_v<X> && std::is_void_v<Y>)
		: base(nullptr, nullptr, std::forward<OZ>(vz)) {}
		
	};
	
}



int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	constexpr auto b = ink::Vec<void, char, size_t>(3,4ULL);
	sizeof(b);
	
	return 0;
}
