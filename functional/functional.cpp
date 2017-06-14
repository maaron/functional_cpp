// functional.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <vector>
#include <algorithm>
#include <iterator>
#include <assert.h>
#include <memory>
#include <boost/optional.hpp>

#include "meta.h"

#include "functor.h"
#include "transformable.h"
#include "monoid.h"
#include "enumerable.h"
#include "applicative.h"

namespace functional
{
	template <class T>
	struct functor::typeclass<std::vector<T>> : instance
	{
		template <typename F>
		static std::vector<T> fmap(F&& f, const std::vector<T> v)
		{
			std::vector<T> copy;
			std::transform(v.begin(), v.end(),
				std::back_inserter(copy), std::forward<F>(f));
			return copy;
		}
	};

	template <class T>
	struct transformable::typeclass<std::vector<T>> : instance
	{
		template <typename F>
		static void transform(F&& f, std::vector<T>& v)
		{
			std::vector<T> copy;
			std::transform(v.begin(), v.end(),
				v.begin(), std::forward<F>(f));
		}
	};

	template <>
	struct monoid::typeclass<std::string>
	{
		static std::string mzero() { return ""; }

		static std::string mappend(std::string a, std::string b)
		{
			return a.append(b);
		}
	};

	template <typename Iter>
	struct iterator_pair
	{
		Iter current;
		Iter end;
	};

	template <typename Container>
	struct iterator_pair_storage
	{
		typedef typename Container::iterator Iter;
		std::shared_ptr<Container> container;
		Iter current;
		Iter end;
	};

	template <class T>
	struct enumerable::typeclass<std::vector<T>> : instance
	{
		typedef typename std::vector<T>::iterator iterator_t;

		template <typename T>
		static iterator_pair<iterator_t> enumerate(std::vector<T>& v)
		{
			return iterator_pair<iterator_t>{std::begin(v), std::end(v)};
		}

		template <typename T>
		static iterator_pair_storage<std::vector<T>> enumerate(std::vector<T>&& v)
		{
			auto container = std::make_shared<std::vector<T>>(std::move(v));
			return iterator_pair_storage<std::vector<T>>{
				container, std::begin(*container), std::end(*container)};
		}
	};

	template <class T>
	struct enumerator::typeclass<iterator_pair<T>> : instance
	{
		template <typename T>
		static bool is_end(const iterator_pair<T>& pair)
		{
			return pair.current == pair.end;
		}

		template <typename T>
		static auto current(const iterator_pair<T>& pair)
		{
			return *pair.current;
		}

		template <typename T>
		static iterator_pair<T> next(const iterator_pair<T>& pair)
		{
			return iterator_pair<T>{std::next(pair.current), pair.end};
		}
	};

	template <class T>
	struct enumerator::typeclass<iterator_pair_storage<T>> : instance
	{
		template <typename T>
		static bool is_end(const iterator_pair_storage<T>& pair)
		{
			return pair.current == pair.end;
		}

		template <typename T>
		static auto current(const iterator_pair_storage<T>& pair)
		{
			return *pair.current;
		}

		template <typename T>
		static iterator_pair_storage<T> next(const iterator_pair_storage<T>& pair)
		{
			return iterator_pair_storage<T>{pair.container, std::next(pair.current), pair.end};
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
		static auto apply(boost::optional<F>& optf, boost::optional<T>& opt)
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

	namespace composable
	{
		struct compose_impl
		{
			template <class F, class G>
			auto operator()(F&& f, G&& g)
			{
				return [&f, &g](auto&& a)
				{
					return f(g(a));
				};
			}

			template <class F>
			auto operator()(F&& f)
			{
				return [&f](auto&& g)
				{
					return compose(f, g);
				};
			}
		};

		compose_impl compose;

		template <class F, class G>
		auto operator >> (F&& f, G&& g) { return compose(g, f); }

		template <class F, class G>
		auto operator << (F&& f, G&& g) { return compose(f, g); }

		template <class F, class A>
		auto operator| (A&& a, F&& f)
		{
			return f(std::forward<A>(a));
		}
	}
}

int main()
{
	using namespace functional::functor;
	using namespace functional::transformable;
	using namespace functional::composable;
	using namespace functional::monoid;

	std::vector<int> v = { 1, 2, 3 };

	auto plus1 = [](auto&& i) { return i + 1; };
	auto times2 = [](auto&& i) { return i * 2; };

	//auto v1 = functor::typeclass<std::vector<int>>::fmap([](auto&& i) { return i + 1; }, v);

	auto v2 = fmap(plus1, v);

	auto partial = fmap(plus1);

	auto v22 = partial(v);

	transform(times2, v22);

	auto times2plus1 = compose(plus1, times2);

	auto times2plus1_2 =
		compose([](auto&& i) { return i + 1; });

	auto result = times2plus1(3);

	auto result2 = times2plus1_2(times2)(3);

	auto r3 = (times2 >> plus1)(3);

	auto r4 = (times2 << plus1)(3);

	auto r5 = r4 | times2;

	auto v5 = fmap(times2 >> plus1, v22);

	auto helloworld = mappend(std::string("hello"), std::string("world"));

	assert(mappend(zero)(std::string("asdf")) == std::string("asdf"));

	int one = 1;
	int two = 2;
	assert(two >> one == 1);

	assert((2 | 1) == 3);

	assert(1 << 1 == 2);
	assert(2 >> 1 == 1);

	auto vec = std::vector<int>{ 1, 2, 3 };
	auto vec2 = std::vector<int>{ 3, 4, 5 };

	{
		using namespace functional::enumerable;
		using namespace functional::enumerator;

		std::vector<int> v = { 1, 2, 3 };
		
		assert((enumerate(v) | is_end) == false);
		assert((enumerate(v) | current) == 1);
		assert((enumerate(v) | next | is_end) == false);
		assert((enumerate(v) | next | current) == 2);
		assert((enumerate(v) | next | next | is_end) == false);
		assert((enumerate(v) | next | next | current) == 3);
		assert((enumerate(v) | next | next | next | is_end) == true);

		auto rvalue = enumerate(std::vector<int>{1, 2, 3});

		assert((rvalue | is_end) == false);
		assert((rvalue | current) == 1);
		assert((rvalue | next | is_end) == false);
		assert((rvalue | next | current) == 2);
		assert((rvalue | next | next | is_end) == false);
		assert((rvalue | next | next | current) == 3);
		assert((rvalue | next | next | next | is_end) == true);
	}

	{
		using namespace functional::applicative;

		auto optf = boost::make_optional(plus1);
		auto opt2 = boost::make_optional(2);

		assert(apply(optf, opt2) == boost::make_optional(3));
	}

    return 0;
}

