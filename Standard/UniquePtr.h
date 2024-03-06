#pragma once

namespace Std
{
	template <typename T>
	class UniquePtr
	{
	public:
		UniquePtr(const UniquePtr&) = delete;
		UniquePtr& operator=(const UniquePtr&) = delete;

		UniquePtr()
			: m_Ptr(nullptr)
		{
		}

		explicit UniquePtr(T* ptr)
			: m_Ptr(ptr)
		{
		}

		~UniquePtr()
		{
			if (m_Ptr)
				delete m_Ptr;
		}

		UniquePtr(UniquePtr&& other)
			: m_Ptr(other.Release())
		{
		}

		UniquePtr& operator=(UniquePtr&& other)
		{
			if (this != &other)
				Reset(other.Release());
			return *this;
		}

		explicit operator bool() const
		{
			return m_Ptr != nullptr;
		}

		T* operator->() const
		{
			return m_Ptr;
		}

		T& operator*() const
		{
			return *m_Ptr;
		}

		T* Get()
		{
			return m_Ptr;
		}

		T* Release()
		{
			T* ptr = m_Ptr;
			m_Ptr = nullptr;
			return ptr;
		}

		T* Reset(T* ptr = nullptr)
		{
			if (m_Ptr)
				delete m_Ptr;
			m_Ptr = ptr;
		}

	private:
		T* m_Ptr;
	};
}
