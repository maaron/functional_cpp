// functional.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <vector>
#include <algorithm>
#include <iterator>
#include <assert.h>
#include <memory>
#include <boost/optional.hpp>

#include "functor.h"
#include "transformable.h"
#include "monoid.h"
#include "enumerable.h"
#include "applicative.h"
#include "category.h"

#include "container.h"
#include "optional.h"

template <class F, class A>
auto operator| (A&& a, F&& f)
{
	return f(std::forward<A>(a));
}

int main()
{
	using namespace functional::functor;
	using namespace functional::transformable;
	using namespace functional::category;
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
		assert(apply(optf, boost::none) == boost::none);

		// Need curried functions to make applicative really shine...
	}

    return 0;
}

