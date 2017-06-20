#pragma once

#include <type_traits>

namespace functional
{
	template<class T>
	struct remove_rcv : std::remove_cv<typename std::remove_reference<T>::type> {};

	struct instance
	{
		static constexpr bool is_instance = true;
	};

	struct ninstance
	{
		static constexpr bool is_instance = false;
	};

	template <class T, template <class> class Typeclass>
	struct instance_of
	{
		typedef Typeclass<typename std::decay<T>::type> instance;
		static constexpr bool value = instance::is_instance;
	};

	template <class T, template <class> class Typeclass>
	using instance_of_t = Typeclass<typename remove_rcv<T>::type>;
}
