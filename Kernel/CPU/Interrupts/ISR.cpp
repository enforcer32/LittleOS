#include <Kernel/CPU/Interrupts/ISR.h>
#include <Kernel/CPU/Interrupts/IDT.h>
#include <Kernel/CPU/Interrupts/Interrupt.h>
#include <Kernel/Lib/KPanic.h>

namespace Kernel
{
	namespace CPU
	{
		/* ISRs reserved for CPU exceptions */
		extern "C" void ISR0();
		extern "C" void ISR1();
		extern "C" void ISR2();
		extern "C" void ISR3();
		extern "C" void ISR4();
		extern "C" void ISR5();
		extern "C" void ISR6();
		extern "C" void ISR7();
		extern "C" void ISR8();
		extern "C" void ISR9();
		extern "C" void ISR10();
		extern "C" void ISR11();
		extern "C" void ISR12();
		extern "C" void ISR13();
		extern "C" void ISR14();
		extern "C" void ISR15();
		extern "C" void ISR16();
		extern "C" void ISR17();
		extern "C" void ISR18();
		extern "C" void ISR19();
		extern "C" void ISR20();
		extern "C" void ISR21();
		extern "C" void ISR22();
		extern "C" void ISR23();
		extern "C" void ISR24();
		extern "C" void ISR25();
		extern "C" void ISR26();
		extern "C" void ISR27();
		extern "C" void ISR28();
		extern "C" void ISR29();
		extern "C" void ISR30();
		extern "C" void ISR31();

		// 32 Exception Messages
		const char* ExceptionMessages[] = { "Division By Zero",
									   "Debug",
									   "Non Maskable Interrupt",
									   "Breakpoint",
									   "Into Detected Overflow",
									   "Out of Bounds",
									   "Invalid Opcode",
									   "No Coprocessor",
									   "Double Fault",
									   "Coprocessor Segment Overrun",
									   "Bad TSS",
									   "Segment Not Present",
									   "Stack Fault",
									   "General Protection Fault",
									   "Page Fault",
									   "Unknown Interrupt",
									   "Coprocessor Fault",
									   "Alignment Check",
									   "Machine Check",
									   "Reserved",
									   "Reserved",
									   "Reserved",
									   "Reserved",
									   "Reserved",
									   "Reserved",
									   "Reserved",
									   "Reserved",
									   "Reserved",
									   "Reserved",
									   "Reserved",
									   "Reserved",
									   "Reserved" };


		void ISRInstall()
		{
			IDT::SetGate(0, (uint32_t)ISR0, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(1, (uint32_t)ISR1, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(2, (uint32_t)ISR2, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(3, (uint32_t)ISR3, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(4, (uint32_t)ISR4, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(5, (uint32_t)ISR5, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(6, (uint32_t)ISR6, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(7, (uint32_t)ISR7, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(8, (uint32_t)ISR8, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(9, (uint32_t)ISR9, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(10, (uint32_t)ISR10, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(11, (uint32_t)ISR11, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(12, (uint32_t)ISR12, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(13, (uint32_t)ISR13, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(14, (uint32_t)ISR14, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(15, (uint32_t)ISR15, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(16, (uint32_t)ISR16, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(17, (uint32_t)ISR17, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(18, (uint32_t)ISR18, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(19, (uint32_t)ISR19, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(20, (uint32_t)ISR20, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(21, (uint32_t)ISR21, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(22, (uint32_t)ISR22, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(23, (uint32_t)ISR23, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(24, (uint32_t)ISR24, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(25, (uint32_t)ISR25, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(26, (uint32_t)ISR26, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(27, (uint32_t)ISR27, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(28, (uint32_t)ISR28, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(29, (uint32_t)ISR29, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(30, (uint32_t)ISR30, KERNEL_CODE_SELECTOR, 0x8E);
			IDT::SetGate(31, (uint32_t)ISR31, KERNEL_CODE_SELECTOR, 0x8E);
		}

		extern "C" void FaultHandler(InterruptFrame* frame)
		{
			if (frame->IntNum < 32)
				KPanic(ExceptionMessages[frame->IntNum]);
		}
	}
}
