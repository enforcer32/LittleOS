#include <Kernel/Drivers/VGA.h>
#include <Kernel/Lib/KPrintf.h>
#include <Kernel/Lib/KPanic.h>
#include <Kernel/CPU/GDT.h>

namespace Kernel
{
	void InitKernel()
	{
		if (Drivers::VGA::Init() != 0)
			KPanic("Failed To Initialize VGA Driver\n");

		if(CPU::GDT::Init() != 0)
			KPanic("Failed To Initialize GDT\n");

		KPrintf("Kernal Initialized\n");
	}
}

extern "C" void KMain()
{
	Kernel::InitKernel();
}
