#pragma once

#include <stdint.h>
#include <stddef.h>

void* operator new(size_t size);
void* operator new[](size_t size);
void operator delete(void* ptr);
void operator delete[](void* ptr);
void operator delete(void* ptr, size_t size);
void operator delete[](void* ptr, size_t size);

namespace Kernel
{
	namespace Memory
	{
		int32_t KMallocInit();
		void* KMalloc(size_t size);
		void* KZAlloc(size_t size);
		void KFree(void* ptr);
	}
}
