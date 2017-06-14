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

		struct pure_impl
		{
			template <class T>
			auto operator()(T&& t) const
			{
				return typeclass<std::remove_reference<T>::type>::pure(
					std::forward<T>(t));
			}
		};
		constexpr pure_impl pure{};

		struct apply_impl
		{
			template <class F, class T>
			auto operator()(F&& f, T&& t) const
			{
				using Finst = instance_of<F, typeclass>;
				using Tinst = instance_of<T, typeclass>;
				
				static_assert(Finst::value, "F must be an applicative instance");
				static_assert(Tinst::value, "T must be an applicative instance");

				typedef typename Finst::instance::of_type Farg;
				typedef typename Tinst::instance::of_type Targ;
				static_assert(is_invocable<Farg, Targ>::value, 
					"F must be an applicative whose type argument is a callable taking the type argument of T");

				return typeclass<std::remove_reference<T>::type>::apply(
					std::forward<F>(f),
					std::forward<T>(t));
			}
		};
		// Applicative f => f (a -> b) -> f a -> f b
		constexpr apply_impl apply{};
	}
}
