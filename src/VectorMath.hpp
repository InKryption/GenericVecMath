#ifndef INKRYPTION_UTIL_VECTOR_MATH_HEADER_FILE_GUARD
#define INKRYPTION_UTIL_VECTOR_MATH_HEADER_FILE_GUARD

#include <tuple>

namespace ink::vec_math {
	
	namespace detail {
		
		template<typename T, size_t Idx>
		struct Conditional_Type
		{ using type = void; };
		
		template<typename T, size_t Idx> requires requires(T t) { get<Idx>(t); }
		struct Conditional_Type<T, Idx>
		{ using type = decltype(get<Idx>(std::declval<T>())); };
		
		template<typename T, size_t Idx>
		requires	requires(T			t) { get<Idx>(t); }
		||			requires(T &		t) { get<Idx>(t); }
		||			requires(T &&		t) { get<Idx>(t); }
		||			requires(T const&	t) { get<Idx>(t); }
		struct AxisAPI {
			using Val_Ret	=	typename Conditional_Type<T			,	Idx>::	type;
			using Ref_Ret	=	typename Conditional_Type<T &		,	Idx>::	type;
			using Rval_Ret	=	typename Conditional_Type<T &&		,	Idx>::	type;
			using Cref_Ret	=	typename Conditional_Type<T const&	,	Idx>::	type;
			
			static constexpr Ref_Ret get_fwd(T & t)
			requires( !std::same_as<Ref_Ret, void> )
			{ return get<Idx>(std::forward<T&>(t)); }
			
			static constexpr Rval_Ret get_fwd(T && t)
			requires( !std::same_as<Rval_Ret, void> )
			{ return get<Idx>(std::forward<T&&>(t)); }
			
			static constexpr Cref_Ret get_fwd(T const& t)
			requires( !std::same_as<Cref_Ret, void> )
			{ return get<Idx>(std::forward<T const&>(t)); }
			
		};
		
		// Specializations so that everything comes back to a T handle of everything
		
		template<typename T, size_t Idx> struct AxisAPI<T	const&,	Idx>: public AxisAPI<T, Idx> {};
		template<typename T, size_t Idx> struct AxisAPI<T	&&,		Idx>: public AxisAPI<T, Idx> {};
		template<typename T, size_t Idx> struct AxisAPI<T	&,		Idx>: public AxisAPI<T, Idx> {};
		
		template<typename T> static constexpr decltype(auto)
		x_of(T&& v)
		{ return AxisAPI<T, 0>::get_fwd(std::forward<T>(v)); }
		
		template<typename T> static constexpr decltype(auto)
		y_of(T&& v)
		{ return AxisAPI<T, 1>::get_fwd(std::forward<T>(v)); }
		
		template<typename T> static constexpr decltype(auto)
		z_of(T&& v)
		{ return AxisAPI<T, 2>::get_fwd(std::forward<T>(v)); }
		
	}
	
	using detail::x_of;
	using detail::y_of;
	using detail::z_of;
	
}

#endif
