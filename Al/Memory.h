#pragma once

#include <Al/Types.h>
#include <Al/Utility.h>
#include <Al/Forward.h>

namespace Al
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

		UniquePtr(UniquePtr&& rhs)
			: m_Ptr(rhs.Release())
		{
		}

		UniquePtr& operator=(UniquePtr&& rhs)
		{
			if (this != &rhs)
				Reset(rhs.Release());
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

		bool operator==(const UniquePtr& rhs) const
		{
			return *m_Ptr == *rhs.m_Ptr;
		}

		bool operator!=(const UniquePtr& rhs) const
		{
			return *m_Ptr != *rhs.m_Ptr;
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

		void Reset(T* ptr = nullptr)
		{
			if (m_Ptr)
				delete m_Ptr;
			m_Ptr = ptr;
		}

	private:
		T* m_Ptr;
	};

	template <typename T>
	class SharedPtr
	{
	public:
		SharedPtr()
			: m_Ptr(nullptr), m_Counter(nullptr)
		{
		}

		explicit SharedPtr(T* ptr)
			: m_Ptr(ptr)
		{
			m_Counter = new size_t;
			*m_Counter = 1;
		}

		SharedPtr(SharedPtr<T>& rhs)
		{
			m_Ptr = rhs.m_Ptr;
			m_Counter = rhs.m_Counter;
			(*m_Counter)++;
		}

		~SharedPtr()
		{
			(*m_Counter)--;
			if (*m_Counter == 0)
			{
				delete m_Counter;
				if (m_Ptr)
					delete m_Ptr;
			}
		}

		SharedPtr(SharedPtr&& rhs)
			: m_Ptr(rhs.Release())
		{
		}

		SharedPtr& operator=(SharedPtr&& rhs)
		{
			if (this != &rhs)
				m_Ptr = rhs.Release();
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

		bool operator==(const SharedPtr& rhs) const
		{
			return *m_Ptr == *rhs.m_Ptr;
		}

		bool operator!=(const SharedPtr& rhs) const
		{
			return *m_Ptr != *rhs.m_Ptr;
		}

		T* Get()
		{
			return m_Ptr;
		}

		T* Release()
		{
			T* ptr = m_Ptr;
			m_Ptr = nullptr;
			(*m_Counter)--;
			return ptr;
		}

		void Reset(T* ptr = nullptr)
		{
			Release();
			m_Ptr = ptr;
			m_Counter = new size_t;
			*m_Counter = 1;
		}

		size_t RefCount() const
		{
			return *m_Counter;
		}

	private:
		T* m_Ptr;
		size_t* m_Counter;
	};

	template<typename T, typename... Args>
	UniquePtr<T> MakeUnique(Args&&... args)
	{
		return UniquePtr<T>(new T(Forward<Args>(args)...));
	}

	template<typename T, typename... Args>
	SharedPtr<T> MakeShared(Args&&... args)
	{
		return SharedPtr<T>(new T(Forward<Args>(args)...));
	}
}
