#pragma once

namespace Std
{
	template<class T>
	struct RemoveReference
	{ 
		typedef T Type;
	};

	template<class T>
	struct RemoveReference<T&>
	{ 
		typedef T Type;
	};

	template<class T>
	struct RemoveReference<T&&>
	{ 
		typedef T Type;
	};

	template<class T>
	using RemoveReference_t = typename RemoveReference<T>::Type;
}
