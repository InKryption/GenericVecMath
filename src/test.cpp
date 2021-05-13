#include <stdio.h>

#include <tuple>
#include <cstdint>

namespace ink {
	
	namespace utility {
		
		// Type which is aligned as T, or with alignment of "if_no_align" if T does not have an alignment.
		template<typename T, size_t if_no_align = 1>
		struct alignas(if_no_align)
		align_wrapper {};
		
		template<typename T, size_t if_no_align>
		requires requires { {alignof(T)} -> std::integral; } && (!std::is_void_v<T>)
		struct alignas(T)
		align_wrapper<T, if_no_align> {};
		
	}
	
	namespace generic_vec::detail {
		
		struct NoState {};
		
		struct Comp_t { enum Comp: uint8_t { X = 0x01, Y = 0x02 , Z = 0x04 , W = 0x08 }; };
		using Comp = Comp_t::Comp;
		
		template<typename T, Comp tag>
		struct has_component_t:
		std::bool_constant<
				(!(tag & Comp::X) || requires(T t) { {t.x}; } )
			&&	(!(tag & Comp::Y) || requires(T t) { {t.y}; } )
			&&	(!(tag & Comp::Z) || requires(T t) { {t.z}; } )
			&&	(!(tag & Comp::W) || requires(T t) { {t.w}; } )
		> {};
		
		template<typename, Comp>	struct Member;
		template<typename T> requires(!std::is_void_v<T>) struct Member<T, Comp::X> { T x; };
		template<typename T> requires(!std::is_void_v<T>) struct Member<T, Comp::Y> { T y; };
		template<typename T> requires(!std::is_void_v<T>) struct Member<T, Comp::Z> { T z; };
		template<typename T> requires(!std::is_void_v<T>) struct Member<T, Comp::W> { T w; };
		template<Comp tag> struct Member<void, tag> {};
		
		template<typename T, Comp tag>
		struct Axis: public Member<T, tag> {
			
			private: using base = Member<T, tag>;
			private: using default_ctr_arg = NoState;
			
			public: template<std::convertible_to<T> U>
			requires (!std::same_as<std::remove_cvref_t<U>, default_ctr_arg>)
			constexpr explicit
			Axis(U&& v)
			noexcept(noexcept(base{std::declval<U>()}))
			: base{std::forward<U>(v)} {}
			
			public: template<typename U>
			requires (std::default_initializable<base> && std::same_as<std::remove_cvref_t<U>, default_ctr_arg>)
			constexpr explicit
			Axis(U&&)
			noexcept(noexcept(base{}))
			: base{} {}
			
		};
		
		template<typename T> using AxisX = Axis<T, Comp::X>;
		template<typename T> using AxisY = Axis<T, Comp::Y>;
		template<typename T> using AxisZ = Axis<T, Comp::Z>;
		template<typename T> using AxisW = Axis<T, Comp::W>;
		
		template<size_t i, typename X, typename Y, typename Z, typename W,
			typename AX = AxisX<X>,
			typename AY = AxisY<Y>,
			typename AZ = AxisZ<Z>,
			typename AW = AxisW<W>,
			size_t ix = 3 - (alignof(AX)>=alignof(AY)) - (alignof(AX)>=alignof(AZ)) - (alignof(AX)>=alignof(AW)),
			size_t iy = 3 - (alignof(AY)> alignof(AX)) - (alignof(AY)>=alignof(AZ)) - (alignof(AY)>=alignof(AW)),
			size_t iz = 3 - (alignof(AZ)> alignof(AX)) - (alignof(AZ)> alignof(AY)) - (alignof(AZ)>=alignof(AW)),
			size_t iw = 3 - (alignof(AW)> alignof(AX)) - (alignof(AW)> alignof(AY)) - (alignof(AW)> alignof(AZ))
		> using align_axis_at =
		std::conditional_t<(i == ix), AX,
		std::conditional_t<(i == iy), AY,
		std::conditional_t<(i == iz), AZ,
		std::conditional_t<(i == iw), AW,
		void
		>
		>
		>
		>;
		
		template<typename...>
		struct VecBase;
		
		template<typename X, typename Y, typename Z, typename W>
		struct VecBase<X, Y, Z, W>:
			public align_axis_at<0, X, Y, Z, W>,
			public align_axis_at<1, X, Y, Z, W>,
			public align_axis_at<2, X, Y, Z, W>,
			public align_axis_at<3, X, Y, Z, W>
		{
			using base0 = align_axis_at<0, X, Y, Z, W>;
			using base1 = align_axis_at<1, X, Y, Z, W>;
			using base2 = align_axis_at<2, X, Y, Z, W>;
			using base3 = align_axis_at<3, X, Y, Z, W>;
			
			constexpr
			VecBase():
				base0{NoState{}},
				base1{NoState{}},
				base2{NoState{}},
				base3{NoState{}}
			{}
		};
		
	}
	
}

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
	
	using namespace ink::generic_vec::detail;
	
	
	
	return 0;
}
