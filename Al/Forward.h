#pragma once

#include <Al/TypeTraits.h>

namespace Al
{
	template< class T >
	T&& Forward(RemoveReference_t<T>& param)
	{
		return static_cast<T&&>(param);
	}

	template<class T>
	T&& Forward(RemoveReference_t<T>&& param)
	{
		return static_cast<T&&>(param);
	}
}
