#pragma once

#include <stdint.h>

namespace Kernel
{
	namespace CPU
	{
		extern "C" void EnableInterrupts();
		extern "C" void DisableInterrupts();

		struct InterruptFrame
		{
			uint32_t DS; // Data Segment Selector
			uint32_t EDI, ESI, EBP, IESP, EBX, EDX, ECX, EAX; // Pushed by pusha
			uint32_t IntNum, ErrCode; // Interrupt number and error code
			uint32_t EIP, CS, EFLAGS, ESP, SS; // Pushed by processor
		};
	}
}
