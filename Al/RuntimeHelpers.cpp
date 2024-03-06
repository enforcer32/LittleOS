#include <Al/RuntimeHelpers.h>
#include <Kernel/Lib/KPanic.h>

namespace Al
{
}

extern "C" void __cxa_pure_virtual()
{
	Kernel::KPanic("TEMP: __cxa_pure_virtual\n");
}
