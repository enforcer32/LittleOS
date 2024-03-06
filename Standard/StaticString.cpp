#include <Standard/StaticString.h>
#include <Standard/CString.h>

namespace Std
{
	StaticString::~StaticString()
	{
		if (!m_Data)
			return;
		delete m_Data;
		m_Size = 0;
	}

	StaticString::StaticString(const char* str)
	{
		m_Data = Strdup(str);
		m_Size = Strlen(m_Data);
	}
	
	StaticString::StaticString(const char* str, size_t size)
	{
		m_Data = Strndup(str, size);
		m_Size = size;
	}
	
	StaticString::StaticString(size_t size)
	{
		m_Data = nullptr;
		m_Size = size;
	}

	bool StaticString::operator==(const char* rhs) const
	{
		return !Std::Strncmp(m_Data, rhs, m_Size);
	}

	bool StaticString::operator==(const StaticString& rhs) const
	{
		return !Std::Strncmp(m_Data, rhs.m_Data, m_Size);
	}

	bool StaticString::operator!=(const char* rhs) const
	{
		return Std::Strncmp(m_Data, rhs, m_Size);
	}

	bool StaticString::operator!=(const StaticString& rhs) const
	{
		return Std::Strncmp(m_Data, rhs.m_Data, m_Size);
	}

	UniquePtr<StaticString> StaticString::Clone() const
	{
		return UniquePtr<StaticString>(new StaticString(m_Data));
	}

	const char* StaticString::Data() const
	{
		return m_Data;
	}
	
	size_t StaticString::Size() const
	{
		return m_Size;
	}

	bool StaticString::Empty() const
	{
		return m_Size == 0;
	}

	UniquePtr<StaticString> StaticString::Create(const char* str)
	{
		return UniquePtr<StaticString>(new StaticString(str));
	}

	UniquePtr<StaticString> StaticString::Create(const char* str, size_t size)
	{
		return UniquePtr<StaticString>(new StaticString(str, size));
	}
	
	UniquePtr<StaticString> StaticString::Create(size_t size)
	{
		return UniquePtr<StaticString>(new StaticString(size));
	}
}
