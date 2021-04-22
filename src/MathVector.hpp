#ifndef INK_MATH_VECTOR_HEADER_FILE_GUARD
#define INK_MATH_VECTOR_HEADER_FILE_GUARD

#include <cstdlib>
#include <concepts>

namespace ink {
	
	namespace internal {
		
		template<typename... T>
		struct CheckAligned {
			static constexpr bool value
			=	true;	};
		
		template<typename T1, typename T2, typename... T>
		struct CheckAligned<T1, T2, T...> {
			static constexpr bool value
			=	(alignof(T1) >= alignof(T2))
			&&	CheckAligned<T2, T...>::value;	};
		
		template<typename T1, typename T2>
		struct CheckAligned<T1, T2> {
			static constexpr bool value
			=	(alignof(T1) >= alignof(T2));	};
		
		
		
		template<typename... T>
		struct AlignTypes;
		
		template<typename... T> requires(CheckAligned<T...>::value)
		struct AlignTypes<T...> {
			using type = AlignTypes;
			template<template<typename...> typename Template>
			using rebind = Template<T...>;
		};
		
		template<typename T1, typename... T>
		struct AlignTypes<T1, AlignTypes<T...>>
		{ using type = typename AlignTypes<T1, T...>::type; };
		
		template<typename T1, typename T2, typename... T>
		requires( !CheckAligned<T1, T2, T...>::value )
		struct AlignTypes<T1, T2, T...>
		{
			using _0 = std::conditional_t<(alignof(T1) >= alignof(T2)), T1, T2>;
			using _1 = std::conditional_t<(alignof(T1) >= alignof(T2)), T2, T1>;
			using type = typename AlignTypes<
				_0 , typename AlignTypes<_1, T...>::type
			>:: type;
		};
		
		template<typename... T>
		using AlignTypes_t = typename AlignTypes<T...>::type;
		
		
		
		template<auto CMP> struct Switch {
			
			using cmp_t = decltype(CMP);
			
			template<cmp_t V, typename T> struct Case
			{
				constexpr Case(cmp_t = V, T = T()) {}
			};
			
			template<typename... CASES>
			struct In_impl;
			
			template<cmp_t V1, typename T1, cmp_t... Vs, typename... Ts>
			requires( V1 != CMP )
			struct In_impl<Case<V1, T1>, Case<Vs, Ts>...>
			{ using type = typename In_impl<Case<Vs, Ts>...>::type; };
			
			template<cmp_t V1, typename T1, cmp_t... Vs, typename... Ts>
			requires( V1 == CMP )
			struct In_impl<Case<V1, T1>, Case<Vs, Ts>...>
			{ using type = T1; };
			
			template<typename... CASES>
			using In = typename In_impl<CASES...>::type;
			
		};
		
	}
	
	namespace attempt2 {
		
		template<typename T, size_t axis_index>
		struct Axis;
		
		template<size_t axis_index>
		struct Axis<void, axis_index>
		{
			static constexpr bool
			is_void = true;
			
			constexpr auto&
			data() { return *this; }
			
			constexpr
			Axis(std::nullptr_t = nullptr)
			noexcept {}
		};
		
		template<typename T> requires(!std::same_as<T, void>)
		struct Axis<T, 0>
		{
			static constexpr bool
			is_void = false;
			
			constexpr auto&
			data() { return x; }
			
			constexpr
			Axis()
			noexcept( noexcept(T()) ):
			x() {}
			
			constexpr
			Axis(auto&& x)
			noexcept( noexcept(T( std::declval<T>() )) ):
			x(x) {}
			
			T x;
		};
		
		template<typename T> requires(!std::same_as<T, void>)
		struct Axis<T, 1>
		{
			static constexpr bool
			is_void = false;
			
			constexpr auto&
			data() { return y; }
			
			constexpr
			Axis()
			noexcept( noexcept(T()) ):
			y() {}
			
			constexpr
			Axis(auto&& y)
			noexcept( noexcept(T( std::declval<T>() )) ):
			y(y) {}
			
			T y;
			
		};
		
		template<typename T> requires(!std::same_as<T, void>)
		struct Axis<T, 2>
		{
			static constexpr bool
			is_void = false;
			
			constexpr auto&
			data() { return z; }
			
			constexpr
			Axis()
			noexcept( noexcept(T()) ):
			z() {}
			
			constexpr
			Axis(auto&& z)
			noexcept( noexcept(T( std::declval<T>() )) ):
			z(z) {}
			
			T z;
			
		};
		
		
		
		template<typename Axis1, typename Axis2, typename Axis3>
		struct VecBase;
		
		template<
			typename T_A1, size_t I_A1,
			typename T_A2, size_t I_A2,
			typename T_A3, size_t I_A3
		> struct VecBase<
			Axis<T_A1, I_A1>,
			Axis<T_A2, I_A2>,
			Axis<T_A3, I_A3>
		>: public Axis<T_A1, I_A1>, Axis<T_A2, I_A2>, Axis<T_A3, I_A3>
		{
			
			private: using
			A1 = Axis<T_A1, I_A1>;
			private: using
			A2 = Axis<T_A2, I_A2>;
			private: using
			A3 = Axis<T_A3, I_A3>;
			
			private: template<size_t Idx> static constexpr auto&&
			forward_param(auto&& x, auto&& y, auto&& z) noexcept {
				if constexpr(Idx == 0) return x;
				if constexpr(Idx == 1) return y;
				if constexpr(Idx == 2) return z;
			}
			
			public: constexpr
			VecBase()
				noexcept(noexcept(A1())					&& noexcept(A2())					&& noexcept(A3())					)
				requires(std::default_initializable<A1>	&& std::default_initializable<A2>	&& std::default_initializable<A3>	):
				A1(), A2(), A3()
			{}
			
			public: constexpr
			VecBase(auto&& x, auto&& y, auto&& z)
				noexcept(noexcept( A1(std::declval<A1>()) )	&&	noexcept( A2(std::declval<A2>()) )	&&	noexcept( A3(std::declval<A3>() ))):
				A1(forward_param<I_A1>(x, y, z)),
				A2(forward_param<I_A2>(x, y, z)),
				A3(forward_param<I_A3>(x, y, z))
			{}
			
		};
		
		template<typename X, typename Y, typename Z>
		using PackedVecBase
		=	typename internal
		::	AlignTypes_t<	Axis<X,0> , Axis<Y,1> , Axis<Z,2>	>
		::	rebind< VecBase >;
		
		template<typename X, typename Y = X, typename Z = Y>
		class Vec: public PackedVecBase<X, Y, Z>
		{
			template<typename,typename,typename> friend class Vec;
			
			private:
			using base = PackedVecBase<X, Y, Z>;
			using AxisX = Axis<X, 0>;
			using AxisY = Axis<Y, 1>;
			using AxisZ = Axis<Z, 2>;
			
			/* CONSTRUCTORS */
			
			public: constexpr
			Vec() noexcept(noexcept(base()))
			requires(std::default_initializable<base>):
			base() {}
			
			public: constexpr
			Vec(std::convertible_to<X> auto&& x,
				std::convertible_to<Y> auto&& y,
				std::convertible_to<Z> auto&& z)
				noexcept( noexcept(base( std::declval<X>() , std::declval<Y>() , std::declval<Z>() )) )
				requires(!AxisX::is_void && !AxisY::is_void && !AxisZ::is_void):
				base(x, y, z)
			{}
			
			public: constexpr
			Vec(std::convertible_to<X> auto&& x,
				std::convertible_to<Y> auto&& y)
				noexcept( noexcept(base( std::declval<X>() , std::declval<Y>() , nullptr )) )
				requires(!AxisX::is_void && !AxisY::is_void && AxisZ::is_void):
				base(x, y, nullptr)
			{}
			
			public: constexpr
			Vec(std::convertible_to<X> auto&& x,
				std::convertible_to<Z> auto&& z)
				noexcept( noexcept(base( std::declval<X>() , nullptr , std::declval<Z>() )) )
				requires(!AxisX::is_void && AxisY::is_void && !AxisZ::is_void):
				base(x, nullptr, z)
			{}
			
			public: constexpr
			Vec(std::convertible_to<Y> auto&& y,
				std::convertible_to<Z> auto&& z)
				noexcept( noexcept(base( nullptr , std::declval<Y>() , std::declval<Z>() )) )
				requires(AxisX::is_void && !AxisY::is_void && !AxisZ::is_void):
				base(nullptr, y, z)
			{}
			
			public: constexpr
			Vec(std::convertible_to<X> auto&& x)
				noexcept( noexcept(base( std::declval<X>() , nullptr , nullptr )) )
				requires(!AxisX::is_void && AxisY::is_void && AxisZ::is_void):
				base(x, nullptr, nullptr)
			{}
			
			public: constexpr
			Vec(std::convertible_to<Y> auto&& y)
				noexcept( noexcept(base( nullptr , std::declval<Y>() , nullptr )) )
				requires(AxisX::is_void && !AxisY::is_void && AxisZ::is_void):
				base(nullptr, y, nullptr)
			{}
			
			public: constexpr
			Vec(std::convertible_to<Z> auto&& z)
				noexcept( noexcept(base( nullptr , nullptr , std::declval<Z>() )) )
				requires(AxisX::is_void && AxisY::is_void && !AxisZ::is_void):
				base(nullptr, nullptr, z)
			{}
			
			/* GET */
			
			public: template<size_t Idx> friend constexpr decltype(auto)
			get(Vec & v) noexcept {
				if constexpr(Idx == 0 && !AxisX::is_void) return static_cast<AxisX*>(&v)->data();
				if constexpr(Idx == 1 && !AxisY::is_void) return static_cast<AxisY*>(&v)->data();
				if constexpr(Idx == 2 && !AxisZ::is_void) return static_cast<AxisZ*>(&v)->data();
			}
			
			public: template<size_t Idx> friend constexpr decltype(auto)
			get(Vec && v) noexcept
			{ return std::move(get<Idx>(v)); }
			
			public: template<size_t Idx> friend constexpr auto const&
			get(Vec const& v) noexcept
			{ return get<Idx>(const_cast<Vec&>(v)); }
			
			/* OPERATORS */
			
			private: static constexpr auto
			OP_add		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs + rhs)) { return lhs + rhs; },
			OP_sub		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs - rhs)) { return lhs - rhs; },
			OP_mul		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs * rhs)) { return lhs * rhs; },
			OP_div		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs / rhs)) { return lhs / rhs; },
			OP_mod		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs % rhs)) { return lhs % rhs; },
			OP_bitand	= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs & rhs)) { return lhs & rhs; },
			OP_bitor	= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs | rhs)) { return lhs | rhs; },
			OP_xor		= [](auto&& lhs, auto&& rhs) constexpr noexcept(noexcept(lhs ^ rhs)) { return lhs ^ rhs; };
			
			private: template<typename RX, typename RY, typename RZ>
			struct BinaryArithmeticFunc {
				using VecRHS = Vec<RX, RY, RZ>;
				template<auto OP, typename L, typename R>
				using Res = decltype( OP(std::declval<L>(), std::declval<R>()) );
				
				template<auto OP> static constexpr decltype(auto)
				operate(Vec const& lhs, VecRHS const& rhs)
				noexcept( noexcept(OP(lhs.x, rhs.x)) && noexcept(OP(lhs.y, rhs.y)) && noexcept(lhs.z, rhs.z) )
				{
					return Vec
					<Res<OP, X, RX>, Res<OP, Y, RY>, Res<OP, Z, RZ>>
					(OP(lhs.x, rhs.x), OP(lhs.y, rhs.y), OP(lhs.z, rhs.z));
				}
				
			};
			
		};
		
	}
	
	namespace detail {
		
		enum class AxisTag { x , y , z };
		
		template<typename T> struct XAxis
		{
			auto& data() noexcept		{ return x; }
			auto& data() const noexcept	{ return x; }
			T x;
		};
		
		template<typename T> struct YAxis
		{
			auto& data() noexcept		{ return y; }
			auto& data() const noexcept	{ return y; }
			T y;
		};
		
		template<typename T> struct ZAxis
		{
			auto& data() noexcept		{ return z; }
			auto& data() const noexcept	{ return z; }
			T z;
		};
		
		template<typename T, AxisTag tag> using AxisBase =
		typename internal::Switch<tag>::template In<
			typename internal::Switch<tag>:: Case<AxisTag::x, XAxis<T>>,
			typename internal::Switch<tag>:: Case<AxisTag::y, YAxis<T>>,
			typename internal::Switch<tag>:: Case<AxisTag::z, ZAxis<T>>
		>;
		
		AxisBase<int, AxisTag::x> f;
		template<typename T, AxisTag tag>
		class Axis;
		
		template<typename T, AxisTag tag>
		requires(!std::same_as<void,T>)
		class Axis<T, tag>: public AxisBase<T, tag> {
			private:
			using base = AxisBase<T, tag>;
			
			protected:
			using
			value_type = T;					
			
			using
			ctr_arg = value_type;
			
			static constexpr bool
			is_void = false;
			
			constexpr auto&
			data() const noexcept
			{ return static_cast<base const*>(this)->data(); }
			
			constexpr auto&
			data() noexcept
			{ return static_cast<base const*>(this)->data();; }
			
			public:
			constexpr
			Axis() noexcept(noexcept(T{})) requires(std::default_initializable<T>):
			base{} {}
			
			constexpr
			Axis(std::convertible_to<T> auto&& v) noexcept(noexcept(base{v})):
			base{v} {}
			
		};
		
		template<AxisTag tag>
		class Axis<void, tag> {
			public:
			constexpr Axis(auto&&) noexcept {}
			constexpr Axis(std::nullptr_t = nullptr) noexcept {}
			
			protected:
			using value_type = void;
			using ctr_arg = std::nullptr_t;
			static constexpr bool is_void = true;
			constexpr auto& data() const	noexcept { return *this; }
			constexpr auto& data()			noexcept { return *this; }
		};
		
		
		
		
		template<typename A1, typename A2, typename A3>
		struct Axies;
		
		template<typename T1, AxisTag t1, typename T2, AxisTag t2, typename T3, AxisTag t3>
		struct Axies
				<	Axis<T1, t1> , Axis<T2, t2> , Axis<T3, t3>	>:
		public	Axis<T1, t1> , Axis<T2, t2> , Axis<T3, t3>
		{
			
			protected:
			template<AxisTag at> static constexpr auto&
			raw_get_axis(Axies* axies) noexcept {
				if constexpr(at == t1) return *static_cast<Axis<T1, t1>*>(axies);
				if constexpr(at == t2) return *static_cast<Axis<T2, t2>*>(axies);
				if constexpr(at == t3) return *static_cast<Axis<T3, t3>*>(axies);
			}
			
			template<AxisTag at> static constexpr auto const&
			raw_get_axis(Axies const* axies) noexcept
			{ return const_cast<Axies*>(axies)->template raw_get_axis<at>(); }
			
			template<AxisTag at> using
			get_axis_t = typename internal::Switch<at>::template In<
				typename internal::Switch<at>::Case<t1, Axis<T1, t1>>,
				typename internal::Switch<at>::Case<t2, Axis<T2, t2>>,
				typename internal::Switch<at>::Case<t3, Axis<T3, t3>>
			>;
			
			using Axis1 = get_axis_t<t1>;
			using Axis2 = get_axis_t<t2>;
			using Axis3 = get_axis_t<t3>;
			
			using AxisX = get_axis_t<AxisTag::x>;
			using AxisY = get_axis_t<AxisTag::y>;
			using AxisZ = get_axis_t<AxisTag::z>;
			
			template<AxisTag at> static constexpr auto&&
			forward_correct_axis(
				std::convertible_to<typename AxisX::ctr_arg> auto&& x,
				std::convertible_to<typename AxisY::ctr_arg> auto&& y,
				std::convertible_to<typename AxisZ::ctr_arg> auto&& z
			) noexcept {
				if constexpr(at == AxisTag::x) return x;
				if constexpr(at == AxisTag::y) return y;
				if constexpr(at == AxisTag::z) return z;
			}
			
			public:
			constexpr
			Axies() noexcept
			( noexcept(Axis1()) && noexcept(Axis2()) && noexcept(Axis3()) )
			requires(	std::default_initializable<Axis1>
			&&			std::default_initializable<Axis2>
			&&			std::default_initializable<Axis3>
			):
				Axis1(),
				Axis2(),
				Axis3()
			{}
			
			constexpr
			Axies(	std::convertible_to<typename AxisX::ctr_arg> auto&& x,
					std::convertible_to<typename AxisY::ctr_arg> auto&& y,
					std::convertible_to<typename AxisZ::ctr_arg> auto&& z)
			noexcept(	noexcept(AxisX(x))	&&	noexcept(AxisY(y))	&&	noexcept(AxisZ(z))	)
			requires(requires(decltype(x) xarg, decltype(y) yarg, decltype(z) zarg) { AxisX(xarg); AxisY(yarg); AxisZ(zarg); }):
				Axis1(forward_correct_axis<t1>(x, y, z)),
				Axis2(forward_correct_axis<t2>(x, y, z)),
				Axis3(forward_correct_axis<t3>(x, y, z))
			{}
			
			constexpr
			Axies(	std::convertible_to<typename AxisX::ctr_arg> auto&& x,
					std::convertible_to<typename AxisY::ctr_arg> auto&& y)
			noexcept(	noexcept(AxisX(x))	&&	noexcept(AxisY(y))	)
			requires(requires(decltype(x) xarg, decltype(y) yarg) { AxisX(xarg); AxisY(yarg); } && !AxisX::is_void && !AxisY::is_void && AxisZ::is_void):
				Axis1(forward_correct_axis<t1>(x, y, nullptr)),
				Axis2(forward_correct_axis<t2>(x, y, nullptr)),
				Axis3(forward_correct_axis<t3>(x, y, nullptr))
			{}
			
			constexpr
			Axies(	std::convertible_to<typename AxisX::ctr_arg> auto&& x,
					std::convertible_to<typename AxisZ::ctr_arg> auto&& z)
			noexcept(	noexcept(AxisX(x))	&&	noexcept(AxisZ(z))	)
			requires(requires(decltype(x) xarg, decltype(z) zarg) { AxisX(xarg); AxisZ(zarg); } && !AxisX::is_void && AxisY::is_void && !AxisZ::is_void):
				Axis1(forward_correct_axis<t1>(x, nullptr, z)),
				Axis2(forward_correct_axis<t2>(x, nullptr, z)),
				Axis3(forward_correct_axis<t3>(x, nullptr, z))
			{}
			
			constexpr
			Axies(	std::convertible_to<typename AxisY::ctr_arg> auto&& y,
					std::convertible_to<typename AxisZ::ctr_arg> auto&& z)
			noexcept(	noexcept(AxisY(y))	&&	noexcept(AxisZ(z))	)
			requires(requires(decltype(y) yarg, decltype(z) zarg) { AxisY(yarg); AxisZ(zarg); } && AxisX::is_void && !AxisY::is_void && !AxisZ::is_void):
				Axis1(forward_correct_axis<t1>(nullptr, y, z)),
				Axis2(forward_correct_axis<t2>(nullptr, y, z)),
				Axis3(forward_correct_axis<t3>(nullptr, y, z))
			{}
			
			constexpr
			Axies(std::convertible_to<typename AxisX::ctr_arg> auto&& x)
			noexcept( noexcept(AxisX(x)) )
			requires(requires(decltype(x) xarg) { AxisX(xarg); } && !AxisX::is_void && AxisY::is_void && AxisZ::is_void ):
				AxisX(x)
			{}
			
			constexpr
			Axies(std::convertible_to<typename AxisY::ctr_arg> auto&& y)
			noexcept( noexcept(AxisY(y)) )
			requires(requires(decltype(y) yarg) { AxisX(yarg); } && AxisX::is_void && !AxisY::is_void && AxisZ::is_void ):
				AxisY(y)
			{}
			
			constexpr
			Axies(std::convertible_to<typename AxisZ::ctr_arg> auto&& z)
			noexcept( noexcept(AxisZ(z)) )
			requires(requires(decltype(z) zarg) { AxisZ(zarg); } && AxisX::is_void && AxisY::is_void && !AxisZ::is_void ):
				AxisZ(z)
			{}
			
			constexpr
			Axies([[maybe_unused]] std::nullptr_t x, [[maybe_unused]] std::nullptr_t y = nullptr, [[maybe_unused]] std::nullptr_t z = nullptr) noexcept
			requires(AxisX::is_void && AxisY::is_void && AxisZ::is_void) {}
			
			
			
		};
		
		template<typename X, typename Y = X, typename Z = Y>
		using Vec = internal::AlignTypes_t<Axis<X, AxisTag::x>, Axis<Y, AxisTag::y>, Axis<Z, AxisTag::z>>::template rebind<Axies>;
		
		void test() {
			
			
			
			
		}
		
	}
	
	
}

#endif
