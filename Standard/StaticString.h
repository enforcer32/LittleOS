#pragma once

#include <Standard/UniquePtr.h>
#include <Standard/Types.h>

namespace Std
{
	class StaticString
	{
	public:
		~StaticString();
		StaticString() = delete;
		StaticString(const StaticString&) = delete;
		StaticString& operator=(const StaticString&) = delete;
		StaticString(StaticString&&) = delete;
		StaticString& operator=(StaticString&&) = delete;

		UniquePtr<StaticString> Clone() const;
		const char* Data() const;
		size_t Size() const;
		bool Empty() const;

		static UniquePtr<StaticString> Create(const char* str);
		static UniquePtr<StaticString> Create(const char* str, size_t size);
		static UniquePtr<StaticString> Create(size_t size);

	private:
		StaticString(const char* str);
		StaticString(const char* str, size_t size);
		StaticString(size_t size);

		char* m_Data;
		size_t m_Size;
	};
}
