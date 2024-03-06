#pragma once

#include <Standard/Types.h>

namespace Kernel
{
	namespace CPU
	{
		int32_t PICRemap();
		void PICDisable();
		void PICSendEOI(uint32_t irq);
	}
}
