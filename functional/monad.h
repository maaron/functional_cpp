#pragma once

#include "typeclass.h"

namespace functional
{
	namespace monad
	{
		template <class T>
		struct typeclass : ninstance {};

		struct mbind_impl
		{
			template <class F, class T>
			auto operator()(F&& f, T&& t) const
			{
				return instance_of_t<T, typeclass>::mbind(
					std::forward<F>(f),
					std::forward<T>(t));
			}

			template <class F>
			auto operator()(F&& f) const
			{
				return[f = std::forward<F>(f)](auto&& t)
				{
					return mbind(f, std::forward<decltype(t)>(t));
				};
			}
		};

		constexpr mbind_impl mbind{};
	}
}

/*

m >>= [=](auto&& x) { x + 1; }

MDO(
	(x, m),
	(x + 1)
)

*/
#define ARG1(a, b) a

#define ARG2(a, b) b

#define MDO(bindexp, retexp) (ARG2(bindexp) >>= [=](auto&& ARG1(bindexp)) { retexp; })
