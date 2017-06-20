#pragma once

#include "functor.h"
#include "transformable.h"
#include "monoid.h"
#include <vector>
#include <list>
#include <string>

namespace functional
{
	template <template <class, class> class Container, class T, template <class> class A>
	struct std_container_functor : instance
	{
		template <class F>
		static auto fmap(F&& f, const Container<T, A<T>>& c)
		{
			typedef std::result_of<F(T)>::type R;

			Container<R, A<R>> copy;

			std::transform(std::begin(c), std::end(c),
				std::back_inserter(copy), std::forward<F>(f));

			return copy;
		}
	};

	template <class T, template <class> class A>
	struct functor::typeclass<std::vector<T, A<T>>> : std_container_functor<std::vector, T, A> {};

	template <class T, template <class> class A>
	struct functor::typeclass<std::list<T, A<T>>> : std_container_functor<std::list, T, A> {};

	template <class Container>
	struct container_transformable : instance
	{
		template <class F>
		static void transform(F&& f, Container& c)
		{
			std::transform(std::begin(c), std::end(c),
				std::begin(c), std::forward<F>(f));
		}
	};

	template <class T, template <class> class A>
	struct transformable::typeclass<std::vector<T, A<T>>> : container_transformable<std::vector<T, A<T>>> {};

	template <class T, template <class> class A>
	struct transformable::typeclass<std::list<T, A<T>>> : container_transformable<std::list<T, A<T>>> {};

	template <class Elem, class Traits, template <class> class Allocator>
	struct functor::typeclass<std::basic_string<Elem, Traits, Allocator>> : container_transformable<std::basic_string<Elem, Traits, Allocator>> {};

	template <>
	struct monoid::typeclass<std::string>
	{
		static std::string mzero() { return ""; }

		static std::string mappend(std::string a, std::string b)
		{
			return a.append(b);
		}
	};

	template <template <class, class> class Container, class T, template <class> class A>
	struct container_monoid : instance
	{
		typedef Container<T, A<T>> container_t;

		static container_t mzero() { return container_t{}; }
		
		template <class F>
		static auto mapppend(const container_t& a, const container_t& b)
		{
			container_t copy;

			auto it = std::copy(std::begin(a), std::end(a),
				std::back_inserter(copy));

			std::copy(std::begin(b), std::begin(b), it);

			return copy;
		}
	};

	template <class T, template <class> class A>
	struct monoid::typeclass<std::vector<T, A<T>>> : container_monoid<std::vector, T, A> {};

	template <class T, template <class> class A>
	struct monoid::typeclass<std::list<T, A<T>>> : container_monoid<std::list, T, A> {};

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

	template <class Container>
	struct container_enumerable : instance
	{
		typedef typename Container::iterator iterator_t;

		static iterator_pair<iterator_t> enumerate(Container& v)
		{
			return iterator_pair<iterator_t>{std::begin(v), std::end(v)};
		}

		static iterator_pair_storage<Container> enumerate(Container&& v)
		{
			auto container = std::make_shared<Container>(std::move(v));
			return iterator_pair_storage<Container>{
				container, std::begin(*container), std::end(*container)};
		}
	};

	template <class T>
	struct enumerable::typeclass<std::vector<T>> : container_enumerable<std::vector<T>> {};
	
	template <class T>
	struct enumerable::typeclass<std::list<T>> : container_enumerable<std::list<T>> {};

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
}