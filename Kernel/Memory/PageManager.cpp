#include <Kernel/Memory/PageManager.h>
#include <Kernel/Memory/KMalloc.h>
#include <Kernel/Lib/Errno.h>
#include <Standard/CString.h>

// PAGE GENERAL DEFS
#define PAGING_TOTAL_ENTRIES 1024
#define PAGING_PAGE_SIZE 4096

// PAGE GENERAL MACROS
#define PAGING_IS_ALIGNED(addr) (((uint32_t)addr % PAGING_PAGE_SIZE) == 0)
#define PAGING_DIR_INDEX(vaddr) ((uint32_t)vaddr / (PAGING_TOTAL_ENTRIES * PAGING_PAGE_SIZE))
#define PAGING_TBL_INDEX(vaddr) ((uint32_t)vaddr % (PAGING_TOTAL_ENTRIES * PAGING_PAGE_SIZE) / PAGING_PAGE_SIZE)
#define PAGING_ALIGN_LOWER(addr) ((addr - (uint32_t)addr % PAGING_PAGE_SIZE))

namespace Kernel
{
	namespace Memory
	{
		extern "C" void EnablePaging();
		extern "C" void DisablePaging();
		extern "C" void PagingLoadDirectory(uint32_t* page);

		Page* PageManager::s_CurrentPage = nullptr;

		void PageManager::Enable()
		{
			EnablePaging();
		}

		void PageManager::Disable()
		{
			DisablePaging();
		}

		void PageManager::Switch(Page* page)
		{
			s_CurrentPage = page;
			PagingLoadDirectory(page);
		}

		Page* PageManager::Allocate(uint8_t flags)
		{
			Page* page = new Page[PAGING_TOTAL_ENTRIES];
			Standard::Memset(page, 0, sizeof(Page) * PAGING_TOTAL_ENTRIES);
			
			uint32_t offset = 0;
			for (uint32_t i = 0; i < PAGING_TOTAL_ENTRIES; i++)
			{
				uint32_t* table = new uint32_t[PAGING_TOTAL_ENTRIES];
				Standard::Memset(table, 0, sizeof(uint32_t) * PAGING_TOTAL_ENTRIES);

				for (uint32_t j = 0; j < PAGING_TOTAL_ENTRIES; j++)
					table[j] = (offset + (j * PAGING_PAGE_SIZE)) | flags;

				offset += (PAGING_TOTAL_ENTRIES * PAGING_PAGE_SIZE);
				page[i] = (uint32_t)table | flags | PAGING_IS_WRITEABLE;
			}

			return page;
		}

		void PageManager::Free(Page* page)
		{
			for (uint32_t i = 0; i < PAGING_TOTAL_ENTRIES; i++)
			{
				uint32_t dir = page[i];
				uint32_t* table = (uint32_t*)(dir & 0xfffff000);
				delete table;
			}
			delete page;
		}

		int32_t PageManager::Map(Page* page, void* virt, void* phys, uint8_t flags)
		{
			if (!PAGING_IS_ALIGNED(virt))
				return -EINVAL;
			
			uint32_t dir_index = PAGING_DIR_INDEX(virt);
			uint32_t tbl_index = PAGING_TBL_INDEX(virt);
			uint32_t dir = page[dir_index];
			uint32_t* table = (uint32_t*)(dir & 0xfffff000);
			table[tbl_index] = (uint32_t)phys | flags;
			return 0;
		}
	}
}
