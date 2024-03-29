#pragma once

#include <Al/Types.h>
#include <stdarg.h>

namespace Kernel
{
	void KPrint(const char* str);
	void KPutChar(char ch);
	int32_t KPrintf(const char* fmt, ...);
}
