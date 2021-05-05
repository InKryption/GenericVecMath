#ifndef INK_MATH_VECTOR_HEADER_FILE_GUARD
#define INK_MATH_VECTOR_HEADER_FILE_GUARD

#include <cstdlib>
#include <utility>
#include <type_traits>
#include <cmath>
#include <concepts>
#include <tuple>



namespace ink {
	
	namespace detail {
		template<typename T, typename U>
		struct SameTemplate_t: std::false_type {};
		
		template<template<typename...> typename Template, typename... TArgs, typename... UArgs>
		struct SameTemplate_t<Template<TArgs...>, Template<UArgs...>>: std::true_type {};
		
		template<template<auto...> typename Template, auto... TArgs, auto... UArgs>
		struct SameTemplate_t<Template<TArgs...>, Template<UArgs...>>: std::true_type {};
		
		template<template<typename, auto...> typename Template, typename T1, typename U1, auto... TArgs, auto... UArgs>
		struct SameTemplate_t<Template<T1, TArgs...>, Template<U1, UArgs...>>: std::true_type {};
		
		template<typename T, typename U>
		concept SameTemplate = SameTemplate_t<T, U>::value;
		
		
		
		template<typename T, auto Else = sizeof(char)> struct SizeofWVoid_t:
		std::integral_constant<decltype(sizeof(T)), sizeof(T)> {};
		
		template<auto Else> struct SizeofWVoid_t<void, Else>:
		std::integral_constant<decltype(Else), Else> {};
	}
	
	using detail::SameTemplate;
	
	template<typename T, auto Else = sizeof(char)>
	static constexpr auto SizeofWVoid = detail::SizeofWVoid_t<T, Else>::value;
	
	template<typename X, typename Y, typename Z> class Vec;
	
	namespace detail {
		
		struct Noop
		{
			constexpr operator std::nullptr_t() const noexcept { return nullptr; }
			constexpr Noop(std::nullptr_t = nullptr) noexcept {}
		};
		
		enum class XYZ { X , Y , Z };
		template<typename T, XYZ tag> struct Axis;
		
		
		
		template<> struct Axis<void, XYZ::X> {
			constexpr Axis(std::nullptr_t = nullptr) noexcept {}
			protected: static constexpr Noop x{};
		};
		
		template<> struct Axis<void, XYZ::Y> {
			constexpr Axis(std::nullptr_t = nullptr) noexcept {}
			protected: static constexpr Noop y{};
		};
		
		template<> struct Axis<void, XYZ::Z> {
			constexpr Axis(std::nullptr_t = nullptr) noexcept {}
			protected: static constexpr Noop z{};
		};
		
		
		
		template<typename T> requires(!std::same_as<void, T>)
		struct Axis<T, XYZ::X> {
			constexpr Axis(std::nullptr_t = nullptr) requires(std::default_initializable<T>): x() {}
			constexpr Axis(auto&& x): x(x) {}
			T x;
		};
		
		template<typename T> requires(!std::same_as<void, T>)
		struct Axis<T, XYZ::Y> {
			constexpr Axis(std::nullptr_t = nullptr) requires(std::default_initializable<T>): y() {}
			constexpr Axis(auto&& y): y(y) {}
			T y;
		};
		
		template<typename T> requires(!std::same_as<void, T>)
		struct Axis<T, XYZ::Z> {
			constexpr Axis(std::nullptr_t = nullptr) requires(std::default_initializable<T>): z() {}
			constexpr Axis(auto&& z): z(z) {}
			T z;
		};
		
		
		
		template<typename X, typename Y, typename Z>
		struct AxisGroupMetaInfo {
			
			public:
			using Tag = XYZ;
			
			public: template<Tag tag>
			using real_type = std::conditional_t<(tag == Tag::X), X, std::conditional_t<(tag == Tag::Y), Y, Z>>;
			
			public: template<Tag tag> static constexpr bool
			is_void = std::same_as<void, real_type<tag>>;
			
			public: template<Tag tag>
			using value_type = std::conditional_t<(is_void<tag>), std::nullptr_t, real_type<tag>>;
			
			public: template<Tag tag>
			using GetAxis = detail::Axis<real_type<tag>, tag>;
			
			public: template<Tag tag>
			static constexpr size_t IndexOfAxis =
			(tag == Tag::X) ? (2 - char((SizeofWVoid<real_type<Tag::X>>) >= (SizeofWVoid<real_type<Tag::Y>>)) - char((SizeofWVoid<real_type<Tag::X>>) >= (SizeofWVoid<real_type<Tag::Z>>))) :
			(tag == Tag::Y) ? (2 - char((SizeofWVoid<real_type<Tag::X>>) <  (SizeofWVoid<real_type<Tag::Y>>)) - char((SizeofWVoid<real_type<Tag::Y>>) >= (SizeofWVoid<real_type<Tag::Z>>))) :
			(tag == Tag::Z) ? (2 - char((SizeofWVoid<real_type<Tag::X>>) <  (SizeofWVoid<real_type<Tag::Z>>)) - char((SizeofWVoid<real_type<Tag::Y>>) <  (SizeofWVoid<real_type<Tag::Z>>))) : -1;
			
			public: template<size_t i>
			static constexpr Tag AxisAtIndex =
			(i == IndexOfAxis<Tag::X>) ? (Tag::X) :
			(i == IndexOfAxis<Tag::Y>) ? (Tag::Y) :
			(i == IndexOfAxis<Tag::Z>) ? (Tag::Z) : static_cast<Tag>(static_cast<size_t>(-1));
			
			public: template<size_t i>
			using AxisAt = GetAxis< AxisAtIndex<i> >;
			
			template<typename RX, typename RY, typename RZ> static constexpr decltype(auto)
			OperateVecs(auto&& Op, Vec<X, Y, Z> const& lhs, Vec<RX, RY, RZ> const& rhs) {
				constexpr auto GetProperValue = [](auto&& Op, auto&& l, auto&& r) constexpr {
					using lT = std::remove_cvref_t<decltype(l)>;
					using rT = std::remove_cvref_t<decltype(r)>;
					if		constexpr(std::same_as<Noop, lT> && std::same_as<Noop, rT>)		return nullptr;
					else if	constexpr(!std::same_as<Noop, lT> && !std::same_as<Noop, rT>)	return Op(l, r);
					else if	constexpr(!std::same_as<Noop, lT>) return l;
					else if	constexpr(!std::same_as<Noop, rT>) return r;
				};
				
				return ink::Vec( GetProperValue(Op, lhs.x, rhs.x), GetProperValue(Op, lhs.y, rhs.y), GetProperValue(Op, lhs.z, rhs.z) );
			}
			
			template<typename T> static constexpr decltype(auto)
			OperateScalarRight(auto&& Op, Vec<X, Y, Z> const& lhs, T const& rhs) {
				constexpr auto GetProperValue = [](auto&& Op, auto&& l, auto&& r) constexpr {
					using lT = std::remove_cvref_t<decltype(l)>;
					using rT = std::remove_cvref_t<decltype(r)>;
					if constexpr(std::same_as<lT, Noop> || std::same_as<rT, Noop>) return nullptr;
					else return Op(l, r);
				};
				
				return ink::Vec( GetProperValue(Op, lhs.x, rhs), GetProperValue(Op, lhs.y, rhs), GetProperValue(Op, lhs.z, rhs) );
			}
			
			template<typename T> static constexpr decltype(auto)
			OperateScalarLeft(auto&& Op, T const& lhs, Vec<X, Y, Z> const& rhs) {
				constexpr auto GetProperValue = [](auto&& Op, auto&& l, auto&& r) constexpr {
					using lT = std::remove_cvref_t<decltype(l)>;
					using rT = std::remove_cvref_t<decltype(r)>;
					if constexpr(std::same_as<lT, Noop> || std::same_as<rT, Noop>) return nullptr;
					else return Op(l, r);
				};
				
				return ink::Vec( GetProperValue(Op, lhs, rhs.x), GetProperValue(Op, lhs, rhs.y), GetProperValue(Op, lhs, rhs.z) );
			}
			
			static constexpr decltype(auto)
			OperateUnary(auto&& Op, Vec<X, Y, Z> const& vec) {
				constexpr auto GetProperValue = [](auto&& Op, auto&& v) constexpr {
					using T = std::remove_cvref_t<decltype(v)>;
					if constexpr(std::same_as<T, Noop>) return nullptr;
					else return Op(v);
				};
				
				return ink::Vec( GetProperValue(Op, vec.x), GetProperValue(Op, vec.y), GetProperValue(Op, vec.z) );
			}
			
		};
		
		template<typename X, typename Y, typename Z, typename MetaInfo = AxisGroupMetaInfo<X, Y, Z>>
		struct VecBase:
			public MetaInfo::AxisAt<0>,
			public MetaInfo::AxisAt<1>,
			public MetaInfo::AxisAt<2>
		{
			
			template<typename, typename, typename> friend class Vec;
			template<typename, typename, typename> friend struct AxisGroupMetaInfo;
			template<typename, typename, typename, typename> friend struct VecBase;
			
			protected:	using meta = MetaInfo;
			
			protected:	using baseX = typename meta::GetAxis<XYZ::X>;
			protected:	using baseY = typename meta::GetAxis<XYZ::Y>;
			protected:	using baseZ = typename meta::GetAxis<XYZ::Z>;
			
			private:	using base0 = typename meta::AxisAt<0>;
			private:	using base1 = typename meta::AxisAt<1>;
			private:	using base2 = typename meta::AxisAt<2>;
			
			public: constexpr
			VecBase()
			noexcept(noexcept( baseX(), baseY(), baseZ() ))
			requires(std::default_initializable<base0> && std::default_initializable<base1> && std::default_initializable<base2>)
			: base0(), base1(), base2() {}
			
			public: template<typename OX, typename OY, typename OZ> constexpr
			VecBase(OX&& x, OY&& y, OZ&& z)
			noexcept(noexcept( baseX(std::forward<OX>(x)), baseY(std::forward<OY>(y)), baseZ(std::forward<OZ>(z)) ))
			:	base0( [&]() constexpr noexcept -> decltype(auto) {
					if constexpr(std::same_as<base0, baseX>) { return std::forward<OX>(x); }
					if constexpr(std::same_as<base0, baseY>) { return std::forward<OY>(y); }
					if constexpr(std::same_as<base0, baseZ>) { return std::forward<OZ>(z); }
				}()),
				
				base1( [&]() constexpr noexcept -> decltype(auto) {
					if constexpr(std::same_as<base1, baseX>) return std::forward<OX>(x);
					if constexpr(std::same_as<base1, baseY>) return std::forward<OY>(y);
					if constexpr(std::same_as<base1, baseZ>) return std::forward<OZ>(z);
				}()),
				
				base2( [&]() constexpr noexcept -> decltype(auto) {
					if constexpr(std::same_as<base2, baseX>) return std::forward<OX>(x);
					if constexpr(std::same_as<base2, baseY>) return std::forward<OY>(y);
					if constexpr(std::same_as<base2, baseZ>) return std::forward<OZ>(z);
				}()) {}
			
		};
		
	}
	
	template<typename X, typename Y, typename Z>
	class Vec: public detail::VecBase<X, Y, Z> {
		
		template<typename, typename, typename> friend class Vec;
		template<typename, typename, typename> struct VecBase;
		
		private:
		using base = detail::VecBase<X, Y, Z>;
		using meta = typename base::meta;
		using XYZ = detail::XYZ;
		
		using baseX = typename base::baseX;
		using baseY = typename base::baseY;
		using baseZ = typename base::baseZ;
		
		using ctrX = typename meta::value_type<XYZ::X>;
		using ctrY = typename meta::value_type<XYZ::Y>;
		using ctrZ = typename meta::value_type<XYZ::Z>;
		
		using typeX = decltype(baseX::x);
		using typeY = decltype(baseY::y);
		using typeZ = decltype(baseZ::z);
		
		/* Default constructor */
		
		public: constexpr
		Vec()
		noexcept(noexcept(base()))
		requires(std::default_initializable<base>)
		: base() {}
		
		public: constexpr
		Vec(ctrX const& x, ctrY const& y, ctrZ const& z)
		noexcept(noexcept( base(x, y, z) ))
		: base(x, y, z) {}
		
		public: template<std::convertible_to<ctrX> OX, std::convertible_to<ctrY> OY, std::convertible_to<ctrZ> OZ>
		constexpr
		Vec(Vec<OX, OY, OZ> const& other)
		noexcept(noexcept( base(static_cast<typeX>(other.x), static_cast<typeY>(other.y), static_cast<typeZ>(other.z)) ))
		: base(static_cast<typeX>(other.x), static_cast<typeY>(other.y), static_cast<typeZ>(other.z)) {}
		
		
		
		public: constexpr
		Vec(ctrX const& x, ctrY const& y)
		noexcept(noexcept( base(x, y, nullptr) ))
		requires(std::same_as<Z, void> || std::default_initializable<baseZ>)
		: base(x, y, nullptr) {}
		
		public: constexpr
		Vec(ctrX const& x, ctrZ const& z)
		noexcept(noexcept( base(x, nullptr, z) ))
		requires(std::same_as<Y, void>)
		: base(x, nullptr, z) {}
		
		public: constexpr
		Vec(ctrY const& y, ctrZ const& z)
		noexcept(noexcept( base(nullptr, y, z) ))
		requires(std::same_as<X, void>)
		: base(nullptr, y, z) {}
		
		
		
		public: constexpr explicit
		Vec(ctrX const& x)
		noexcept(noexcept( base(x, nullptr, nullptr) ))
		requires((std::same_as<void, Y> || std::default_initializable<baseY>) && (std::same_as<void, Z> || std::default_initializable<baseZ>))
		: base(x, nullptr, nullptr) {}
		
		public: constexpr explicit
		Vec(ctrY const& y)
		noexcept(noexcept( base(nullptr, y, nullptr) ))
		requires(std::same_as<void, X> && (std::same_as<void, Z> && std::default_initializable<baseZ>))
		: base(nullptr, y, nullptr) {}
		
		public: constexpr explicit
		Vec(ctrZ& z)
		noexcept(noexcept( base(nullptr, nullptr, z) ))
		requires(std::same_as<void, X> && std::same_as<void, Y>)
		: base(nullptr, nullptr, z) {}
		
		
		
		public: template<typename RVec>
		requires (SameTemplate<RVec, Vec>)
		friend constexpr decltype(auto)
		operator*(Vec const& lhs, RVec const& rhs)
		{
			constexpr auto lambda = [](auto&& l, auto&& r) constexpr { return l * r; };
			return meta::OperateVecs(lambda, lhs, rhs);
		}
		
		public: template<typename T>
		requires (!SameTemplate<T, Vec>)
		friend constexpr decltype(auto)
		operator*(Vec const& lhs, T const& rhs)
		{
			constexpr auto lambda = [](auto&& l, auto&& r) constexpr { return l * r; };
			return meta::OperateScalarRight(lambda, lhs, rhs);
		}
		
		public: template<typename T>
		requires (!SameTemplate<T, Vec>)
		friend constexpr decltype(auto)
		operator*(T const& lhs, Vec const& rhs)
		{
			constexpr auto lambda = [](auto&& l, auto&& r) constexpr { return l * r; };
			return meta::OperateScalarLeft(lambda, lhs, rhs);
		}
		
		
		
		public: template<typename RVec>
		requires (SameTemplate<RVec, Vec>)
		friend constexpr decltype(auto)
		operator/(Vec const& lhs, RVec const& rhs)
		{
			constexpr auto lambda = [](auto&& lhs, auto&& rhs) constexpr { return lhs / rhs; };
			return meta::OperateVecs(lambda, lhs, rhs);
		}
		
		public: template<typename T>
		requires (!SameTemplate<T, Vec>)
		friend constexpr decltype(auto)
		operator/(Vec const& lhs, T const& rhs)
		{
			constexpr auto lambda = [](auto&& lhs, auto&& rhs) constexpr { return lhs / rhs; };
			return meta::OperateScalarRight(lambda, lhs, rhs);
		}
		
		public: template<typename T>
		requires (!SameTemplate<T, Vec>)
		friend constexpr decltype(auto)
		operator/(T const& lhs, Vec const& rhs)
		{
			constexpr auto lambda = [](auto&& lhs, auto&& rhs) constexpr { return lhs / rhs; };
			return meta::OperateScalarLeft(lambda, lhs, rhs);
		}
		
		
		
		public: template<typename RVec>
		requires (SameTemplate<RVec, Vec>)
		friend constexpr decltype(auto)
		operator+(Vec const& lhs, RVec const& rhs)
		{
			constexpr auto lambda = [](auto&& l, auto&& r) constexpr { return l + r; };
			return meta::OperateVecs(lambda, lhs, rhs);
		}
		
		public: friend constexpr decltype(auto)
		operator+(Vec const& v)
		{
			constexpr auto lambda = [](auto&& v) constexpr { return +v; };
			return meta::OperateUnary(lambda, v);
		}
		
		
		
		public: template<typename RVec>
		requires (SameTemplate<RVec, Vec>)
		friend constexpr decltype(auto)
		operator-(Vec const& lhs, RVec const& rhs)
		{
			constexpr auto lambda = [](auto&& l, auto&& r) constexpr { return l - r; };
			return meta::OperateVecs(lambda, lhs, rhs);
		}
		
		public: friend constexpr decltype(auto)
		operator-(Vec const& v)
		{
			constexpr auto lambda = [](auto&& v) constexpr { return -v; };
			return meta::OperateUnary(lambda, v);
		}
		
	};
	
	
	
	template<typename X, typename Y, typename Z> Vec(X, Y, Z) -> Vec<X, Y, Z>;
	Vec(std::nullptr_t, std::nullptr_t, std::nullptr_t) -> Vec<void, void, void>;
	
	template<typename X, typename Y> Vec(X, Y, std::nullptr_t) -> Vec<X, Y, void>;
	template<typename X, typename Z> Vec(X, std::nullptr_t, Z) -> Vec<X, void, Z>;
	template<typename Y, typename Z> Vec(std::nullptr_t, Y, Z) -> Vec<void, Y, Z>;
	
	template<typename X, typename Y> Vec(X, Y) -> Vec<X, Y, void>;
	
	template<typename X> Vec(X, std::nullptr_t, std::nullptr_t) -> Vec<X, void, void>;
	template<typename Y> Vec(std::nullptr_t, Y, std::nullptr_t) -> Vec<void, Y, void>;
	template<typename Z> Vec(std::nullptr_t, std::nullptr_t, Z) -> Vec<void, void, Z>;
	
	template<typename X> Vec(X, std::nullptr_t) -> Vec<X, void, void>;
	template<typename Y> Vec(std::nullptr_t, Y) -> Vec<void, Y, void>;
	template<typename X> Vec(X) -> Vec<X, void, void>;
	
	template<typename X, typename Y, typename Z> Vec(Vec<X, Y, Z>) -> Vec<std::remove_cvref_t<X>, std::remove_cvref_t<Y>, std::remove_cvref_t<Z>>;
	
}

void test() {
	
	
	
}

#endif
