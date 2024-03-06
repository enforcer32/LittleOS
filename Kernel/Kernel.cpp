#include <Kernel/Drivers/VGA.h>
#include <Kernel/Lib/KPrintf.h>
#include <Kernel/Lib/KPanic.h>
#include <Kernel/CPU/GDT.h>
#include <Kernel/CPU/Interrupts/IDT.h>
#include <Kernel/CPU/Interrupts/PIC.h>
#include <Kernel/CPU/Interrupts/IRQ.h>
#include <Kernel/Memory/KMalloc.h>
#include <Kernel/Memory/PageManager.h>
#include <Kernel/FileSystem/Disk.h>
#include <Standard/Utility.h>
#include <Standard/CString.h>
#include <Kernel/FileSystem/Ext2/FileSystem.h>

namespace Kernel
{
	static Memory::Page* sKernelPage = nullptr;

	void SimpleIRQ(CPU::InterruptFrame* frame)
	{
		KPrintf("Called IRQ: %d\n", frame->IntNum - 32);
	}

	void InitKernel()
	{
		if (Drivers::VGA::Init() != 0)
			KPanic("Failed To Initialize VGA Driver\n");

		if (CPU::GDT::Init() != 0)
			KPanic("Failed To Initialize GDT\n");

		if (CPU::IDT::Init() != 0)
			KPanic("Failed to Initialize IDT\n");

		if (CPU::PICRemap() != 0)
			KPanic("Failed to Remap PIC\n");

		if (Memory::KMallocInit() != 0)
			KPanic("Failed to Initialize KMalloc\n");

		sKernelPage = Memory::PageManager::Allocate(PAGING_IS_PRESENT | PAGING_IS_WRITEABLE | PAGING_ACCESS_FROM_ALL);
		if (!sKernelPage)
			KPanic("Failed to Initialize Kernel Page\n");

		Memory::PageManager::Switch(sKernelPage);
		Memory::PageManager::Enable();

		// SIMPLE IRQ
		//CPU::IRQInstallHandler(0, SimpleIRQ);

		CPU::EnableInterrupts();

		// Init FileSystem
		auto ata = Std::UniquePtr<Drivers::ATA>(new Drivers::ATA);
		ata->Init(Drivers::ATABus::Primary, Drivers::ATADrive::Slave);
		Std::UniquePtr<FileSystem::Disk> disk;
		disk.Reset(new FileSystem::Disk);
		disk->Init(Std::Move(ata));

		FileSystem::Ext2FileSystem ext2fs;
		if (ext2fs.Init(Std::Move(disk)) != 0)
			KPanic("Failed to Initialize Ext2FileSystem\n");
		
		//ext2fs.DumpSuperblock();
		//ext2fs.DumpBlockGroupDescriptorTable();

		KPrintf("Kernel Initialized\n");
		for(;;);
	}
}

extern "C" void KMain()
{
	Kernel::InitKernel();
}
