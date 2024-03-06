#pragma once

namespace Std
{
	template<typename T>
	constexpr T&& Move(T& arg)
	{
		return static_cast<T&&>(arg);
	}
}
