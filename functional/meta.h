#pragma once

#include <type_traits>

namespace functional
{
	template <class T, class A, class = void>
	struct is_invocable : std::false_type
	{
	};

	template <class T>
	struct to_void
	{
		typedef void type;
	};

	template <class T, class A>
	struct is_invocable<T, A,
		typename to_void<typename std::result_of<T(A)>::type>::type>
		: std::true_type
	{
		typedef typename std::result_of<T(A)>::type result_type;
	};

}

