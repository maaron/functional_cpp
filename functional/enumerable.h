#pragma once

namespace functional
{
	namespace enumerable
	{
		template <class T>
		struct typeclass : ninstance {};

		struct enumerate_impl
		{
			template <class T>
			auto operator()(T&& t) const
			{
				return instance_of_t<T, typeclass>::enumerate(
					std::forward<T>(t));
			}
		};

		constexpr enumerate_impl enumerate{};
	}

	namespace enumerator
	{
		template <class T>
		struct typeclass : ninstance {};

		struct is_end_impl
		{
			template <class T>
			bool operator()(T&& t) const
			{
				return instance_of_t<T, typeclass>::is_end(
					std::forward<T>(t));
			}
		};

		constexpr is_end_impl is_end{};

		struct current_impl
		{
			template <class T>
			auto operator()(T&& t) const
			{
				return instance_of_t<T, typeclass>::current(
					std::forward<T>(t));
			}
		};

		constexpr current_impl current{};

		struct next_impl
		{
			template <class T>
			auto operator()(T&& t) const
			{
				return instance_of_t<T, typeclass>::next(
					std::forward<T>(t));
			}
		};

		constexpr next_impl next{};

	}
}
