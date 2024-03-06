#include <Kernel/CPU/Interrupts/IDT.h>
#include <Kernel/CPU/Interrupts/ISR.h>
#include <Kernel/CPU/Interrupts/IRQ.h>
#include <Kernel/Lib/KPrintf.h>

namespace Kernel
{
	namespace CPU
	{
		IDTRDesc IDT::s_IDTR;
		IDTDesc IDT::s_IDT[IDT_TOTAL_INTERRUPTS]{};

		extern "C" void IDTLoad(IDTRDesc * idtr);

		int32_t IDT::Init()
		{
			KPrintf("Initializing IDT...\n");

			s_IDTR.Limit = sizeof(s_IDT) - 1;
			s_IDTR.Base = (uint32_t)s_IDT;

			// Install ISRs
			ISRInstall();
			IRQInstall();

			IDTLoad(&s_IDTR);
			return 0;
		}

		void IDT::SetGate(uint32_t interrupt, uint32_t base, uint16_t selector, uint8_t flags)
		{
			s_IDT[interrupt].OffsetLow = (base & 0xFFFF);
			s_IDT[interrupt].Selector = selector;
			s_IDT[interrupt]._Reserved = 0;
			s_IDT[interrupt].Flags = flags;
			s_IDT[interrupt].OffsetHigh = (base >> 16) & 0xFFFF;
		}
	}
}
