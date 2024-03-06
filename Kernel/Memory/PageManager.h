#pragma once

#include <Standard/Types.h>

// PAGE ENTRY FLAGS
#define PAGING_IS_PRESENT 0x1
#define PAGING_IS_WRITEABLE 0x2
#define PAGING_ACCESS_FROM_ALL 0x4
#define PAGING_WRITE_THROUGH 0x8
#define PAGING_CACHE_DISABLED 0x10

namespace Kernel
{
	namespace Memory
	{
		using Page = uint32_t;

		class PageManager
		{
		public:
			static void Enable();
			static void Disable();
			static void Switch(Page* page);
			static Page* Allocate(uint8_t flags);
			static void Free(Page* page);
			static int32_t Map(Page* page, void* virt, void* phys, uint8_t flags);

		private:
			static Page* s_CurrentPage;
		};
	}
}
