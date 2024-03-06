#pragma once

#include <stdint.h>

namespace Kernel
{
	int32_t Kitoa(int32_t num, char* buf, uint32_t len, uint32_t base);
	int32_t Kctoi(char ch);
}
