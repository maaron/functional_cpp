#pragma once

#include "functor.h"
#include "applicative.h"
#include "monad.h"

namespace functional
{
	template <class T>
	struct functor::typeclass<boost::optional<T>> : instance
	{
		template <class F, class T>
		static auto fmap(F&& f, const boost::optional<T>& opt)
		{
			typedef decltype(f(opt.get())) result_t;

			if (opt)
				return boost::make_optional(f(opt.get()));
			else
				return boost::optional<result_t>();
		}
	};

	template <class T>
	struct applicative::typeclass<boost::optional<T>> : instance
	{
		typedef T of_type;

		template <class T>
		static boost::optional<T> pure(T&& t)
		{
			return boost::make_optional(
				std::forward<T>(t));
		}

		template <class F, class T>
		static auto apply(const boost::optional<F>& optf, const boost::optional<T>& opt)
		{
			typedef decltype(optf.get()(opt.get())) R;
			if (optf && opt)
			{
				return boost::make_optional(
					optf.get()(opt.get()));
			}
			else return boost::optional<R>();
		}
	};

	template <>
	struct applicative::typeclass<boost::none_t> : instance
	{
		template <class T>
		static auto apply(const boost::none_t, T&& t)
		{
			return boost::none;
		}

		template <class T>
		static auto apply(T&& t, const boost::none_t)
		{
			return boost::none;
		}
	};

	template <class T>
	struct monad::typeclass<boost::optional<T>> : instance
	{
		template <class F, class T>
		static auto mbind(F&& f, const boost::optional<T>& opt)
		{
			return applicative::apply(
				applicative::pure(std::forward<F>(f)), 
				opt);
		}
	};
}
