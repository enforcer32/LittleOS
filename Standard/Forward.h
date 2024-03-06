#pragma once

#include <Standard/TypeTraits.h>

namespace Std
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
