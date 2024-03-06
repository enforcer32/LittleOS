#pragma once

#include <Al/Types.h>

namespace Al
{
	class StringView
	{
	public:
		StringView();
		~StringView();
		StringView(const char* str);
		StringView(const char* str, size_t size);
		StringView(const StringView& rhs);
		StringView(StringView&& rhs) = delete;

		bool operator==(const StringView& rhs) const;
		bool operator!=(const StringView& rhs) const;

		const char* Data() const;
		size_t Size() const;
		bool Empty() const;
		StringView Substr(size_t pos, size_t len) const;
		size_t Find(char ch, size_t pos = 0) const;

	private:
		const char* m_Data;
		size_t m_Size;
	};
}
