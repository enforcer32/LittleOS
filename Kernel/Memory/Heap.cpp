#include <Kernel/Memory/Heap.h>
#include <Kernel/Lib/Errno.h>
#include <Kernel/Lib/KString.h>
#include <Kernel/Lib/KPrintf.h>

/* HEAP_BLOCK_START, HEAP_BLOCK_CONTIGUOUS, 0, 0, HEAP_BLOCK_TYPE_4BIT */
#define HEAP_BLOCK_START 0x80
#define HEAP_BLOCK_CONTIGUOUS 0x40
#define HEAP_BLOCK_ALLOCATED 0x01
#define HEAP_BLOCK_FREE 0x00
#define HEAP_BLOCK_TYPE 0x0f

#define HEAP_ALIGN_CHECK(addr, blocksize) (((uint32_t)addr % blocksize) == 0)
#define HEAP_ALIGN_SIZE(size, blocksize) (size % blocksize ? (size - (size % blocksize)) + blocksize : size)
#define HEAP_BLOCK_IS_ALLOCATED(block) ((block & HEAP_BLOCK_TYPE) == HEAP_BLOCK_ALLOCATED)
#define HEAP_BLOCK_IS_FREE(block) ((block & HEAP_BLOCK_TYPE) == HEAP_BLOCK_FREE)

namespace Kernel
{
	namespace Memory
	{
		int32_t Heap::Init(uint32_t* startAddr, uint32_t* endAddr, uint32_t blockSizeBytes, uint32_t heapSizeBytes, uint32_t* blockTableAddress)
		{
			KPrintf("Initializing Heap...\n");

			if (!HEAP_ALIGN_CHECK(startAddr, blockSizeBytes) || !HEAP_ALIGN_CHECK(endAddr, blockSizeBytes))
				return -EINVAL;

			m_HeapSizeBytes = heapSizeBytes;
			m_EndAddress = endAddr;
			m_BlockSizeBytes = blockSizeBytes;
			m_BlockCount = m_HeapSizeBytes / m_BlockSizeBytes;
			m_Blocks = (uint8_t*)blockTableAddress;
			m_StartAddress = startAddr; // Reserved space for block table

			if (!VerifyBlockTable())
				return -EINVAL;

			KMemset(m_Blocks, 0x0, m_BlockCount);
			return 0;
		}

		uint32_t* Heap::Malloc(uint32_t size)
		{
			uint32_t alignedSize = HEAP_ALIGN_SIZE(size, m_BlockSizeBytes);
			uint32_t totalBlocks = alignedSize / m_BlockSizeBytes;
			return MallocBlocks(totalBlocks);
		}

		void Heap::Free(uint32_t* ptr)
		{
			SetBlocksFree(AddressToBlock(ptr));
		}

		bool Heap::VerifyBlockTable()
		{
			uint32_t tableSize = ((uint8_t*)m_EndAddress - (uint8_t*)m_StartAddress);
			uint32_t totalBlock = tableSize / m_BlockSizeBytes;
			return (m_BlockCount == totalBlock);
		}

		uint32_t* Heap::MallocBlocks(uint32_t count)
		{
			uint32_t* addr = 0;

			int32_t startBlock = GetStartBlock(count);
			if (startBlock < 0)
				return addr;

			SetBlocksAllocated(startBlock, count);

			addr = BlockToAddress(startBlock);
			return addr;
		}

		int32_t Heap::GetStartBlock(uint32_t totalBlocks)
		{
			int32_t startBlock = -1;
			uint32_t currentBlock = 0;

			for (uint32_t i = 0; i < m_BlockCount; i++)
			{
				if (!HEAP_BLOCK_IS_FREE(m_Blocks[i]))
				{
					startBlock = -1;
					currentBlock = 0;
					continue;
				}

				if (startBlock == -1)
					startBlock = i;

				currentBlock++;
				if (currentBlock == totalBlocks)
					break;
			}

			if (startBlock == -1)
				return -ENOMEM;

			return startBlock;
		}

		void Heap::SetBlocksAllocated(uint32_t startBlock, uint32_t totalBlocks)
		{
			uint8_t block = HEAP_BLOCK_START | HEAP_BLOCK_ALLOCATED;
			if (totalBlocks > 1)
				block |= HEAP_BLOCK_CONTIGUOUS;

			uint32_t endBlock = (startBlock + totalBlocks) - 1;
			for (uint32_t i = startBlock; i <= endBlock; i++)
			{
				m_Blocks[i] = block;
				block = HEAP_BLOCK_ALLOCATED;
				if (i != endBlock - i)
					block |= HEAP_BLOCK_CONTIGUOUS;
			}
		}

		void Heap::SetBlocksFree(uint32_t startBlock)
		{
			for (uint32_t i = startBlock; i < m_BlockCount; i++)
			{
				uint8_t block = m_Blocks[i];
				m_Blocks[i] = HEAP_BLOCK_FREE;
				if (!(block & HEAP_BLOCK_CONTIGUOUS))
					break;
			}
		}

		uint32_t* Heap::BlockToAddress(uint32_t block)
		{
			return (m_StartAddress + (block * m_BlockSizeBytes));
		}

		uint32_t Heap::AddressToBlock(uint32_t* address)
		{
			return (uint32_t)(address - m_StartAddress) / m_BlockSizeBytes;
		}
	}
}
