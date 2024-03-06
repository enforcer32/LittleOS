#pragma once

#include <Kernel/CPU/Interrupts/Interrupt.h>

namespace Kernel
{
	namespace CPU
	{
		void IRQInstall();
		void IRQInstallHandler(uint32_t irq, void (*handler)(InterruptFrame* frame));
		void IRQUninstallHandler(uint32_t irq);
	}
}
