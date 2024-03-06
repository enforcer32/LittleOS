#include <Kernel/CPU/Interrupts/IRQ.h>
#include <Kernel/CPU/Interrupts/IDT.h>
#include <Kernel/CPU/Interrupts/PIC.h>

namespace Kernel
{
	namespace CPU
	{
		/* IRQ FUNCTIONS */
		extern "C" void IRQ0();
		extern "C" void IRQ1();
		extern "C" void IRQ2();
		extern "C" void IRQ3();
		extern "C" void IRQ4();
		extern "C" void IRQ5();
		extern "C" void IRQ6();
		extern "C" void IRQ7();
		extern "C" void IRQ8();
		extern "C" void IRQ9();
		extern "C" void IRQ10();
		extern "C" void IRQ11();
		extern "C" void IRQ12();
		extern "C" void IRQ13();
		extern "C" void IRQ14();
		extern "C" void IRQ15();

		/* IRQ HANDLERS */
		void (*IRQRoutines[16])(InterruptFrame* frame) {};

		void IRQInstall()
		{
			IDT::SetGate(32, (uint32_t)IRQ0, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(33, (uint32_t)IRQ1, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(34, (uint32_t)IRQ2, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(35, (uint32_t)IRQ3, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(36, (uint32_t)IRQ4, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(37, (uint32_t)IRQ5, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(38, (uint32_t)IRQ6, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(39, (uint32_t)IRQ7, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(40, (uint32_t)IRQ8, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(41, (uint32_t)IRQ9, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(42, (uint32_t)IRQ10, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(43, (uint32_t)IRQ11, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(44, (uint32_t)IRQ12, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(45, (uint32_t)IRQ13, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(46, (uint32_t)IRQ14, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(47, (uint32_t)IRQ15, KERNEL_CODE_SELECTOR, 0x8E);
		}

		void IRQInstallHandler(uint32_t irq, void (*handler)(InterruptFrame* frame))
		{
			IRQRoutines[irq] = handler;
		}

		void IRQUninstallHandler(uint32_t irq)
		{
			IRQRoutines[irq] = 0;
		}

		extern "C" void IRQHandler(InterruptFrame * frame)
		{
			void (*handler)(InterruptFrame * frame);

			/* Handler CUSTOM IRQs */
			handler = IRQRoutines[frame->IntNum - 32];
			if (handler && frame->IntNum - 32 != 0)
				handler(frame);

			PICSendEOI(frame->IntNum - 32);

			/* TIMER IRQ- PERFORM TASK SWITCH AFTER SENDING EOI TO INTERRUPT CONTROLLER */
			if (handler && frame->IntNum - 32 == 0)
				handler(frame);
		}
	}
}
