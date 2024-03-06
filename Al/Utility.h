#pragma once

namespace Al
{
	template<typename T>
	constexpr T&& Move(T& arg)
	{
		return static_cast<T&&>(arg);
	}
}
