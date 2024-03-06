#pragma once

#include <Standard/Types.h>
#include <Standard/Utility.h>

namespace Std
{
	template<typename T>
	class Vector
	{
	public:
		Vector()
			: m_Data(nullptr), m_Size(0), m_Capacity(0)
		{
			Resize(10);
		}

		~Vector()
		{
			Clear();
			::operator delete(m_Data, m_Capacity * sizeof(T));	
		}

		const T& operator[](size_t index) const
		{
			return m_Data[index];
		}

		T& operator[](size_t index)
		{
			return m_Data[index];
		}

		void PushBack(const T& value)
		{
			if (m_Size >= m_Capacity)
				Resize(m_Capacity * 2);

			m_Data[m_Size++] = value;
		}

		void PushBack(T&& value)
		{
			if (m_Size >= m_Capacity)
				Resize(m_Capacity * 2);

			m_Data[m_Size++] = Std::Move(value);
		}

		void PopBack()
		{
			if (m_Size > 0)
			{
				m_Size--;
				m_Data[m_Size].~T();
			}
		}

		void Clear()
		{
			for (size_t i = 0; i < m_Size; i++)
				m_Data[m_Size].~T();
			m_Size = 0;
		}

		size_t Size() const
		{
			return m_Size;
		}

		bool Empty() const
		{
			return m_Size == 0;
		}

	private:
		void Resize(size_t capacity)
		{
			T* data = (T*)::operator new(capacity * sizeof(T));
			for (size_t i = 0; i < m_Size; i++)
				data[i] = Std::Move(m_Data[i]);
			for (size_t i = 0; i < m_Size; i++)
				m_Data[m_Size].~T();
			::operator delete(m_Data, m_Capacity * sizeof(T));
			m_Data = data;
			m_Capacity = capacity;
		}

	private:
		T* m_Data;
		size_t m_Size;
		size_t m_Capacity;
	};
}
