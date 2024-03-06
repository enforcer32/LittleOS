#include <Kernel/Memory/KMalloc.h>
#include <Kernel/Memory/Heap.h>
#include <Kernel/Lib/KPrintf.h>
#include <Kernel/Lib/KPanic.h>
#include <Al/CString.h>

void* operator new(size_t size)
{
	return Kernel::Memory::KMalloc(size);
}

void* operator new[](size_t size)
{
	return Kernel::Memory::KMalloc(size);
}

void operator delete(void* ptr)
{
	Kernel::Memory::KFree(ptr);
}

void operator delete[](void* ptr)
{
	Kernel::Memory::KFree(ptr);
}

void operator delete(void* ptr, size_t size)
{
	Kernel::Memory::KFree(ptr);
}

void operator delete[](void* ptr, size_t size)
{
	Kernel::Memory::KFree(ptr);
}

namespace Kernel
{
	namespace Memory
	{
		static Heap sHeap;

		int32_t KMallocInit()
		{
			KPrintf("Initializing KMem...\n");
			uint32_t heapStartAddress = 0x01000000;
			uint32_t heapSizeBytes = 104857600; /* 100MB */
			uint32_t heapBlockSize = 4096;
			uint32_t heapEndAddress = (heapStartAddress + heapSizeBytes);
			uint32_t heapBlockTableAddress = 0x00007E00;

			if (sHeap.Init((uint32_t*)heapStartAddress, (uint32_t*)heapEndAddress, heapBlockSize, heapSizeBytes, (uint32_t*)heapBlockTableAddress) != 0)
				KPanic("Failed to Initialize Heap\n");

			return 0;
		}

		void* KMalloc(size_t size)
		{
			return sHeap.Malloc(size);
		}

		void* KZAlloc(size_t size)
		{
			void* ptr = KMalloc(size);
			if (!ptr) return 0;
			Al::Memset(ptr, 0x0, size);
			return ptr;
		}

		void KFree(void* ptr)
		{
			sHeap.Free((uint32_t*)ptr);
		}
	}
}
