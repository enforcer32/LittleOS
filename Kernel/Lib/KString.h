#pragma once

#include <stdint.h>
#include <stddef.h>

namespace Kernel
{
	size_t KStrlen(const char* str);
	void KStrrev(char* str, uint32_t num);

	void* KMemset(void* str, int32_t ch, size_t num);
	void* KMemsetw(void* str, uint16_t ch, size_t num);
	void* KMemcpy(void* dst, const void* str, size_t num);
}
