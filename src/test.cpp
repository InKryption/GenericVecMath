#include <stdio.h>
#include <cstdlib>
#include <tuple>
#include <type_traits>

namespace ink {
	
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
			
			private: static constexpr auto
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
			
			template<size_t i> static constexpr decltype(auto)
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
		
	}
	
	template<typename X = void, typename Y = X, typename Z = void> class Vec;
	Vec() -> Vec<void, void, void>;
	template<typename X, typename Y, typename Z> Vec(X, Y, Z) -> Vec<
		std::conditional_t<(std::same_as<X, std::nullptr_t>), void, X>,
		std::conditional_t<(std::same_as<Y, std::nullptr_t>), void, Y>,
		std::conditional_t<(std::same_as<Z, std::nullptr_t>), void, Z>
	>;
	template<typename X, typename Y> Vec(X, Y) -> Vec<
		std::conditional_t<(std::same_as<X, std::nullptr_t>), void, X>,
		std::conditional_t<(std::same_as<Y, std::nullptr_t>), void, Y>,
		void
	>;
	template<typename X> Vec(X) -> Vec<
		std::conditional_t<(std::same_as<X, std::nullptr_t>), void, X>,
		void, void
	>;
	
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
		operator*(Vec const& lhs, Vec<OX, OY, OZ> const& rhs)
		{
			constexpr decltype(auto) conditional = []() {};
			
			decltype(auto) x_out = []() {}();
			decltype(auto) y_out = []() {}();
			decltype(auto) z_out = []() {}();
		}
		
	};
	
}

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	ink::Vec m{3};
	
	return 0;
}
