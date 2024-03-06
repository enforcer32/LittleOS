#pragma once

#include <stdint.h>

namespace Kernel
{
	namespace CPU
	{
		namespace Port
		{
			extern "C"
			{
				uint8_t insb(uint16_t port);
				void outb(uint16_t port, uint8_t data);
				uint16_t insw(uint16_t port);
				void outw(uint16_t port, uint16_t data);
				uint32_t insd(uint32_t port);
				uint32_t outd(uint32_t port, uint32_t data);
			}
		}
	}
}
