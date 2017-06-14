#pragma once

#include "typeclass.h"

namespace functional
{
	namespace transformable
	{
		template <class T>
		struct typeclass : ninstance {};

		struct transform_impl
		{
			template <class F, class T>
			auto operator()(F&& f, T&& t) const
			{
				typeclass<std::remove_reference<T>::type>::transform(
					std::forward<F>(f),
					std::forward<T>(t));
			}

			template <class F>
			auto operator()(F&& f) const
			{
				return[f = std::forward<F>(f)](auto&& t)
				{
					transform(
						std::forward<F>(f),
						std::forward<T>(t));
				};
			}
		};

		constexpr transform_impl transform{};
	}
}