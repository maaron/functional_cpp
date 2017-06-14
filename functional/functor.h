#pragma once

#include "typeclass.h"

namespace functional
{
	namespace functor
	{
		template <class T>
		struct typeclass : ninstance {};

		struct fmap_impl
		{
			template <class F, class T>
			auto operator()(F&& f, T&& t) const
			{
				return typeclass<std::remove_reference<T>::type>::fmap(
					std::forward<F>(f),
					std::forward<T>(t));
			}

			template <class F>
			auto operator()(F&& f) const
			{
				return [f = std::forward<F>(f)](auto&& t)
				{
					return fmap(f, std::forward<decltype(t)>(t));
				};
			}
		};

		constexpr fmap_impl fmap{};
	}
}