#pragma once

#include "typeclass.h"

namespace functional
{
	namespace category
	{
		// This could perhaps be implemented as a typeclass, but implementing an instance for two 
		// arbitrary callable types is beyond my meta-programming abilities.  For now, compose() only 
		// works on such types, which is fine for the time being.

		struct compose_impl
		{
			template <class F, class G>
			auto operator()(F&& f, G&& g) const
			{
				return [&f, &g](auto&& a)
				{
					return f(g(a));
				};
			}

			template <class F>
			auto operator()(F&& f) const
			{
				return [&f](auto&& g)
				{
					return compose(f, g);
				};
			}
		};

		constexpr compose_impl compose{};

		template <class F, class G>
		auto operator >> (F&& f, G&& g) { return compose(g, f); }

		template <class F, class G>
		auto operator << (F&& f, G&& g) { return compose(f, g); }
	}
}
