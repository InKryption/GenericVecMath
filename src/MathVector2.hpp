#ifndef INK_MATH_VECTOR_HEADER_FILE_GUARD
#define INK_MATH_VECTOR_HEADER_FILE_GUARD

#include <cstdlib>
#include <utility>
#include <type_traits>
#include <concepts>



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
	
	/**
	 * Any type which is any of:
	 * (+) To const&
	 * (+) To &&
	 * (+) To &
	 * (+) To
	 * (+) <Convertible to 'To const&' - this includes all of the former>
	 */
	template<typename From, typename To>
	concept temp_reference_to = std::convertible_to<From, To const&>;
	
	template<typename From, typename To, typename OrDefault = std::nullptr_t>
	concept tmp_ref_or_default = temp_reference_to<From, To> || std::same_as<From, OrDefault>;
	
	namespace detail {
		
		struct Noop {
			constexpr			Noop(std::nullptr_t = nullptr) noexcept {}
			explicit constexpr	Noop([[maybe_unused]] auto&&) noexcept {}
			template<typename T> explicit constexpr operator T() noexcept(T()) { return T(); }
			
			#define BinaryOp(op)																			\
			template<typename T> friend constexpr T operator op(Noop, T&& rhs) noexcept { return rhs; }	\
			template<typename T> friend constexpr T operator op(T&& lhs, Noop) noexcept { return lhs; }	\
			friend constexpr decltype(auto) operator op(Noop, Noop) noexcept {	return Noop();	}
				BinaryOp(<=>)
				BinaryOp(==)	BinaryOp(!=)
				BinaryOp(>=)	BinaryOp(>)
				BinaryOp(<=)	BinaryOp(<)
				BinaryOp(+)		BinaryOp(-)
				BinaryOp(*)		BinaryOp(/)		BinaryOp(%)
				BinaryOp(|)		BinaryOp(&)		BinaryOp(^)
				BinaryOp(>>)	BinaryOp(<<)
				BinaryOp(&&)	BinaryOp(||)
			#undef BinaryOp
			
			#define UnaryOp(op)	friend constexpr decltype(auto) operator op(temp_reference_to<Noop> auto&& noop) noexcept { return noop; }
				UnaryOp(+)		UnaryOp(-)
				UnaryOp(!)		UnaryOp(~)
				UnaryOp(*)		UnaryOp(&)
			#undef UnaryOp
			
			#define ShortHandOp(op, arg) constexpr decltype(auto) operator op(arg) const noexcept { return *this; }
				ShortHandOp(++, int)
				ShortHandOp(--,int)
				ShortHandOp(++,)
				ShortHandOp(--,)
			#undef ShortHandOp
			
			#define AssignOp(op) constexpr decltype(auto) operator op(auto&& rhs) const noexcept { return *this; }
				AssignOp(=)
				AssignOp(+=)	AssignOp(-=)
				AssignOp(*=)	AssignOp(/=)	AssignOp(%=)
				AssignOp(|=)	AssignOp(&=)	AssignOp(^=)
				AssignOp(>>=)	AssignOp(<<=)
			#undef AssignOp
			
			template<typename... Args> constexpr decltype(auto)
			operator()([[maybe_unused]] Args&&...) const noexcept { return *this; }
			
			template<typename T> constexpr decltype(auto)
			operator[]([[maybe_unused]] T&&) const noexcept { return *this; }
			
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
			
			public: template<Tag tag>
			using value_type = std::conditional_t<(std::is_same_v<real_type<tag>, void>), std::nullptr_t, real_type<tag>>;
			
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
			
		};
		
		template<typename X, typename Y, typename Z, typename MetaInfo = AxisGroupMetaInfo<X, Y, Z>>
		struct VecBase:
			public MetaInfo::AxisAt<0>,
			public MetaInfo::AxisAt<1>,
			public MetaInfo::AxisAt<2>
		{
			
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
			
			public: constexpr
			VecBase(auto&& x, auto&& y, auto&& z)
			noexcept(noexcept( baseX(x), baseY(y), baseZ(z) ))
			:	base0( [&]() constexpr noexcept -> decltype(auto) {
					if constexpr(std::same_as<base0, baseX>) return x;
					if constexpr(std::same_as<base0, baseY>) return y;
					if constexpr(std::same_as<base0, baseZ>) return z;
				}()),
				
				base1( [&]() constexpr noexcept -> decltype(auto) {
					if constexpr(std::same_as<base1, baseX>) return x;
					if constexpr(std::same_as<base1, baseY>) return y;
					if constexpr(std::same_as<base1, baseZ>) return z;
				}()),
				
				base2( [&]() constexpr noexcept -> decltype(auto) {
					if constexpr(std::same_as<base2, baseX>) return x;
					if constexpr(std::same_as<base2, baseY>) return y;
					if constexpr(std::same_as<base2, baseZ>) return z;
				}()) {}
			
		};
		
	}
	
	template<typename X, typename Y, typename Z>
	class Vec: public detail::VecBase<X, Y, Z> {
		
		template<typename, typename, typename> friend class Vec;
		
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
		
		/* Default constructor */
		
		public: constexpr
		Vec()
		noexcept(noexcept(base()))
		requires(std::default_initializable<base>)
		: base() {}
		
		public: constexpr
		Vec(tmp_ref_or_default<ctrX> auto&& x, tmp_ref_or_default<ctrY> auto&& y, tmp_ref_or_default<ctrZ> auto&& z)
		noexcept(noexcept( base(x, y, z) ))
		: base(x, y, z) {}
		
		public: template<typename OX, typename OY, typename OZ> constexpr
		Vec(Vec<OX, OY, OZ> const& other)
		noexcept(noexcept(base(other.x, other.y, other.z)))
		: base(other.x, other.y, other.z) {}
		
		public: template<typename OX, typename OY, typename OZ> constexpr
		Vec(Vec<OX, OY, OZ> && other)
		noexcept(noexcept(base(other.x, other.y, other.z)))
		: base(other.x, other.y, other.z) {}
		
		
		
		public: constexpr
		Vec(tmp_ref_or_default<ctrX> auto&& x, tmp_ref_or_default<ctrY> auto&& y)
		noexcept(noexcept( base(x, y, nullptr) ))
		requires(std::same_as<Z, void> || std::default_initializable<baseZ>)
		: base(x, y, nullptr) {}
		
		public: constexpr
		Vec(tmp_ref_or_default<ctrX> auto&& x, tmp_ref_or_default<ctrZ> auto&& z)
		noexcept(noexcept( base(x, nullptr, z) ))
		requires(std::same_as<Y, void>)
		: base(x, nullptr, z) {}
		
		public: constexpr
		Vec(tmp_ref_or_default<ctrY> auto&& y, tmp_ref_or_default<ctrZ> auto&& z)
		noexcept(noexcept( base(nullptr, y, z) ))
		requires(std::same_as<X, void>)
		: base(nullptr, y, z) {}
		
		
		
		public: constexpr
		Vec(tmp_ref_or_default<ctrX> auto&& x)
		noexcept(noexcept( base(x, nullptr, nullptr) ))
		requires((std::same_as<void, Y> || std::default_initializable<baseY>) && (std::same_as<void, Z> || std::default_initializable<baseZ>))
		: base(x, nullptr, nullptr) {}
		
		public: constexpr
		Vec(tmp_ref_or_default<ctrY> auto&& y)
		noexcept(noexcept( base(nullptr, y, nullptr) ))
		requires(std::same_as<void, X> && (std::same_as<void, Z> && std::default_initializable<baseZ>))
		: base(nullptr, y, nullptr) {}
		
		public: constexpr explicit
		Vec(auto&& z)
		noexcept(noexcept( base(nullptr, nullptr, z) ))
		requires(std::same_as<void, X> && std::same_as<void, Y>)
		: base(nullptr, nullptr, z) {}
		
		
		private: template<typename RX, typename RY, typename RZ>
		static constexpr bool compatible_arith_axies
		=	((std::same_as<X, void> && std::same_as<RX, void>) || (!std::same_as<X, void> && !std::same_as<RX, void>))
		&&	((std::same_as<Y, void> && std::same_as<RY, void>) || (!std::same_as<Y, void> && !std::same_as<RY, void>))
		&&	((std::same_as<Z, void> && std::same_as<RZ, void>) || (!std::same_as<Z, void> && !std::same_as<RZ, void>));
		
		
		public: template<typename RX, typename RY, typename RZ>
		requires(compatible_arith_axies<RX, RY, RZ>)
		friend constexpr decltype(auto)
		operator+(Vec const& lhs, Vec<RX, RY, RZ> const& rhs)
		noexcept(noexcept( (lhs.x + rhs.x), (lhs.y + rhs.y), (lhs.z + rhs.z) ))
		requires requires(decltype(lhs) lhs, decltype(rhs) rhs)
		{ {lhs.x + rhs.x}; {lhs.y + rhs.y}; {lhs.z + rhs.z}; }
		{
			using detail::Noop;
			
			decltype(auto) x_out = lhs.x + rhs.x;
			decltype(auto) y_out = lhs.y + rhs.y;
			decltype(auto) z_out = lhs.z + rhs.z;
			
			using rOX = decltype(x_out);
			using rOY = decltype(y_out);
			using rOZ = decltype(z_out);
			
			constexpr auto
			xNoop = (std::same_as<Noop, rOX>),
			yNoop = (std::same_as<Noop, rOY>),
			zNoop = (std::same_as<Noop, rOZ>);
			
			using OX = std::conditional_t<(xNoop), void, rOX>;
			using OY = std::conditional_t<(yNoop), void, rOY>;
			using OZ = std::conditional_t<(zNoop), void, rOZ>;
			
			using OXArg = std::conditional_t<(xNoop), std::nullptr_t, rOX>;
			using OYArg = std::conditional_t<(yNoop), std::nullptr_t, rOY>;
			using OZArg = std::conditional_t<(zNoop), std::nullptr_t, rOZ>;
			
			return Vec<OX, OY, OZ> (
				static_cast<OXArg>(x_out),
				static_cast<OYArg>(y_out),
				static_cast<OZArg>(z_out)
			);
		}
		
	};
	
	template<typename X, typename Y, typename Z> Vec(X, Y, Z) -> Vec<X, Y, Z>;
	Vec(std::nullptr_t, std::nullptr_t, std::nullptr_t) -> Vec<void, void, void>;
	
	template<typename X, typename Y> Vec(X, Y, std::nullptr_t) -> Vec<X, Y, void>;
	template<typename X, typename Z> Vec(X, std::nullptr_t, Z) -> Vec<X, void, Z>;
	template<typename Y, typename Z> Vec(std::nullptr_t, Y, Z) -> Vec<void, Y, Z>;
	
	template<typename X> Vec(X, std::nullptr_t, std::nullptr_t) -> Vec<X, void, void>;
	template<typename Y> Vec(std::nullptr_t, Y, std::nullptr_t) -> Vec<void, Y, void>;
	template<typename Z> Vec(std::nullptr_t, std::nullptr_t, Z) -> Vec<void, void, Z>;
	
	template<typename X> Vec(X, std::nullptr_t) -> Vec<X, void, void>;
	template<typename Y> Vec(std::nullptr_t, Y) -> Vec<void, Y, void>;
	template<typename X> Vec(X) -> Vec<X, void, void>;
	
}

#endif
