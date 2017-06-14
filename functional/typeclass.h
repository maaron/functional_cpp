#pragma once

namespace functional
{
	struct instance
	{
		static constexpr bool is_instance = true;
	};

	struct ninstance
	{
		static constexpr bool is_instance = false;
	};

	template <class T, template <class> class Typeclass>
	struct instance_of
	{
		typedef Typeclass<typename std::remove_reference<T>::type> instance;
		static constexpr bool value = instance::is_instance;
	};
}
