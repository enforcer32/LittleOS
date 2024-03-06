#pragma once

#include <Al/Types.h>

namespace Al
{
	size_t Strlen(const char* str);
	size_t Strnlen(const char* str, size_t maxlen);
	void Strrev(char* str, size_t num);

	char* Strchr(const char* str, char ch);
	char* Strrchr(const char* str, char ch);
	size_t Strspn(const char* str1, const char* str2);
	size_t Strcspn(const char* str1, const char* str2);
	char* Strtok(char* str, const char* delim);
	char* Strtok_r(char* str, const char* delim, char** saveptr);
	char* Strncpy(char* str1, const char* str2, size_t num);
	int32_t Strncmp(const char* str1, const char* str2, size_t num);
	char* Strdup(const char* str);
	char* Strndup(const char* str, size_t num);

	void* Memset(void* dst, int32_t ch, size_t num);
	void* Memsetw(void* dst, uint16_t ch, size_t num);
	void* Memcpy(void* dst, const void* src, size_t num);
	int32_t Memcmp(const void* src1, const void* src2, size_t num);
}
