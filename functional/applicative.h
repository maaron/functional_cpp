#pragma once

#include "typeclass.h"
#include "meta.h"

namespace functional
{
	namespace applicative
	{
		template <class T>
		struct typeclass : ninstance
		{
			typedef void of_type;
		};

		template <class T>
		struct pure_value { T value; };

		struct apply_impl
		{
			template <class Instance>
			struct lift_value
			{
				template <class T>
				static auto of(T&& t)
				{
					return std::forward<T>(t);
				}

				template <class T>
				static auto of(pure_value<T>&& pv)
				{
					return Instance::pure(std::forward<T>(pv.value));
				}
			};

			template <class F, class T>
			auto operator()(F&& f, T&& t) const
			{
				return typeclass<std::decay<T>::type>::apply(
					lift_value<typeclass<std::decay<T>::type>>::of(std::forward<F>(f)),
					std::forward<T>(t));
			}

			template <class F>
			auto operator()(F&& f)
			{
				return[f = std::forward<F>(f)](auto&& t)
				{
					return apply(f, std::forward<decltype(t)>(t));
				}
			}
		};
		// Applicative f => f (a -> b) -> f a -> f b
		constexpr apply_impl apply{};

		struct pure_impl
		{
			template <class T>
			auto operator()(T&& t) const
			{
				return pure_value<std::remove_reference<T>::type>{ std::forward<T>(t) };
			}
		};
		// Applicative f => a -> f a
		constexpr pure_impl pure{};
	}
}
