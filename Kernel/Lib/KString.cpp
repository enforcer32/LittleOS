#include <Kernel/Lib/KString.h>

namespace Kernel
{
	size_t KStrlen(const char* str)
	{
		size_t len = 0;
		while (str[len]) len++;
		return len;
	}

	void KStrrev(char* str, uint32_t num)
	{
		for (uint32_t i = 0; i < num / 2; i++) {
			char tmp = str[i];
			str[i] = str[num - 1 - i];
			str[num - 1 - i] = tmp;
		}
	}

	void* KMemset(void* str, int32_t ch, size_t num)
	{
		uint8_t* p1 = (uint8_t*)str;
		while (num--) *p1++ = ch;
		return str;
	}

	void* KMemsetw(void* str, uint16_t ch, size_t num)
	{
		uint16_t* p1 = (uint16_t*)str;
		while (num--) *p1++ = ch;
		return str;
	}

	void* KMemcpy(void* dst, const void* str, size_t num)
	{
		uint8_t* p1 = (uint8_t*)dst;
		uint8_t* p2 = (uint8_t*)str;
		while (num--) *p1++ = *p2++;
		return dst;
	}
}
