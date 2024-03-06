#pragma once

#include <Al/Types.h>

namespace Al
{
	class String
	{
	public:
		String();
		~String();
		String(const char* str);
		String(const char* str, size_t size);
		explicit String(size_t size, char ch = 0);
		String(const String& rhs);
		String(String&& rhs);

		String& operator=(const String& rhs);
		String& operator=(String&& rhs);
		bool operator==(const String& rhs) const;
		bool operator!=(const String& rhs) const;
		String operator+(const String& rhs) const;
		String operator+(char rhs) const;
		String& operator+=(const String& rhs);
		String& operator+=(char rhs);

		const char* Data() const;
		size_t Size() const;
		bool Empty() const;
		String Substr(size_t pos, size_t len) const;
		size_t Find(char ch, size_t pos = 0) const;

	public:
		static const size_t NPOS = -1;

	private:
		char* m_Data;
		size_t m_Size;
	};
}
