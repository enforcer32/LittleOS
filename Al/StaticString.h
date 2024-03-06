#pragma once

#include <Al/Memory.h>
#include <Al/StringView.h>

namespace Al
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

		bool operator==(const char* rhs) const;
		bool operator==(const StaticString& rhs) const;
		bool operator==(StringView rhs) const;
		bool operator!=(const char* rhs) const;
		bool operator!=(const StaticString& rhs) const;
		bool operator!=(StringView rhs) const;

		UniquePtr<StaticString> Clone() const;
		const char* Data() const;
		size_t Size() const;
		bool Empty() const;
		StringView View() const;

		static UniquePtr<StaticString> Create(const char* str);
		static UniquePtr<StaticString> Create(const char* str, size_t size);
		static UniquePtr<StaticString> Create(size_t size);
		static UniquePtr<StaticString> Create(StringView str);

	private:
		StaticString(const char* str);
		StaticString(const char* str, size_t size);
		StaticString(size_t size);

		char* m_Data;
		size_t m_Size;
	};
}
