#include <Kernel/Memory/KMem.h>
#include <Kernel/Memory/Heap.h>
#include <Kernel/Lib/KString.h>
#include <Kernel/Lib/KPrintf.h>
#include <Kernel/Lib/KPanic.h>

namespace Kernel
{
	namespace Memory
	{
		static Heap sHeap;

		int32_t KMemInit()
		{
			KPrintf("Initializing KMem...\n");
			uint32_t HeapStartAddress = 0x01000000;
			uint32_t heapSizeBytes = 104857600; /* 100MB */
			uint32_t heapBlockSize = 4096;
			uint32_t heapEndAddress = (HeapStartAddress + heapSizeBytes);

			if (sHeap.Init((uint32_t*)HeapStartAddress, (uint32_t*)heapEndAddress, heapBlockSize, heapSizeBytes) != 0)
				KPanic("Failed to Initialize Heap\n");

			return 0;
		}

		uint32_t* KMalloc(uint32_t size)
		{
			return sHeap.Malloc(size);
		}

		uint32_t* KZAlloc(uint32_t size)
		{
			uint32_t* ptr = KMalloc(size);
			if (!ptr) return 0;
			KMemset(ptr, 0x0, size);
			return ptr;
		}

		void KFree(uint32_t* ptr)
		{
			sHeap.Free(ptr);
		}
	}
}
