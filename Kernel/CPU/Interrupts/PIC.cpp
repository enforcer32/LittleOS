#include <Kernel/CPU/Interrupts/PIC.h>
#include <Kernel/CPU/Port.h>
#include <Kernel/Lib/KPrintf.h>

#define PIC1 0x20 /* IO base address for master PIC */
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)

#define PIC2 0xA0 /* IO base address for slave PIC */
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)

#define PIC_EOI 0x20 /* End-of-interrupt command code */

#define ICW1_ICW4 0x01 /* ICW4 (not) needed */
#define ICW1_SINGLE 0x02 /* Single (cascade) mode */
#define ICW1_INTERVAL4 0x04 /* Call address interval 4 (8) */
#define ICW1_LEVEL 0x08 /* Level triggered (edge) mode */
#define ICW1_INIT 0x10 /* Initialization - required! */

#define ICW4_8086 0x01 /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO 0x02 /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE 0x08 /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C /* Buffered mode/master */
#define ICW4_SFNM 0x10 /* Special fully nested (not) */

/* MAP IRQ 0-15 TO IDT ENTRIES 32-47 */
#define PIC1_OFFSET 0x20
#define PIC2_OFFSET 0x28

namespace Kernel
{
	namespace CPU
	{
		/* IRQs 0 to 7 are mapped to reserved IDT entries 8 to 15 (PROBLEM). REMAP IRQ 0-15 TO IDT ENTRIES 32-47 */
		int32_t PICRemap()
		{
			KPrintf("Remapping PIC...\n");

			// Save Masks
			uint8_t a1, a2;
			a1 = Port::insb(PIC1_DATA);
			a2 = Port::insb(PIC2_DATA);

			// Setup Master PIC
			Port::outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); // starts the initialization sequence (in cascade mode)
			Port::outb(PIC1_DATA, PIC1_OFFSET); // ICW2: Master PIC Vector Offset
			Port::outb(PIC1_DATA, 4); // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100) (TELL MASTER HE HAS A SLAVE)
			Port::outb(PIC1_DATA, ICW4_8086); // x86 MODE

			// Setup Slave PIC
			Port::outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
			Port::outb(PIC2_DATA, PIC2_OFFSET);
			Port::outb(PIC2_DATA, 2); // ICW3: tell Slave PIC its cascade identity (0000 0010)
			Port::outb(PIC2_DATA, ICW4_8086);

			// Restore Saved Masks
			Port::outb(PIC1_DATA, a1);
			Port::outb(PIC2_DATA, a2);
			return 0;
		}

		void PICDisable()
		{
			Port::outb(PIC1_DATA, 0xFF);
		}

		// Acknowledge Interrupt
		void PICSendEOI(uint32_t irq)
		{
			/* INT >= 8 (meaning IRQ8 - 15), then we need to send an EOI to the slave controller */
			if (irq >= 8)
				Port::outb(PIC2_COMMAND, PIC_EOI);
			/* In either case, send EOI to Master Interrupt Controller */
			Port::outb(PIC1_COMMAND, PIC_EOI);
		}
	}
}
