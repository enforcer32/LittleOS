#include <Al/CString.h>

namespace Al
{
	size_t Strlen(const char* str)
	{
		size_t len = 0;
		while (str[len]) len++;
		return len;
	}

	size_t Strnlen(const char* str, size_t maxlen)
	{
		size_t len = 0;
		while (str[len] && len < maxlen)
			len++;
		return len;
	}

	void Strrev(char* str, size_t num)
	{
		for (size_t i = 0; i < num / 2; i++) {
			char tmp = str[i];
			str[i] = str[num - 1 - i];
			str[num - 1 - i] = tmp;
		}
	}

	char* Strchr(const char* str, char ch)
	{
		do {
			if (*str == ch)
				return (char*)str;
		} while (*str++);

		return 0;
	}

	char* Strrchr(const char* str, char ch)
	{
		char* f = NULL, * t = NULL;
		while ((t = Strchr(str, ch)) != NULL) {
			f = t;
			str = t + 1;
		}
		return f;
	}

	size_t Strspn(const char* str1, const char* str2)
	{
		size_t len = 0;

		if (!str1 || !str2)
			return 0;

		while (*str1 && Strchr(str2, *str1++))
			len++;

		return len;
	}

	size_t Strcspn(const char* str1, const char* str2)
	{
		size_t len = 0;

		if (!str1 || !str2)
			return 0;

		while (*str1) {
			if (Strchr(str2, *str1++))
				return len;

			len++;
		}

		return len;
	}

	char* Strtok(char* str, const char* delim)
	{
		static char* saveptr;
		return Strtok_r(str, delim, &saveptr);
	}

	char* Strtok_r(char* str, const char* delim, char** saveptr)
	{
		char* end;
		if (!str)
			str = *saveptr;
		if (*str == '\0') {
			*saveptr = str;
			return 0;
		}

		str += Strspn(str, delim);
		if (*str == '\0') {
			*saveptr = str;
			return 0;
		}

		end = str + Strcspn(str, delim);
		if (*end == '\0') {
			*saveptr = end;
			return str;
		}

		*end = '\0';
		*saveptr = end + 1;
		return str;
	}

	char* Strncpy(char* str1, const char* str2, size_t num)
	{
		size_t i = 0;
		for (i = 0; i < num && str2[i] != '\0'; i++)
			str1[i] = str2[i];
		for (; i < num; i++)
			str1[i] = '\0';
		return str1;
	}

	int32_t Strncmp(const char* str1, const char* str2, size_t num)
	{
		unsigned char c1, c2;

		while (num-- > 0) {
			c1 = (unsigned char)*str1++;
			c2 = (unsigned char)*str2++;

			if (c1 != c2)
				return c1 - c2;

			if (c1 == '\0')
				return 0;
		}

		return 0;
	}

	char* Strdup(const char* str)
	{
		size_t len = Strlen(str) + 1;
		char* dup = new char[len];
		if (dup)
			Strncpy(dup, str, len);
		dup[len-1] = '\0';
		return dup;
	}

	char* Strndup(const char* str, size_t num)
	{
		char* dup = new char[num + 1];
		if (dup)
			Strncpy(dup, str, num);
		dup[num] = '\0';
		return dup;
	}

	void* Memset(void* dst, int32_t ch, size_t num)
	{
		uint8_t* p1 = (uint8_t*)dst;
		while (num--) *p1++ = ch;
		return dst;
	}

	void* Memsetw(void* dst, uint16_t ch, size_t num)
	{
		uint16_t* p1 = (uint16_t*)dst;
		while (num--) *p1++ = ch;
		return dst;
	}

	void* Memcpy(void* dst, const void* src, size_t num)
	{
		uint8_t* p1 = (uint8_t*)dst;
		const uint8_t* p2 = (uint8_t*)src;
		while (num--) *p1++ = *p2++;
		return dst;
	}

	int32_t Memcmp(const void* src1, const void* src2, size_t num)
	{
		const uint8_t* p1 = (uint8_t*)src1;
		const uint8_t* p2 = (uint8_t*)src2;
		for (size_t i = 0; i < num; i++, p1++, p2++) {
			if (*p1 < *p2)
				return -1;
			else if (*p1 > *p2)
				return 1;
		}
		return 0;
	}
}
