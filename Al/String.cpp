#include <Al/String.h>
#include <Al/CString.h>

namespace Al
{
	String::String()
		: m_Data(nullptr), m_Size(0)
	{
	}

	String::~String()
	{
		if (m_Data)
			delete[] m_Data;
	}

	String::String(const char* str)
	{
		m_Data = Strdup(str);
		m_Size = Strlen(str);
	}

	String::String(const char* str, size_t size)
	{
		m_Data = Strndup(str, size);
		m_Size = size;
	}

	String::String(size_t size, char ch)
	{
		m_Data = new char[size];
		m_Size = size;
		Memset(m_Data, ch, size);
	}

	String::String(const String& rhs)
	{
		m_Data = Strdup(rhs.m_Data);
		m_Size = rhs.m_Size;
	}

	String::String(String&& rhs)
	{
		m_Data = rhs.m_Data;
		m_Size = rhs.m_Size;
		rhs.m_Data = nullptr;
	}

	String& String::operator=(const String& rhs)
	{
		m_Data = Strdup(rhs.m_Data);
		m_Size = rhs.m_Size;
		return *this;
	}

	String& String::operator=(String&& rhs)
	{
		m_Data = rhs.m_Data;
		m_Size = rhs.m_Size;
		rhs.m_Data = nullptr;
		return *this;
	}

	bool String::operator==(const String& rhs) const
	{
		return !Al::Strncmp(m_Data, rhs.m_Data, m_Size);
	}

	bool String::operator!=(const String& rhs) const
	{
		return Al::Strncmp(m_Data, rhs.m_Data, m_Size);
	}

	String String::operator+(const String& rhs) const
	{
		size_t len = m_Size + rhs.m_Size + 1;
		char* buf = new char[len];
		Memcpy(buf, m_Data, m_Size);
		Memcpy(buf + m_Size, rhs.m_Data, rhs.m_Size);
		buf[len - 1] = 0;
		return buf;
	}

	String String::operator+(char rhs) const
	{
		size_t len = m_Size + 2;
		char* buf = new char[len];
		Memcpy(buf, m_Data, m_Size);
		buf[len - 2] = rhs;
		buf[len - 1] = 0;
		return buf;
	}

	String& String::operator+=(const String& rhs)
	{
		*this = *this + rhs;
		return *this;
	}

	String& String::operator+=(char rhs)
	{
		*this = *this + rhs;
		return *this;
	}

	const char* String::Data() const
	{
		return m_Data;
	}

	size_t String::Size() const
	{
		return m_Size;
	}

	bool String::Empty() const
	{
		return m_Size == 0;
	}

	String String::Substr(size_t pos, size_t len) const
	{
		char* buf = new char[len];
		Memcpy(buf, m_Data + pos, len);
		buf[len] = 0;
		return buf;
	}
}
