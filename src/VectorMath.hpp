#ifndef INKRYPTION_UTIL_VECTOR_MATH_HEADER_FILE_GUARD
#define INKRYPTION_UTIL_VECTOR_MATH_HEADER_FILE_GUARD

#include <tuple>

namespace ink::vec_math {
	
	namespace detail {
		
		static constexpr
		size_t	X = {	0		},
				Y = {	X + 1	},
				Z = {	Y + 1	}
		;
		
		template<	size_t X = detail::X,
					size_t Y = X + 1,
					size_t Z = Y + 1	>
		struct Axies {};
		
		template<typename axies = Axies<>> struct get_impl;
		
		template<size_t X, size_t Y, size_t Z, template<auto...> typename axies>
		struct get_impl<axies<X, Y, Z>> {
			
			template<size_t i> static constexpr size_t
			equiv() noexcept
			{
				if constexpr(i == detail::X) return X;
				if constexpr(i == detail::Y) return Y;
				if constexpr(i == detail::Z) return Z;
			}
			
			template<size_t axis, typename T> static constexpr decltype(auto)
			get_fwd(T&& v) noexcept
			(noexcept(	get<equiv<axis>()>(	std::forward<T>(	std::declval<T>()	)	)	))
			{ return	get<equiv<axis>()>(	std::forward<T>(	v					)	)	;}
			
		};
		
		
		
		template<size_t axis, typename axies = Axies<>, typename T> static constexpr decltype(auto)
		get_fwd(T&& v) noexcept
		(noexcept(	get_impl<axies>::template get_fwd<axis , T>(	std::forward<T>(	std::declval<T>()	)	)	))
		{ return	get_impl<axies>::template get_fwd<axis , T>(	std::forward<T>(	v					)	)	;}
		
		
		
		template<typename axies = Axies<>, typename T> static constexpr decltype(auto)
		x_of(T&& v) noexcept
		(noexcept(	get_fwd<X , axies>(	std::forward<T>(	std::declval<T>()	)	)	))
		{ return	get_fwd<X , axies>(	std::forward<T>(	v					)	)	;}
		
		template<typename axies = Axies<>, typename T> static constexpr decltype(auto)
		y_of(T&& v) noexcept
		(noexcept(	get_fwd<Y , axies>(	std::forward<T>(	std::declval<T>()	)	)	))
		{ return	get_fwd<Y , axies>(	std::forward<T>(	v					)	)	;}
		
		template<typename axies = Axies<>, typename T> static constexpr decltype(auto)
		z_of(T&& v) noexcept
		(noexcept(	get_fwd<Z , axies>(	std::forward<T>(	std::declval<T>()	)	)	))
		{ return	get_fwd<Z , axies>(	std::forward<T>(	v					)	)	;}
		
		
		
		template<typename T, size_t Idx, typename axies = Axies<>>
		using get_type = decltype( get_fwd<Idx, axies, T>(std::forward<T>(std::declval<T>())) );
		
		template<typename T>
		concept not_void = !std::same_as<T, void>;
		
		template<typename T, size_t Idx, typename axies = Axies<>>
		concept valid_get =			requires(T t)
		{ {get_fwd<Idx>(t)} -> not_void; };
		
		template<typename T, size_t Idx, typename axies = Axies<>>
		concept valid_get_cref	=	bool {
									(valid_get<T, Idx, axies>																)
		&&							(std::	is_reference_v	<								get_type<T, Idx, axies>		>	)
		&&							(std::	is_const_v		<	std::remove_reference_t<	get_type<T, Idx, axies>	>	>	)	};
		
		template<typename T, size_t Idx, typename axies = Axies<>>
		concept valid_get_ref	=	bool {
									(valid_get<T, Idx, axies>																)
		&&							(std::	is_reference_v	<								get_type<T, Idx, axies>		>	)
		&&							(!std::	is_const_v		<	std::remove_reference_t<	get_type<T, Idx, axies>	>	>	)	};
		
		
		
		template<typename X, typename Y, typename Z>
		struct Vec_Impl;
		
		template<typename X, typename Y, typename Z>
		requires( (alignof(X) >= alignof(Y)) && (alignof(X) >= alignof(Z)) && (alignof(Y) >= alignof(Z)) )
		struct Vec_Impl<X, Y, Z>
		{
			constexpr
			Vec_Impl() requires(std::is_default_constructible_v<X> && std::is_default_constructible_v<Y> && std::is_default_constructible_v<Z>):
			x(), y(), z() {}
			
			constexpr
			Vec_Impl(X && x, Y && y, Z&& z):
			x(x), y(y), z(z) {}
			
			constexpr
			Vec_Impl(X const& x, Y const& y, Z const& z):
			x(x), y(y), z(z) {}
			
			X x; Y y; Z z;
		};
		
		template<typename X, typename Y, typename Z>
		requires( (alignof(X) > alignof(Y)) && (alignof(X) >= alignof(Z)) && (alignof(Y) < alignof(Z)) )
		struct Vec_Impl<X, Y, Z>
		{
			constexpr
			Vec_Impl() requires(std::is_default_constructible_v<X> && std::is_default_constructible_v<Y> && std::is_default_constructible_v<Z>):
			x(), y(), z() {}
			
			constexpr
			Vec_Impl(X && x, Y && y, Z&& z):
			x(x), z(z), y(y) {}
			
			constexpr
			Vec_Impl(X const& x, Y const& y, Z const& z):
			x(x), z(z), y(y) {}
			
			X x; Z z; Y y;
		};
		
		
		
		template<typename X, typename Y, typename Z>
		requires( (alignof(X) <= alignof(Y)) && (alignof(X) >= alignof(Z)) && (alignof(Y) > alignof(Z)) )
		struct Vec_Impl<X, Y, Z>
		{
			constexpr
			Vec_Impl() requires(std::is_default_constructible_v<X> && std::is_default_constructible_v<Y> && std::is_default_constructible_v<Z>):
			x(), y(), z() {}
			
			constexpr
			Vec_Impl(X && x, Y && y, Z&& z):
			y(y), x(x), z(z) {}
			
			constexpr
			Vec_Impl(X const& x, Y const& y, Z const& z):
			y(y), x(x), z(z) {}
			
			Y y; X x; Z z;
		};
		
		template<typename X, typename Y, typename Z>
		requires( (alignof(X) < alignof(Y)) && (alignof(X) < alignof(Z)) && (alignof(Y) > alignof(Z)) )
		struct Vec_Impl<X, Y, Z>
		{
			constexpr
			Vec_Impl() requires(std::is_default_constructible_v<X> && std::is_default_constructible_v<Y> && std::is_default_constructible_v<Z>):
			x(), y(), z() {}
			
			constexpr
			Vec_Impl(X && x, Y && y, Z&& z):
			y(y), z(z), x(x) {}
			
			constexpr
			Vec_Impl(X const& x, Y const& y, Z const& z):
			y(y), z(z), x(x) {}
			
			Y y; Z z; X x;
		};
		
		
		
		template<typename X, typename Y, typename Z>
		requires( (alignof(X) >= alignof(Y)) && (alignof(X) < alignof(Z)) && (alignof(Y) < alignof(Z)) )
		struct Vec_Impl<X, Y, Z>
		{
			constexpr
			Vec_Impl() requires(std::is_default_constructible_v<X> && std::is_default_constructible_v<Y> && std::is_default_constructible_v<Z>):
			x(), y(), z() {}
			
			constexpr
			Vec_Impl(X && x, Y && y, Z&& z):
			z(z), x(x), y(y) {}
			
			constexpr
			Vec_Impl(X const& x, Y const& y, Z const& z):
			z(z), x(x), y(y) {}
			
			Z z; X x; Y y;
		};
		
		template<typename X, typename Y, typename Z>
		requires( (alignof(X) < alignof(Y)) && (alignof(X) < alignof(Z)) && (alignof(Y) < alignof(Z)) )
		struct Vec_Impl<X, Y, Z>
		{
			constexpr
			Vec_Impl() requires(std::is_default_constructible_v<X> && std::is_default_constructible_v<Y> && std::is_default_constructible_v<Z>):
			x(), y(), z() {}
			
			constexpr
			Vec_Impl(X && x, Y && y, Z&& z):
			z(z), y(y), x(x) {}
			
			constexpr
			Vec_Impl(X const& x, Y const& y, Z const& z):
			z(z), y(y), x(x) {}
			
			Z z; Y y; X x;
		};
		
		
		
		template<typename X, typename Y = X, typename Z = Y>
		struct Vec: public Vec_Impl<X, Y, Z> {
			
			public: constexpr
			Vec() requires(std::is_default_constructible_v<Vec_Impl<X,Y,Z>>): Vec_Impl<X,Y,Z>()
			{}
			
			public: constexpr
			Vec(X x, Y y, Z z): Vec_Impl<X,Y,Z>(x, y, z)
			{}
			
			private: template<typename axies = Axies<>>
			struct Ctr;
			
			private: template<size_t _x, size_t _y, size_t _z, template<auto...> typename axies>
			struct Ctr<axies<_x, _y, _z>> {
				friend struct Vec;
				
				template<typename T> constexpr
				operator T() const {
					T out{};
					if constexpr(valid_get_ref<T, detail::X, axies<_x, _y, _z>>) { x_of<axies<_x, _y, _z>>(out) = M_data->x; }
					if constexpr(valid_get_ref<T, detail::Y, axies<_x, _y, _z>>) { y_of<axies<_x, _y, _z>>(out) = M_data->y; }
					if constexpr(valid_get_ref<T, detail::Z, axies<_x, _y, _z>>) { z_of<axies<_x, _y, _z>>(out) = M_data->z; }
					return out;
				}
				
				private: Vec const*
				M_data{nullptr};
			};
			
			public: template<typename axies = Axies<>> constexpr auto
			construct() const
			{ Ctr<axies> ctr; ctr.M_data = this; return ctr; }
			
			public: template<typename T, typename axies = Axies<>> constexpr
			operator T() const
			{ return construct<axies>(); }
			
			
		};
		
		template<size_t axis, typename X, typename Y, typename Z> static constexpr auto&&
		get(Vec<X, Y, Z> const& v) {
			if constexpr(axis == detail::X) { return v.x; }
			if constexpr(axis == detail::Y) { return v.y; }
			if constexpr(axis == detail::Z) { return v.z; }
		}
		
		template<size_t axis, typename X, typename Y, typename Z> static constexpr auto&&
		get(Vec<X, Y, Z> && v) {
			if constexpr(axis == detail::X) { return v.x; }
			if constexpr(axis == detail::Y) { return v.y; }
			if constexpr(axis == detail::Z) { return v.z; }
		}
		
		template<size_t axis, typename X, typename Y, typename Z> static constexpr auto&&
		get(Vec<X, Y, Z> & v) {
			return get<axis>(std::move(v));
		}
		
		
		
		//template<
		//	typename axies_Lhs,
		//	typename axies_Rhs = axies_Lhs,
		//	size_t axis_Lhs,
		//	size_t axis_Rhs = axis_Lhs,
		//	valid_get<axis_Lhs, axies_Lhs> Lhs,
		//	valid_get<axis_Rhs, axies_Rhs> Rhs
		//> static constexpr decltype(auto)
		//add(Lhs&& lhs, Rhs&& rhs) {
			
		//}
		
		
	}
	
	using detail::X;
	using detail::Y;
	using detail::Z;
	
	using detail::Axies;
	
	using detail::x_of;
	using detail::y_of;
	using detail::z_of;
	
	using detail::Vec;
	
}

#endif
