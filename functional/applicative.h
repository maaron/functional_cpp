#pragma once

#include "typeclass.h"

namespace functional
{
	namespace applicative
	{
		template <class T>
		struct typeclass : ninstance
		{
			typedef void of_type;
		};

		struct apply_impl
		{
			template <class Instance>
			struct lift_value
			{
				template <class T, class Instance>
				auto of(T&& t)
				{
					return std::forward<T>(t);
				}

				template <class T, class Instance>
				auto of(pure_value<T>&& pv)
				{
					return Instance::pure(std::forward<T>(pv.value));
				}
			};

			template <class F, class T>


			template <class F, class T>
			auto operator()(F&& f, T&& t) const
			{
				using Finst = instance_of<F, typeclass>;
				using Tinst = instance_of<T, typeclass>;
				
				static_assert(Finst::value, "F must have an applicative instance");
				static_assert(Tinst::value, "T must have an applicative instance");
				static_assert(is_invocable<
					Finst::instance::of_type, 
					Tinst::instance::of_type>::value,
					"F must have a type argument that is callable with the type argument of T");

				return typeclass<std::remove_reference<T>::type>::apply(
					std::forward<F>(f),
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

			template <class F, class T>
			auto operator()(F&& f, pure_value<T>&& pv)
			{
				return apply(
					std::forward<F>(f),
					typeclass<std::remove_reference<F>::type>::pure(
						std::forward<pure_value<T>>(pv)));
			}

			template <class F, class T>
			auto operator()(pure_value<F>&& pv, T&& t)
			{
				return apply(
					typeclass<std::remove_reference<F>::type>::pure(
						std::forward<pure_value<F>>(pv)),
					std::forward<T>(t));
			}
		};
		// Applicative f => f (a -> b) -> f a -> f b
		constexpr apply_impl apply{};

		template <class T>
		struct pure_value { T value; };

		struct pure_impl
		{
			template <class T>
			auto operator()(T&& t) const
			{
				return pure_value{ std::forward<T>(t) };
			}
		};
		// Applicative f => a -> f a
		constexpr pure_impl pure{};
	}
}
