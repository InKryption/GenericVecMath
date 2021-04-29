#ifndef INK_MATH_VECTOR_HEADER_FILE_GUARD
#define INK_MATH_VECTOR_HEADER_FILE_GUARD

#include <cstdlib>
#include <utility>
#include <tuple>
#include <concepts>	 

namespace ink {
	
	namespace detail {
		
		// This is a utility construct; it can be operated on with anything, but always produce a noop,
		// either returning the other operated variable reference, or itself in the case of unary operations.
		// This applies as well to operations involving comparisons (returns the appropriate reference to the
		// left-hand-side or right-hand-side argument). 
		// It is an empty struct, therefore empty-base-like optimization can be applied using [[no_unique_address]] on supporting
		// compilers.
		// 
		// Note:
		// It is -implicitly- default constructible.
		// It is -implicitly- constructible from any type T.
		// It is -explicitly- convertible to any type T.
		// This means that Noop can essentially be a dummy-target for initializations, but will warn
		// if the user converts Noop to another type without statically casting it to the desired type.
		struct Noop {
			constexpr Noop() noexcept {}
			template<typename T> constexpr Noop([[maybe_unused]] T&&) noexcept {}
			template<typename T> explicit constexpr operator T() const noexcept(noexcept(T())) { return T(); }
			
			#define BinaryOp(op)	\
			friend constexpr auto&& operator op(Noop, auto&& rhs) noexcept { return rhs; }	\
			friend constexpr auto&& operator op(auto&& lhs, Noop) noexcept { return lhs; }	\
			friend constexpr decltype(auto) operator op(Noop, Noop) noexcept { return Noop(); }
			BinaryOp(+)	BinaryOp(-)
			BinaryOp(*)	BinaryOp(/)	BinaryOp(%)
			BinaryOp(&)	BinaryOp(|)	BinaryOp(^)
			BinaryOp(<<)
			BinaryOp(>>)
			#undef BinaryOp
			
			#define UnaryOp(op)	\
			friend constexpr decltype(auto) operator op(Noop noop) noexcept { return noop; }
			UnaryOp(+)	UnaryOp(-)
			UnaryOp(*)	UnaryOp(&)
			UnaryOp(!)	UnaryOp(~)
			#undef UnaryOp
			
			#define BoolOp(op)	\
			template<typename T> requires(!std::convertible_to<T, Noop const&>) friend constexpr decltype(auto) operator op(Noop, T&& rhs) noexcept { return rhs; }	\
			template<typename T> requires(!std::convertible_to<T, Noop const&>) friend constexpr decltype(auto) operator op(T&& lhs, Noop) noexcept { return lhs; }
			BoolOp(&&)	BoolOp(||)
			BoolOp(==)	BoolOp(!=)
			BoolOp(>=)	BoolOp(<=)
			BoolOp(>)	BoolOp(<)
			BoolOp(<=>)
			#undef BoolOp
			
			#define NoopCmp(op)	\
			friend constexpr decltype(auto) operator op(Noop, Noop) noexcept { return Noop(); }
			NoopCmp(==)	NoopCmp(!=)
			NoopCmp(>=)	NoopCmp(<=)
			NoopCmp(>)	NoopCmp(<)
			#undef NoopCmp
			
			#define NoopAssignOp(op)	\
			template<typename T> constexpr decltype(auto) operator op(T&& rhs) const noexcept { return rhs; }
			NoopAssignOp(=)
			NoopAssignOp(+=)	NoopAssignOp(-=)
			NoopAssignOp(*=)	NoopAssignOp(/=)	NoopAssignOp(%=)
			NoopAssignOp(&=)	NoopAssignOp(|=)	NoopAssignOp(^=)
			NoopAssignOp(>>=)	NoopAssignOp(<<=)
			#undef NoopAssignOp
			
			#define ShortHandOp(op, arg)	\
			constexpr decltype(auto) operator op(arg) const noexcept { return *this; }
			ShortHandOp(++,int)
			ShortHandOp(--,int)
			ShortHandOp(++,)
			ShortHandOp(--,)
			#undef ShortHandOp
			
			template<typename... Args> constexpr decltype(auto)
			operator()([[maybe_unused]] Args&&...) const noexcept
			{ return *this; }
			
			template<typename T> constexpr decltype(auto)
			operator[]([[maybe_unused]] T&&) const noexcept
			{ return *this; }
			
		};
		
		template<typename T, typename U>
		struct CommonTemplate: std::false_type {};
		
		template<template<typename...> typename Template, typename... Ts, typename... Us>
		struct CommonTemplate<Template<Ts...>, Template<Us...>>: std::true_type {};
		
		template<typename T, typename U>
		static constexpr bool CommonTemplate_v = CommonTemplate<T, U>::value;
		
		template<typename T>
		concept non_void = (!std::same_as<void, T>);
		
		enum class XYZ { X , Y , Z };
		template<typename T, XYZ tag> struct Axis;
		
		#define SpecializeAxis(XYZ_tag, member_name)			\
																\
		template<non_void T> struct Axis<T, XYZ_tag> {			\
			public: using XYZ = ink::detail::XYZ;				\
			public: using ctr_arg = T;							\
			public: using value_type = T;						\
			protected: static constexpr XYZ tag = XYZ_tag;		\
																\
			public: constexpr									\
			Axis()												\
			noexcept(noexcept(T()))								\
			requires( std::is_default_constructible<T>{}() )	\
			: member_name() {}									\
																\
			public: constexpr									\
			Axis(T & member_name)								\
			noexcept(noexcept(T(member_name)))					\
			: member_name(member_name) {}						\
																\
			public: constexpr									\
			Axis(T && member_name)								\
			noexcept(noexcept(T(member_name)))					\
			: member_name(member_name) {}						\
																\
			public: constexpr									\
			Axis(T const& member_name)							\
			noexcept(noexcept(T(member_name)))					\
			: member_name(member_name) {}						\
																\
			public: T member_name;								\
		};														\
																\
		template<> struct Axis<void, XYZ_tag> {					\
			public: using XYZ = ink::detail::XYZ;				\
			public: using ctr_arg = std::nullptr_t;				\
			public: using value_type = void;					\
			protected: static constexpr XYZ tag = XYZ_tag;		\
																\
			public: constexpr									\
			Axis([[maybe_unused]] std::nullptr_t = nullptr)		\
			noexcept {}											\
																\
			protected: static constexpr Noop member_name{};		\
		};
		
		SpecializeAxis(XYZ::X, x)
		SpecializeAxis(XYZ::Y, y)
		SpecializeAxis(XYZ::Z, z)
		#undef SpecializeNonVoidAxis
		
		template<typename X, typename Y, typename Z>
		struct AxisOrdering {
			
			static constexpr bool
			X_Y = alignof(X) >= alignof(Y),
			Y_X = alignof(X) <  alignof(Y),
			
			Y_Z = alignof(Y) >= alignof(Z),
			Z_Y = alignof(Y) <  alignof(Z),
			
			X_Z = alignof(X) >= alignof(Z),
			Z_X = alignof(X) <  alignof(Z);
			
			static constexpr bool
			X_Y_Z = (X_Y && Y_Z && X_Z),
			X_Z_Y = (X_Y && Z_Y && X_Z),
			
			Y_X_Z = (Y_X && Y_Z && X_Z),
			Y_Z_X = (Y_X && Y_Z && Z_X),
			
			Z_X_Y = (X_Y && Z_Y && Z_X),
			Z_Y_X = (Y_X && Z_Y && Z_X);
			
			static constexpr bool
			X0 = ( X_Y_Z || X_Z_Y ), X1 = ( Y_X_Z || Z_X_Y ), X2 = ( Y_Z_X || Z_Y_X ),
			Y0 = ( Y_X_Z || Y_Z_X ), Y1 = ( X_Y_Z || Z_Y_X ), Y2 = ( X_Z_Y || Z_X_Y ),
			Z0 = ( Z_X_Y || Z_Y_X ), Z1 = ( X_Z_Y || Y_Z_X ), Z2 = ( X_Y_Z || Y_X_Z );
			
			using _0 = std::conditional_t<X0, X, std::conditional_t<Y0, Y, Z>>;
			using _1 = std::conditional_t<X1, X, std::conditional_t<Y1, Y, Z>>;
			using _2 = std::conditional_t<X2, X, std::conditional_t<Y2, Y, Z>>;
			
		};
		
		template<typename X, typename Y, typename Z, typename Order = AxisOrdering<Axis<X, XYZ::X>, Axis<Y, XYZ::Y>, Axis<Z, XYZ::Z>>>
		struct VecBase: public Order::_0, public Order::_1, public Order::_2
		{
			
			private:
			using baseX = Axis<X, XYZ::X>;
			using baseY = Axis<Y, XYZ::Y>;
			using baseZ = Axis<Z, XYZ::Z>;
			
			protected:
			using ctr_x = baseX::ctr_arg;
			using ctr_y = baseY::ctr_arg;
			using ctr_z = baseZ::ctr_arg;
			
			private:
			using base0 = typename Order::_0;
			using base1 = typename Order::_1;
			using base2 = typename Order::_2;
			
			public: constexpr
			VecBase() noexcept( noexcept(base0()) && noexcept(base1()) && noexcept(base2()) )
			requires( std::is_default_constructible_v<base0> && std::is_default_constructible_v<base1> && std::is_default_constructible_v<base1> )
			: base0(), base1(), base2() {}
			
			#define TEMP_FORWARD_LAMBDA_MACRO(base)	\
			[&]()constexpr noexcept -> decltype(auto) {	\
				if constexpr(base::tag == XYZ::X) {return x;}	\
				if constexpr(base::tag == XYZ::Y) {return y;}	\
				if constexpr(base::tag == XYZ::Z) {return z;}	\
			}()
			
			public: constexpr
			VecBase(auto&& x, auto&& y, auto&& z) noexcept( noexcept(baseX(x)) && noexcept(baseY(y)) && noexcept(baseZ(z)) )
			requires( std::is_constructible_v<baseX, decltype(x)> && std::is_constructible_v<baseY, decltype(y)> && std::is_constructible_v<baseZ, decltype(z)> )
			:	base0( TEMP_FORWARD_LAMBDA_MACRO(base0) ),
				base1( TEMP_FORWARD_LAMBDA_MACRO(base1) ),
				base2( TEMP_FORWARD_LAMBDA_MACRO(base2) ) {}
			
			#undef TEMP_FORWARD_LAMBDA_MACRO
			
			/*
			#define DECL_CTR(xspec, yspec, zspec, init0, init1, init2, ordering)	\
			constexpr	\
			VecBase(typename baseX::ctr_arg xspec x, typename baseY::ctr_arg yspec y, typename baseZ::ctr_arg zspec z)	\
			noexcept( noexcept(baseX(x)) && noexcept(baseY(y)) && noexcept(baseZ(z)) )	\
			requires(	std::is_constructible<baseX, decltype(x)>{}()	\
			&&			std::is_constructible<baseY, decltype(y)>{}()	\
			&&			std::is_constructible<baseZ, decltype(z)>{}()	\
			&&			Order::ordering)								\
			: base0(init0), base1(init1), base2(init2) {}				\
			
			#define CTR_XSPEC_VARIANTS(m, ...)				\
				m(&, __VA_ARGS__)							\
				m(&&, __VA_ARGS__)							\
				m(const&, __VA_ARGS__)
			#define CTR_XYSPEC_VARIANTS(m, ...)				\
				CTR_XSPEC_VARIANTS(m, &, __VA_ARGS__)		\
				CTR_XSPEC_VARIANTS(m, &&, __VA_ARGS__)		\
				CTR_XSPEC_VARIANTS(m, const&, __VA_ARGS__)
			#define CTR_SPEC_VARIANTS(m, ...)				\
				CTR_XYSPEC_VARIANTS(m, &, __VA_ARGS__)		\
				CTR_XYSPEC_VARIANTS(m, &&, __VA_ARGS__)		\
				CTR_XYSPEC_VARIANTS(m, const&, __VA_ARGS__)
			
			CTR_SPEC_VARIANTS(DECL_CTR, x, y, z, X_Y_Z)
			CTR_SPEC_VARIANTS(DECL_CTR, x, z, y, X_Z_Y)
			
			CTR_SPEC_VARIANTS(DECL_CTR, y, x, z, Y_X_Z)
			CTR_SPEC_VARIANTS(DECL_CTR, y, z, x, Y_Z_X)
			
			CTR_SPEC_VARIANTS(DECL_CTR, z, x, y, Z_X_Y)
			CTR_SPEC_VARIANTS(DECL_CTR, z, y, x, Z_Y_X)
			
			#undef CTR_SPEC_VARIANTS
			#undef CTR_XYSPEC_VARIANTS
			#undef CTR_XSPEC_VARIANTS
			
			#undef DECL_CTR
			*/
			
		};
		
	}
	
	template<typename X, typename Y, typename Z> class Vec;
	
	template<typename X, typename Y, typename Z>
	static constexpr decltype(auto)
	make_vec(X&& x, Y&& y, Z&& z) noexcept
	{
		return Vec(
			[&]() constexpr noexcept -> X { if constexpr(std::same_as<std::decay_t<X>, detail::Noop>) { return nullptr; } else { return std::forward<X>(x); } }(),
			[&]() constexpr noexcept -> Y { if constexpr(std::same_as<std::decay_t<Y>, detail::Noop>) { return nullptr; } else { return std::forward<Y>(y); } }(),
			[&]() constexpr noexcept -> Z { if constexpr(std::same_as<std::decay_t<Z>, detail::Noop>) { return nullptr; } else { return std::forward<Z>(z); } }()
		);
	}
	
	template<typename X, typename Y = X, typename Z = void>
	class Vec: public detail::VecBase<X, Y, Z> {
		
		template<typename, typename, typename> friend class Vec;
		
		private: using
		base = detail::VecBase<X, Y, Z>;
		
		private:
		using ctr_x = base::ctr_x;
		using ctr_y = base::ctr_y;
		using ctr_z = base::ctr_z;
		
		using type_x = decltype(base::x);
		using type_y = decltype(base::y);
		using type_z = decltype(base::z);
		
		private: static constexpr bool
		void_x = std::same_as<void, X>,
		void_y = std::same_as<void, Y>,
		void_z = std::same_as<void, Z>;
		
		/* CONSTRUCTORS */
		
		public: template<typename RX, typename RY, typename RZ, typename OVec = Vec<RX, RY, RZ>>
		explicit constexpr
		operator Vec<RX, RY, RZ>() const
		noexcept(	noexcept(static_cast<typename OVec::type_x>(this->x))
		&&			noexcept(static_cast<typename OVec::type_y>(this->y))
		&&			noexcept(static_cast<typename OVec::type_z>(this->z)))
		requires requires(type_x fromx, type_y fromy, type_z fromz)
		{
			{static_cast<typename OVec::type_x>(fromx)};
			{static_cast<typename OVec::type_y>(fromy)};
			{static_cast<typename OVec::type_z>(fromz)};
		}
		{
			return OVec( static_cast<typename OVec::type_x>(this->x), static_cast<typename OVec::type_y>(this->y), static_cast<typename OVec::type_z>(this->z) );
		}
		
		public: constexpr
		Vec() noexcept(noexcept(base()))
		requires( std::is_default_constructible<base>{}() )
		: base() {}
		
		public: constexpr
		Vec(auto&& x, auto&& y, auto&& z) noexcept(noexcept(base(x, y, z)))
		requires(std::is_constructible_v<base, decltype(x), decltype(y), decltype(z)>)
		: base(x, y, z) {}
		
		
		
		public: constexpr
		Vec(auto&& x, auto&& y) noexcept(noexcept(base(x, y, ctr_z())))
		requires( !void_x && !void_y && (void_z || std::is_default_constructible_v<Z>) && std::is_constructible_v<base, decltype(x), decltype(y), ctr_z> )
		: base(x, y, ctr_z()) {}
		
		public: constexpr
		Vec(auto&& x, auto&& z) noexcept(noexcept(base(x, ctr_y(), z)))
		requires( !void_x && void_y && !void_z && std::is_default_constructible_v<base, decltype(x), ctr_y, decltype(z)> )
		: base(x, ctr_y(), z) {}
		
		public: constexpr
		Vec(auto&& y, auto&& z) noexcept(noexcept(base(ctr_x(), y, z)))
		requires( void_x && !void_y && !void_z && std::is_default_constructible_v<base, ctr_x, decltype(y), decltype(z)> )
		: base(ctr_x(), y, z) {}
		
		
		
		public: explicit constexpr /* Set to explicit to avoid implicit conversions from fundamental types to vectors with only an x axis. */
		Vec(auto&& x) noexcept(noexcept(base(x, ctr_y(), ctr_z())))
		requires( !void_x && (void_y || std::is_default_constructible_v<Y>) && (void_z || std::is_default_constructible_v<Z>) && std::is_constructible_v<base, decltype(x), ctr_y, ctr_z> )
		: base(x, ctr_y(), ctr_z()) {}
		
		public: constexpr
		Vec(auto&& y) noexcept(noexcept(base(ctr_x(), y, ctr_z())))
		requires( void_x && !void_y && void_z && std::is_constructible_v<base, ctr_z, decltype(y), ctr_z> )
		: base(ctr_x(), y, ctr_z()) {}
		
		public: constexpr
		Vec(auto&& z) noexcept(noexcept(base(ctr_x(), ctr_y(), z)))
		requires( void_x && void_y && !void_z && std::is_default_constructible_v<base, ctr_x, ctr_y, decltype(z)> )
		: base(ctr_x(), ctr_y(), z) {}
		
		
		
		/* Get method, for get<n>(Vec) syntax. */
		
		template<size_t i> requires(i % 3 == 0) friend constexpr auto&&
		get(Vec& v) noexcept {
			if constexpr(i == 0) return v.x;
			if constexpr(i == 1) return v.y;
			if constexpr(i == 2) return v.z;
		}
		
		template<size_t i> requires(i % 3 == 0) friend constexpr auto&&
		get(Vec&& v) noexcept {
			if constexpr(i == 0) return v.x;
			if constexpr(i == 1) return v.y;
			if constexpr(i == 2) return v.z;
		}
		
		template<size_t i> requires(i % 3 == 0) friend constexpr auto&&
		get(Vec const& v) noexcept {
			if constexpr(i == 0) return v.x;
			if constexpr(i == 1) return v.y;
			if constexpr(i == 2) return v.z;
		}
		
		
		
		private: template<typename Lhs, typename Rhs>
		static constexpr bool ConstrainToVecTypes
		=	std::is_same_v<Vec, std::remove_cvref_t<Lhs>>
		&&	detail::CommonTemplate_v<Vec, std::remove_cvref_t<Rhs>>;
		
		public: template<typename T, typename U>
		requires(ConstrainToVecTypes<T, U>)
		friend constexpr decltype(auto)
		operator+(T&& lhs, U&& rhs) noexcept( noexcept(lhs.x + rhs.x) && noexcept(lhs.y + rhs.y) && noexcept(lhs.z + rhs.z) )
		requires requires(T&& lhs, U&& rhs) { {lhs.x + rhs.x}; {lhs.y + rhs.y}; {lhs.z + rhs.z}; }
		{
			return make_vec(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
		}
		
		public: template<typename T, typename U>
		requires(ConstrainToVecTypes<T, U>)
		friend constexpr decltype(auto)
		operator-(T&& lhs, U&& rhs) noexcept( noexcept(lhs.x - rhs.x) && noexcept(lhs.y - rhs.y) && noexcept(lhs.z - rhs.z) )
		requires requires(T&& lhs, U&& rhs) { {lhs.x - rhs.x}; {lhs.y - rhs.y}; {lhs.z - rhs.z}; }
		{
			return make_vec(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
		}
		
		public: template<typename T, typename U>
		requires(ConstrainToVecTypes<T, U>)
		friend constexpr decltype(auto)
		operator*(T&& lhs, U&& rhs) noexcept( noexcept(lhs.x * rhs.x) && noexcept(lhs.y * rhs.y) && noexcept(lhs.z * rhs.z) )
		requires requires(T&& lhs, U&& rhs) { {lhs.x * rhs.x}; {lhs.y * rhs.y}; {lhs.z * rhs.z}; }
		{
			return make_vec(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
		}
		
		public: template<typename T, typename U>
		requires(ConstrainToVecTypes<T, U>)
		friend constexpr decltype(auto)
		operator/(T&& lhs, U&& rhs) noexcept( noexcept(lhs.x / rhs.x) && noexcept(lhs.y / rhs.y) && noexcept(lhs.z / rhs.z) )
		requires requires(T&& lhs, U&& rhs) { {lhs.x / rhs.x}; {lhs.y / rhs.y}; {lhs.z / rhs.z}; }
		{
			return make_vec(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
		}
		
		public: template<typename T, typename U>
		requires(ConstrainToVecTypes<T, U>)
		friend constexpr decltype(auto)
		operator%(T&& lhs, U&& rhs) noexcept( noexcept(lhs.x % rhs.x) && noexcept(lhs.y % rhs.y) && noexcept(lhs.z % rhs.z) )
		requires requires(T&& lhs, U&& rhs) { {lhs.x % rhs.x}; {lhs.y % rhs.y}; {lhs.z % rhs.z}; }
		{
			return make_vec(lhs.x % rhs.x, lhs.y % rhs.y, lhs.z % rhs.z);
		}
		
	};
	
	template<class X, class Y, class Z> Vec(X, Y, Z)			-> Vec<X, Y, Z>;
	
	template<class X> Vec(X, std::nullptr_t, std::nullptr_t)	-> Vec<X, void, void>;
	template<class Y> Vec(std::nullptr_t, Y, std::nullptr_t)	-> Vec<void, Y, void>;
	template<class Z> Vec(std::nullptr_t, std::nullptr_t, Z)	-> Vec<void, void, Z>;
	
	template<class X, class Y>	Vec(X, Y, std::nullptr_t)		-> Vec<X, Y, void>;
	template<class X, class Z>	Vec(X, std::nullptr_t, Z)		-> Vec<X, void, Z>;
	template<class Y, class Z>	Vec(std::nullptr_t, Y, Z)		-> Vec<void, Y, Z>;
	
	Vec(std::nullptr_t, std::nullptr_t, std::nullptr_t)			-> Vec<void, void, void>;
	Vec(std::nullptr_t, std::nullptr_t)							-> Vec<void, void, void>;
	Vec(std::nullptr_t)											-> Vec<void, void, void>;
	
	template<class X>			Vec(X)							-> Vec<X, void, void>;
	template<class X, class Y>	Vec(X, Y)						-> Vec<X, Y>;
	
	template<class X> Vec(X, std::nullptr_t)					-> Vec<X, void, void>;
	template<class Y> Vec(std::nullptr_t, Y)					-> Vec<void, Y, void>;
	
}

template<typename... T>
struct std::tuple_size<ink::Vec<T...>>
: std::integral_constant<std::size_t, sizeof...(T)> {};

template<typename X, typename Y, typename Z>
struct std::tuple_element<0, ink::Vec<X, Y, Z>>
{ using type = std::conditional_t< !(std::same_as<void,X>), X, ink::detail::Axis<X, ink::detail::XYZ::X>>; };

template<typename X, typename Y, typename Z>
struct std::tuple_element<1, ink::Vec<X, Y, Z>>
{ using type = std::conditional_t< !(std::same_as<void,Y>), Y, ink::detail::Axis<Y, ink::detail::XYZ::Y>>; };

template<typename X, typename Y, typename Z>
struct std::tuple_element<2, ink::Vec<X, Y, Z>>
{ using type = std::conditional_t< !(std::same_as<void,Z>), Z, ink::detail::Axis<Z, ink::detail::XYZ::Z>>; };

std::tuple_element_t<0, ink::Vec<int, char, bool>> f;

#endif
