#pragma once

namespace Al
{
	template<typename T, size_t TSize>
	class Array
	{
	public:
		constexpr size_t Size()
		{
			return TSize;
		}

		T& operator[](size_t index)
		{
			return m_Data[index];
		}

		const T& operator[](size_t index) const
		{
			return m_Data[index];
		}

		const T* Data() const
		{
			return m_Data;
		}

	private:
		T m_Data[TSize];
	};
}
