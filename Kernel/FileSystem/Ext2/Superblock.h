#pragma once

#include <Standard/Types.h>

#define EXT2_SIGNATURE 0xEF53
#define EXT2_SUPERBLOCK_OFFSET 1024
#define EXT2_SUPERBLOCK_SIZE 1024

namespace Kernel
{
	namespace FileSystem
	{
		enum class Ext2FileSystemState : uint16_t
		{
			Clean = 1,
			Error = 2
		};

		enum class Ext2ErrorHandlingMethod : uint16_t
		{
			Ignore = 1,
			RemountReadOnly = 2,
			KernelPanic = 3
		};

		enum class Ext2OperatingSystemID : uint32_t
		{
			Linux = 0,
			GNUHurd = 1,
			Masix = 2,
			FreeBSD = 3,
			Other = 4
		};

		enum class Ext2OptionalFeatureFlags : uint32_t
		{
			PreallocatedBlocks = 0x0001,
			AFSServerInode = 0x0002,
			FileSystemJournal = 0x0004,
			InodeExtendedAttrib = 0x0008,
			FileSystemResizable = 0x0010,
			DirHasIndex = 0x0020
		};

		enum class Ext2RequiredFeatureFlags : uint32_t
		{
			Compression = 0x0001,
			DirTypeField = 0x0002,
			FileSystemReplayJournal = 0x0004,
			FileSystemJournalDevice = 0x0008,
		};

		enum class Ext2ReadOnlyFeatureFlags : uint32_t
		{
			Sparse = 0x0001,
			FileSize64 = 0x0002,
			DirBinaryTree = 0x0004,
		};

		struct Ext2Superblock
		{
			uint32_t InodeCount;
			uint32_t BlockCount;
			uint32_t ReservedBlockCount;
			uint32_t FreeBlockCount;
			uint32_t FreeInodeCount;
			uint32_t FirstDataBlock;
			uint32_t BlockSize;
			uint32_t FragmentSize;
			uint32_t BlockGroupBlockCount;
			uint32_t BlockGroupFragmentCount;
			uint32_t BlockGroupInodeCount;
			uint32_t LastMountTime;
			uint32_t LastWriteTime;
			uint16_t MountCountSinceFSCK;
			uint16_t MountLimitBeforeFSCK;
			uint16_t Signature;
			Ext2FileSystemState FileSystemState;
			Ext2ErrorHandlingMethod ErrorHandler;
			uint16_t MinorVersion;
			uint32_t LaskFSCKTime;
			uint32_t FSCKInterval;
			Ext2OperatingSystemID OSID;
			uint32_t MajorVersion;
			uint16_t UserID;
			uint16_t GroupID;
			/* Extended Superblock Fields - MAJOR >= 1 */
			uint32_t FirstInode;
			uint16_t InodeSize;
			uint16_t BlockGroup;
			Ext2OptionalFeatureFlags OptionalFeatures;
			Ext2RequiredFeatureFlags RequiredFeatues;
			Ext2ReadOnlyFeatureFlags UnsupportedFeatures;
			uint8_t FileSystemID[16];
			uint8_t VolumeName[16];
			uint8_t PathVolumeLastMounted[64];
			uint32_t CompressionAlgorithm;
			uint8_t PreallocateBlockCountFile;
			uint8_t PreallocateBlockCountDir;
			uint8_t _Unused1[2];
			uint8_t JournalID[16];
			uint32_t JournalInode;
			uint32_t JournalDevice;
			uint32_t OrphanInodeListHead;
			uint8_t _Unused2[788];
		} __attribute__((packed));

		struct Ext2BlockGroupDescriptor
		{
			uint32_t BlockBitmap;
			uint32_t InodeBitmap;
			uint32_t InodeTable;
			uint16_t FreeBlockCount;
			uint16_t FreeInodeCount;
			uint16_t DirCount;
			uint8_t _unused[14];
		} __attribute__((packed));
	}
}
