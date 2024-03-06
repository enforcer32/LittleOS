#pragma once

#include <stdint.h>

namespace Kernel
{
	namespace Memory
	{
		class Heap
		{
		public:
			int32_t Init(uint32_t* startAddr, uint32_t* endAddr, uint32_t blockSizeBytes, uint32_t heapSizeBytes);
			uint32_t* Malloc(uint32_t size);
			void Free(uint32_t* ptr);

		private:
			bool VerifyBlockTable();
			uint32_t* MallocBlocks(uint32_t count);
			int32_t GetStartBlock(uint32_t totalBlocks);
			void SetBlocksAllocated(uint32_t startBlock, uint32_t totalBlocks);
			void SetBlocksFree(uint32_t startBlock);
			uint32_t* BlockToAddress(uint32_t block);
			uint32_t AddressToBlock(uint32_t* address);

		private:
			uint32_t m_HeapSizeBytes;
			uint32_t* m_StartAddress;
			uint32_t* m_EndAddress;
			uint8_t* m_Blocks; /* HEAP_BLOCK_START, HEAP_BLOCK_CONTIGUOUS, 0, 0, HEAP_BLOCK_TYPE_4BIT */
			uint32_t m_BlockCount;
			uint32_t m_BlockSizeBytes;
		};
	}
}
