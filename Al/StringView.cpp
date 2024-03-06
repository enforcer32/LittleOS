#include <Al/StringView.h>
#include <Al/CString.h>
#include <Al/String.h>

namespace Al
{
	StringView::StringView()
		: m_Data(nullptr), m_Size(0)
	{
	}

	StringView::~StringView()
	{
	}

	StringView::StringView(const char* str)
		: m_Data(str), m_Size(Strlen(str))
	{}

	StringView::StringView(const char* str, size_t size)
		: m_Data(str), m_Size(size)
	{
	}

	StringView::StringView(const StringView& rhs)
	{
		m_Data = rhs.m_Data;
		m_Size = rhs.m_Size;
	}

	bool StringView::operator==(const StringView& rhs) const
	{
		return !Strncmp(m_Data, rhs.m_Data, m_Size);
	}

	bool StringView::operator!=(const StringView& rhs) const
	{
		return Strncmp(m_Data, rhs.m_Data, m_Size);
	}

	const char* StringView::Data() const
	{
		return m_Data;
	}

	size_t StringView::Size() const
	{
		return m_Size;
	}

	bool StringView::Empty() const
	{
		return m_Size == 0 || !m_Data;
	}

	StringView StringView::Substr(size_t pos, size_t len) const
	{
		return StringView(m_Data + pos, len);
	}

	size_t StringView::Find(char ch, size_t pos) const
	{
		for (size_t i = pos; i < m_Size; i++)
			if (m_Data[i] == ch)
				return i;
		return Al::String::NPOS;
	}
}
