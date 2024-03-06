#include <Kernel/FileSystem/Ext2/FileSystem.h>
#include <Kernel/Lib/KPrintf.h>
#include <Standard/Utility.h>
#include <Standard/CString.h>

namespace Kernel
{
	namespace FileSystem
	{
		int32_t Ext2FileSystem::Init(Std::UniquePtr<Disk> disk)
		{
			m_Disk = Std::Move(disk);
			
			if (ParseSuperblock() != 0)
			{
				KPrintf("Failed to Parse Superblock\n");
				return -1;
			}

			if (ParseBlockGroupDescriptorTable() != 0)
			{
				KPrintf("Failed to Parse ParseBlockGroupDescriptorTable\n");
				return -1;
			}

			m_RootInode.Reset(GetInode(EXT2_ROOT_INODE));
			if (!m_RootInode)
			{
				KPrintf("Failed to Get Root Inode: %d\n", EXT2_ROOT_INODE);
				return -1;
			}

			//DumpInode(*m_RootInode.Get());
			DumpInodeDirs(*m_RootInode.Get());

			return 0;
		}

		void Ext2FileSystem::Free()
		{

		}

		void* Ext2FileSystem::ReadBlock(size_t block)
		{
			uint8_t* buf = new uint8_t[m_Superblock->BlockSize];
			if (m_Disk->Read(buf, m_Superblock->BlockSize, (block * (m_Superblock->BlockSize / m_Disk->GetSectorSize()))) != (int32_t)m_Superblock->BlockSize)
			{
				KPrintf("Ext2FS Read Block Error");
				return nullptr;
			}
			return buf;
		}

		const Ext2BlockGroupDescriptor& Ext2FileSystem::GetBlockGroupDescriptor(uint32_t group)
		{
			return m_BlockGroupDescriptors[group];
		}

		Ext2Inode* Ext2FileSystem::GetInode(uint32_t inode)
		{
			if (inode == 0)
				return {};

			uint32_t bg = (inode - 1) / m_Superblock->BlockGroupInodeCount;
			auto& bgd = GetBlockGroupDescriptor(bg);

			uint32_t index = (inode - 1) % m_Superblock->BlockGroupInodeCount;
			uint32_t block = (index * m_Superblock->InodeSize) / m_Superblock->BlockSize;
			index = index % (m_Superblock->BlockSize / m_Superblock->InodeSize);
			uint32_t offset = (index * m_Superblock->InodeSize);

			uint8_t* inodetable = (uint8_t*)ReadBlock(bgd.InodeTable + block);
			Ext2Inode* inodebuf = new Ext2Inode;
			*inodebuf = *(Ext2Inode*)(inodetable + offset);
			delete inodetable;
			return inodebuf;
		}

		int32_t Ext2FileSystem::ParseSuperblock()
		{
			m_Superblock.Reset(new Ext2Superblock);
			if (m_Disk->Read(m_Superblock.Get(), EXT2_SUPERBLOCK_SIZE, (EXT2_SUPERBLOCK_OFFSET / m_Disk->GetSectorSize())) != 1024)
			{
				KPrintf("Failed to Read Ext2Superblock\n");
				return -1;
			}

			if (m_Superblock->Signature != EXT2_SIGNATURE)
			{
				KPrintf("Ext2Superblock Invalid Signature: %d\n", m_Superblock->Signature);
				return -1;
			}

			m_Superblock->BlockSize = (1024 << m_Superblock->BlockSize);
			m_Superblock->FragmentSize = (1024 << m_Superblock->BlockSize);

			KPrintf("Ext2->(0x%x) Version: (%d.%d), BlockSize: (%d)\n", m_Superblock->Signature, m_Superblock->MajorVersion, m_Superblock->MinorVersion, m_Superblock->BlockSize);
			return 0;
		}

		int32_t Ext2FileSystem::ParseBlockGroupDescriptorTable()
		{
			m_FirstBlockGroupDescriptorTable = m_Superblock->FirstDataBlock + 1;
			m_BlockGroupDescriptorCount = m_Superblock->BlockCount / m_Superblock->BlockGroupBlockCount;
			if (!m_BlockGroupDescriptorCount)
				m_BlockGroupDescriptorCount = 1;

			for (size_t i = 0; i < m_BlockGroupDescriptorCount; i++)
			{
				size_t index = (i * sizeof(Ext2BlockGroupDescriptor)) / m_Superblock->BlockSize;
				size_t offset = (i * sizeof(Ext2BlockGroupDescriptor)) % m_Superblock->BlockSize;

				uint8_t* block = (uint8_t*)ReadBlock(m_FirstBlockGroupDescriptorTable + index);
				if (!block)
					return -1;
				m_BlockGroupDescriptors.PushBack(*(Ext2BlockGroupDescriptor*)(block + offset));
				delete block;
			}

			return 0;
		}

		void Ext2FileSystem::DumpSuperblock()
		{
			KPrintf("\n-----EXT2 DUMP SUPERBLOCK START-----\n");
			KPrintf("InodeCount = %d\n", m_Superblock->InodeCount);
			KPrintf("BlockCount = %d\n", m_Superblock->BlockCount);
			KPrintf("ReservedBlockCount = %d\n", m_Superblock->ReservedBlockCount);
			KPrintf("FreeBlockCount = %d\n", m_Superblock->FreeBlockCount);
			KPrintf("FreeInodeCount = %d\n", m_Superblock->FreeInodeCount);
			KPrintf("FirstDataBlock = %d\n", m_Superblock->FirstDataBlock);
			KPrintf("BlockSize = %d\n", m_Superblock->BlockSize);
			KPrintf("FragmentSize = %d\n", m_Superblock->FragmentSize);
			KPrintf("BlockGroupBlockCount = %d\n", m_Superblock->BlockGroupBlockCount);
			KPrintf("BlockGroupFragmentCount = %d\n", m_Superblock->BlockGroupFragmentCount);
			KPrintf("BlockGroupInodeCount = %d\n", m_Superblock->BlockGroupInodeCount);
			KPrintf("LastMountTime = %d\n", m_Superblock->LastMountTime);
			KPrintf("LastWriteTime = %d\n", m_Superblock->LastWriteTime);
			KPrintf("MountCountSinceFSCK = %d\n", m_Superblock->MountCountSinceFSCK);
			KPrintf("MountLimitBeforeFSCK = %d\n", m_Superblock->MountLimitBeforeFSCK);
			KPrintf("Signature = %x\n", m_Superblock->Signature);
			KPrintf("FileSystemState = %d\n", m_Superblock->FileSystemState);
			KPrintf("ErrorHandler = %d\n", m_Superblock->ErrorHandler);
			KPrintf("MinorVersion = %d\n", m_Superblock->MinorVersion);
			KPrintf("LaskFSCKTime = %d\n", m_Superblock->LaskFSCKTime);
			KPrintf("FSCKInterval = %d\n", m_Superblock->FSCKInterval);
			KPrintf("OSID = %d\n", m_Superblock->OSID);
			KPrintf("MajorVersion = %d\n", m_Superblock->MajorVersion);
			KPrintf("UserID = %d\n", m_Superblock->UserID);
			KPrintf("GroupID = %d\n", m_Superblock->GroupID);
			KPrintf("FirstInode = %d\n", m_Superblock->FirstInode);
			KPrintf("InodeSize = %d\n", m_Superblock->InodeSize);
			KPrintf("BlockGroup = %d\n", m_Superblock->BlockGroup);
			KPrintf("OptionalFeatures = %d\n", m_Superblock->OptionalFeatures);
			KPrintf("RequiredFeatues = %d\n", m_Superblock->RequiredFeatues);
			KPrintf("UnsupportedFeatures = %d\n", m_Superblock->UnsupportedFeatures);
			KPrintf("FileSystemID = %s\n", m_Superblock->FileSystemID);
			KPrintf("VolumeName = %s\n", m_Superblock->VolumeName);
			KPrintf("PathVolumeLastMounted = %s\n", m_Superblock->PathVolumeLastMounted);
			KPrintf("CompressionAlgorithm = %d\n", m_Superblock->CompressionAlgorithm);
			KPrintf("PreallocateBlockCountFile = %d\n", m_Superblock->PreallocateBlockCountFile);
			KPrintf("PreallocateBlockCountDir = %d\n", m_Superblock->PreallocateBlockCountDir);
			KPrintf("JournalID = %s\n", m_Superblock->JournalID);
			KPrintf("JournalDevice = %d\n", m_Superblock->JournalDevice);
			KPrintf("OrphanInodeListHead = %d\n", m_Superblock->OrphanInodeListHead);
			KPrintf("JournalInode = %d\n", m_Superblock->JournalInode);
			KPrintf("-----EXT2 DUMP SUPERBLOCK EXTRA-----\n");
			KPrintf("BlockGroupCount = %d\n", m_Superblock->BlockCount / m_Superblock->BlockGroupBlockCount);
			KPrintf("BlockGroupSize = %d\n", m_Superblock->BlockSize * 8 - 1);
			KPrintf("-----EXT2 DUMP SUPERBLOCK END-----\n");
		}

		void Ext2FileSystem::DumpBlockGroupDescriptorTable()
		{
			KPrintf("\n-----EXT2 DUMP (%d) BLOCKGROUP DESCRIPTOR START-----\n", m_BlockGroupDescriptorCount);
			for (size_t i = 0; i < m_BlockGroupDescriptorCount; i++)
			{
				KPrintf("---START BlockGroup(%d)---\n", i);
				KPrintf("BlockBitmap = %d\n", m_BlockGroupDescriptors[i].BlockBitmap);
				KPrintf("BlockBitmap = %d\n", m_BlockGroupDescriptors[i].BlockBitmap);
				KPrintf("InodeTable = %d\n", m_BlockGroupDescriptors[i].InodeTable);
				KPrintf("FreeBlockCount = %d\n", m_BlockGroupDescriptors[i].FreeBlockCount);
				KPrintf("FreeInodeCount = %d\n", m_BlockGroupDescriptors[i].FreeInodeCount);
				KPrintf("DirCount = %d\n", m_BlockGroupDescriptors[i].DirCount);
				KPrintf("---END BlockGroup(%d)---\n", i);
			}
			KPrintf("-----EXT2 DUMP (%d) BLOCKGROUP DESCRIPTOR END-----\n", m_BlockGroupDescriptorCount);
		}

		void Ext2FileSystem::DumpInode(const Ext2Inode& inode)
		{
			KPrintf("\n-----EXT2 DUMP INODE START-----\n");
			KPrintf("Mode = %d\n", inode.Mode);
			KPrintf("UserID = %d\n", inode.UserID);
			KPrintf("SizeLow = %d\n", inode.SizeLow);
			KPrintf("LastAccessTime = %d\n", inode.LastAccessTime);
			KPrintf("LastCreationTime = %d\n", inode.LastCreationTime);
			KPrintf("LastModificationTime = %d\n", inode.LastModificationTime);
			KPrintf("DeletionTime = %d\n", inode.DeletionTime);
			KPrintf("GroupID = %d\n", inode.GroupID);
			KPrintf("HardLinkCount = %d\n", inode.HardLinkCount);
			KPrintf("SectorCount = %d\n", inode.SectorCount);
			KPrintf("Flags = %d\n", inode.Flags);
			KPrintf("OSSV1 = %d\n", inode.OSSV1);
			KPrintf("DPB = %d\n", inode.DBP);
			KPrintf("DPB[0] = %d\n", inode.DBP[0]);
			KPrintf("SIBP = %d\n", inode.SIBP);
			KPrintf("DIBP = %d\n", inode.DIBP);
			KPrintf("TIBP = %d\n", inode.TIBP);
			KPrintf("FragmentAddr = %d\n", inode.FragmentAddr);
			KPrintf("OSSV2 = %d\n", inode.OSSV2);
			KPrintf("-----EXT2 DUMP INODE END-----\n");
		}

		void Ext2FileSystem::DumpInodeDirs(const Ext2Inode& inode)
		{
			if (!(inode.Mode & (uint16_t)Ext2InodeType::Directory))
				return;

			for (size_t i = 0; i < EXT2_DBP_SIZE; i++)
			{
				if (!inode.DBP[i])
					continue;

				Ext2DirEntry* dir = (Ext2DirEntry*)ReadBlock(inode.DBP[i]);

				if (dir->Size == 0 || dir->NameLength == 0 || dir->Inode <= 0 || dir->Inode >= m_Superblock->InodeCount)
				{
					delete dir;
					break;
				}

				while (dir->NameLength > 0 && dir->Inode > 0 && dir->Inode < m_Superblock->InodeCount)
				{
					DumpDirEntry(*dir);
					dir = (Ext2DirEntry*)((uint8_t*)dir + dir->Size);
				}

				delete dir;
			}
		}

		void Ext2FileSystem::DumpDirEntry(const Ext2DirEntry& dir)
		{
			KPrintf("\n-----EXT2 DUMP Dirent START-----\n");
			char* tmpname = new char[dir.NameLength + 1];
			Std::Memcpy(tmpname, dir.Name, dir.NameLength);
			tmpname[dir.NameLength] = 0;
			KPrintf("Name = %s\n", tmpname);
			delete[] tmpname;
			KPrintf("Inode = %d\n", dir.Inode);
			KPrintf("Size = %d\n", dir.Size);
			KPrintf("NameLength = %d\n", dir.NameLength);
			KPrintf("Type = %d\n", dir.Type);
			KPrintf("-----EXT2 DUMP Dirent END-------\n");
		}
	}
}
