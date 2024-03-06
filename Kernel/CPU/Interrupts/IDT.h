#pragma once

#include <Al/Types.h>

#define IDT_TOTAL_INTERRUPTS 256

#define KERNEL_CODE_SELECTOR 0x08
#define KERNEL_DATA_SELECTOR 0x10
#define USER_CODE_SELECTOR 0x18
#define USER_DATA_SELECTOR 0x20
#define TSS_SELECTOR 0x28

namespace Kernel
{
	namespace CPU
	{
		struct IDTDesc
		{
			uint16_t OffsetLow;
			uint16_t Selector;
			uint8_t _Reserved;
			uint8_t Flags;
			uint16_t OffsetHigh;
		} __attribute__((packed));

		struct IDTRDesc
		{
			uint16_t Limit;
			uint32_t Base;
		} __attribute__((packed));

		class IDT
		{
		public:
			static int32_t Init();
			static void SetGate(uint32_t interrupt, uint32_t base, uint16_t selector, uint8_t flags);

		private:
			static IDTRDesc s_IDTR;
			static IDTDesc s_IDT[IDT_TOTAL_INTERRUPTS];
		};
	}
}
