#pragma once

#include <Al/Types.h>

namespace Al
{
	namespace TypeConverter
	{
		int32_t IntToStr(int32_t num, char* buf, uint32_t len, uint32_t base);
		int32_t CharToInt(char ch);
	}
}
