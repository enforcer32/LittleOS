#include <Kernel/Lib/KPanic.h>
#include <Kernel/Lib/KPrintf.h>

namespace Kernel
{
	void KPanic(const char* str)
	{
		KPrintf(str);
		while (true) {}
	}
}
