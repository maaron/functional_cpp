#pragma once

#include "typeclass.h"

namespace functional
{
	namespace monoid
	{
		template <class T>
		struct typeclass {};

		struct zero_impl {};

		constexpr zero_impl zero{};

		struct mappend_impl
		{
			template <typename T>
			auto operator()(T&& a, T&& b) const
			{
				return typeclass<T>::mappend(
					std::forward<T>(a),
					std::forward<T>(b));
			}

			template <typename T>
			auto operator()(zero_impl, T&& b) const
			{
				return typeclass<T>::mappend(
					typeclass<T>::mzero(),
					std::forward<T>(b));
			}

			template <typename T>
			auto operator()(T&& a, zero_impl) const
			{
				return typeclass<T>::mappend(
					std::forward<T>(b),
					typeclass<T>::mzero());
			}

			template <typename T>
			auto operator()(T&& a) const
			{
				return[fa = std::forward<T>(a)](auto&& b)
				{
					return mappend(
						fa,
						std::forward<decltype(b)>(b));
				};
			}
		};

		constexpr mappend_impl mappend{};
	}
}
