#include <Kernel/FileSystem/Ext2/FileSystem.h>
#include <Kernel/FileSystem/Ext2/PathParser.h>
#include <Kernel/Lib/KPrintf.h>
#include <Kernel/Lib/KPanic.h>
#include <Al/Utility.h>
#include <Al/CString.h>

namespace Kernel
{
	namespace FileSystem
	{
		int32_t Ext2FileSystem::Init(Al::UniquePtr<Disk> disk)
		{
			m_Disk = Al::Move(disk);

			if (ParseSuperblock() != 0)
			{
				KPrintf("Ext2FileSystem Failed to Parse Superblock\n");
				return -1;
			}

			if (ParseBlockGroupDescriptors() != 0)
			{
				KPrintf("Ext2FileSystem Failed to Parse ParseBlockGroupDescriptorTable\n");
				return -1;
			}

			m_RootInode = GetInode(EXT2_ROOT_INODE);
			if (!m_RootInode)
			{
				KPrintf("Ext2FileSystem Failed to Get Root Inode: %d\n", EXT2_ROOT_INODE);
				return -1;
			}

			return 0;
		}

		Al::SharedPtr<File> Ext2FileSystem::Open(Al::StringView path)
		{
			if (GetInodeFromPath(path))
				return Al::MakeShared<File>(Al::SharedPtr<KFileSystem>(this), path);

			return nullptr;
		}

		int32_t Ext2FileSystem::Read(const Al::SharedPtr<File>& file, void* buf, size_t len, size_t offset)
		{
			auto inode = GetInodeFromPath(file->GetFilePath());
			return ReadFile(inode, buf, len);
		}

		int32_t Ext2FileSystem::Write(const Al::SharedPtr<File>& file, const void* buf, size_t len, size_t offset)
		{
			KPrintf("Ext2FileSystem: WRITE NOT IMPLEMENTED!\n");
			return -1;
		}

		int32_t Ext2FileSystem::ParseSuperblock()
		{
			m_Superblock = Al::MakeUnique<Ext2Superblock>();
			if (m_Disk->Read(m_Superblock.Get(), EXT2_SUPERBLOCK_SIZE, (EXT2_SUPERBLOCK_OFFSET / m_Disk->GetSectorSize())) != 1024)
			{
				KPrintf("Ext2FileSystem Failed to Read Ext2Superblock\n");
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

		int32_t Ext2FileSystem::ParseBlockGroupDescriptors()
		{
			m_FirstBlockGroup = m_Superblock->FirstDataBlock + 1;
			m_BlockGroupDescriptorCount = m_Superblock->BlockCount / m_Superblock->BlockGroupBlockCount;
			if (!m_BlockGroupDescriptorCount)
				m_BlockGroupDescriptorCount = 1;

			for (uint32_t i = 0; i < m_BlockGroupDescriptorCount; i++)
			{
				uint32_t index = (i * sizeof(Ext2BlockGroupDescriptor)) / m_Superblock->BlockSize;
				uint32_t offset = (i * sizeof(Ext2BlockGroupDescriptor)) % m_Superblock->BlockSize;

				uint8_t* block = (uint8_t*)ReadBlock(m_FirstBlockGroup + index);
				m_BlockGroupDescriptors.PushBack(*(Ext2BlockGroupDescriptor*)(block + offset));
				delete block;
			}

			return 0;
		}

		const Ext2BlockGroupDescriptor& Ext2FileSystem::GetBlockGroupDescriptor(uint32_t group) const
		{
			return m_BlockGroupDescriptors[group];
		}

		Al::SharedPtr<Ext2Inode> Ext2FileSystem::GetInode(uint32_t inode)
		{
			if (inode == 0)
				return nullptr;

			uint32_t bg = (inode - 1) / m_Superblock->BlockGroupInodeCount;
			auto& bgd = GetBlockGroupDescriptor(bg);

			uint32_t index = (inode - 1) % m_Superblock->BlockGroupInodeCount;
			uint32_t block = (index * m_Superblock->InodeSize) / m_Superblock->BlockSize;

			index = index % (m_Superblock->BlockSize / m_Superblock->InodeSize);
			uint32_t offset = (index * m_Superblock->InodeSize);

			uint8_t* inodetable = (uint8_t*)ReadBlock(bgd.InodeTable + block);
			Al::SharedPtr<Ext2Inode> inodebuf = Al::MakeShared<Ext2Inode>(*(Ext2Inode*)(inodetable + offset));
			delete inodetable;
			return inodebuf;
		}

		Al::SharedPtr<Ext2Inode> Ext2FileSystem::GetInodeFromDir(const Al::SharedPtr<Ext2Inode>& inodebuf, Al::StringView name)
		{
			if (!(inodebuf->Mode & (uint16_t)Ext2InodeType::Directory))
				return nullptr;

			for (size_t i = 0; i < EXT2_DBP_SIZE; i++)
			{
				if (!inodebuf->DBP[i]) continue;

				uint8_t* data = (uint8_t*)ReadBlock(inodebuf->DBP[i]);
				Ext2Dirent* dir = (Ext2Dirent*)data;

				if (dir->Size == 0 || dir->NameLength == 0 || dir->Inode <= 0 || dir->Inode >= m_Superblock->InodeCount)
				{
					delete data;
					break;
				}

				while (dir->NameLength > 0 && dir->Inode > 0 && dir->Inode < m_Superblock->InodeCount)
				{
					dir->Name[dir->NameLength] = 0;
					if (!Al::Strncmp((char*)dir->Name, name.Data(), dir->NameLength))
					{
						uint32_t inode = dir->Inode;
						delete data;
						return GetInode(inode);
					}
					dir = (Ext2Dirent*)((uint8_t*)dir + dir->Size);
				}

				delete data;
			}
			return nullptr;
		}

		Al::SharedPtr<Ext2Inode> Ext2FileSystem::GetInodeFromPath(Al::StringView path)
		{
			if (path.Size() == 1 && path.Data()[0] == '/')
				return m_RootInode;

			Ext2PathParser pathParser(path);
			auto parsed = pathParser.Parse();
			if (!parsed)
			{
				KPrintf("Ext2FileSystem Failed to ParsePath: %s\n", path.Data());
				return nullptr;
			}

			auto node = m_RootInode;
			for (size_t i = 0; i < parsed->Parts.Size(); i++)
			{
				node = GetInodeFromDir(node, parsed->Parts[i]->View());
				if (!node)
				{
					KPrintf("Ext2FileSystem Failed to Get %s From %s\n", parsed->Parts[i]->Data(), path.Data());
					return nullptr;
				}

				if (!(node->Mode & (uint16_t)Ext2InodeType::Directory))
				{
					KPrintf("Ext2FileSystem Not a Directory: %s\n", parsed->Parts[i]->Data(), path.Data());
					return nullptr;
				}
			}

			return GetInodeFromDir(node, parsed->Target->View());
		}

		void* Ext2FileSystem::ReadBlock(uint32_t block)
		{
			uint8_t* buf = new uint8_t[m_Superblock->BlockSize];
			if (m_Disk->Read(buf, m_Superblock->BlockSize, (block * (m_Superblock->BlockSize / m_Disk->GetSectorSize()))) != (int32_t)m_Superblock->BlockSize)
			{
				KPrintf("Ext2FileSystem Read Block Error\n");
				return nullptr;
			}
			return buf;
		}

		void* Ext2FileSystem::ReadFile(const Al::SharedPtr<Ext2Inode>& inodebuf, size_t size)
		{
			if (!(inodebuf->Mode & (uint16_t)Ext2InodeType::RegularFile))
				return nullptr;

			uint32_t read = 0, curr = 0;
			uint8_t* data = new uint8_t[size];
			Al::Memset(data, 0, size);

			for (uint32_t i = 0; i < EXT2_DBP_SIZE; i++)
			{
				if (inodebuf->DBP[i])
				{
					uint8_t* block = (uint8_t*)ReadBlock(inodebuf->DBP[i]);
					while (read < size && curr < m_Superblock->BlockSize && block[curr])
						data[read++] = block[curr++];
					curr = 0;
					delete[] block;

					if (read >= size)
						goto end;
				}
			}

			if (read < size)
			{
				if (inodebuf->SIBP)
				{
					uint32_t* blocks = (uint32_t*)ReadBlock(inodebuf->SIBP);
					for (size_t i = 0; i < m_Superblock->BlockSize / 4; i++)
					{
						if (blocks[i])
						{
							uint8_t* block = (uint8_t*)ReadBlock(blocks[i]);
							while (read < size && curr < m_Superblock->BlockSize && block[curr])
								data[read++] = block[curr++];
							curr = 0;
							delete[] block;

							if (read >= size)
								goto end;
						}
					}
					delete blocks;
				}

				if (inodebuf->DIBP)
				{
					uint32_t* blocks = (uint32_t*)ReadBlock(inodebuf->DIBP);
					for (uint32_t i = 0; i < m_Superblock->BlockSize / 4; i++)
					{
						if (blocks[i])
						{
							uint32_t* subblocks = (uint32_t*)ReadBlock(blocks[i]);
							for (uint32_t i = 0; i < m_Superblock->BlockSize / 4; i++)
							{
								if (subblocks[i])
								{
									uint8_t* block = (uint8_t*)ReadBlock(subblocks[i]);
									while (read < size && curr < m_Superblock->BlockSize && block[curr])
										data[read++] = block[curr++];
									curr = 0;
									delete[] block;

									if (read >= size)
										goto end;
								}
							}
							delete subblocks;
						}
					}
					delete blocks;
				}

				if (inodebuf->TIBP)
					KPanic("Ext2FileSystem Doesn't Support Reading Triply Indirect Block Pointer\n");
			}

		end:
			return data;
		}

		int32_t Ext2FileSystem::ReadFile(const Al::SharedPtr<Ext2Inode>& inodebuf, void* buf, size_t len)
		{
			if (!(inodebuf->Mode & (uint16_t)Ext2InodeType::RegularFile))
				return -1;

			uint32_t read = 0, curr = 0;
			uint8_t* data = (uint8_t*)buf;

			for (uint32_t i = 0; i < EXT2_DBP_SIZE; i++)
			{
				if (inodebuf->DBP[i])
				{
					uint8_t* block = (uint8_t*)ReadBlock(inodebuf->DBP[i]);
					while (read < len && curr < m_Superblock->BlockSize && block[curr])
						data[read++] = block[curr++];
					curr = 0;
					delete[] block;

					if (read >= len)
						goto end;
				}
			}

			if (read < len)
			{
				if (inodebuf->SIBP)
				{
					uint32_t* blocks = (uint32_t*)ReadBlock(inodebuf->SIBP);
					for (size_t i = 0; i < m_Superblock->BlockSize / 4; i++)
					{
						if (blocks[i])
						{
							uint8_t* block = (uint8_t*)ReadBlock(blocks[i]);
							while (read < len && curr < m_Superblock->BlockSize && block[curr])
								data[read++] = block[curr++];
							curr = 0;
							delete[] block;

							if (read >= len)
								goto end;
						}
					}
					delete blocks;
				}

				if (inodebuf->DIBP)
				{
					uint32_t* blocks = (uint32_t*)ReadBlock(inodebuf->DIBP);
					for (uint32_t i = 0; i < m_Superblock->BlockSize / 4; i++)
					{
						if (blocks[i])
						{
							uint32_t* subblocks = (uint32_t*)ReadBlock(blocks[i]);
							for (uint32_t i = 0; i < m_Superblock->BlockSize / 4; i++)
							{
								if (subblocks[i])
								{
									uint8_t* block = (uint8_t*)ReadBlock(subblocks[i]);
									while (read < len && curr < m_Superblock->BlockSize && block[curr])
										data[read++] = block[curr++];
									curr = 0;
									delete[] block;

									if (read >= len)
										goto end;
								}
							}
							delete subblocks;
						}
					}
					delete blocks;
				}

				if (inodebuf->TIBP)
					KPanic("Ext2FileSystem Doesn't Support Reading Triply Indirect Block Pointer\n");
			}

		end:
			return read;
		}

		void Ext2FileSystem::DumpSuperblock()
		{
			KPrintf("\n-----Ext2FileSystem DUMP SUPERBLOCK START-----\n");
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
			KPrintf("-----Ext2FileSystem DUMP SUPERBLOCK END-----\n");
		}

		void Ext2FileSystem::DumpBlockGroupDescriptors()
		{
			KPrintf("\n-----Ext2FileSystem DUMP (%d) BLOCKGROUP DESCRIPTOR START-----\n", m_BlockGroupDescriptorCount);
			for (uint32_t i = 0; i < m_BlockGroupDescriptorCount; i++)
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
			KPrintf("-----Ext2FileSystem DUMP (%d) BLOCKGROUP DESCRIPTOR END-----\n", m_BlockGroupDescriptorCount);
		}

		void Ext2FileSystem::DumpInode(const Al::SharedPtr<Ext2Inode>& inodebuf)
		{
			KPrintf("\n-----Ext2FileSystem DUMP INODE START-----\n");
			KPrintf("Mode = %d\n", inodebuf->Mode);
			KPrintf("UserID = %d\n", inodebuf->UserID);
			KPrintf("SizeLow = %d\n", inodebuf->SizeLow);
			KPrintf("LastAccessTime = %d\n", inodebuf->LastAccessTime);
			KPrintf("LastCreationTime = %d\n", inodebuf->LastCreationTime);
			KPrintf("LastModificationTime = %d\n", inodebuf->LastModificationTime);
			KPrintf("DeletionTime = %d\n", inodebuf->DeletionTime);
			KPrintf("GroupID = %d\n", inodebuf->GroupID);
			KPrintf("HardLinkCount = %d\n", inodebuf->HardLinkCount);
			KPrintf("SectorCount = %d\n", inodebuf->SectorCount);
			KPrintf("Flags = %d\n", inodebuf->Flags);
			KPrintf("OSSV1 = %d\n", inodebuf->OSSV1);
			KPrintf("DPB = %d\n", inodebuf->DBP);
			KPrintf("DPB[0] = %d\n", inodebuf->DBP[0]);
			KPrintf("SIBP = %d\n", inodebuf->SIBP);
			KPrintf("DIBP = %d\n", inodebuf->DIBP);
			KPrintf("TIBP = %d\n", inodebuf->TIBP);
			KPrintf("FragmentAddr = %d\n", inodebuf->FragmentAddr);
			KPrintf("OSSV2 = %d\n", inodebuf->OSSV2);
			KPrintf("-----Ext2FileSystem DUMP INODE END-----\n");
		}

		void Ext2FileSystem::DumpDirInode(const Al::SharedPtr<Ext2Inode>& inodebuf)
		{
			if (!(inodebuf->Mode & (uint16_t)Ext2InodeType::Directory))
				return;

			for (uint32_t i = 0; i < EXT2_DBP_SIZE; i++)
			{
				if (!inodebuf->DBP[i]) continue;

				uint8_t* data = (uint8_t*)ReadBlock(inodebuf->DBP[i]);
				Ext2Dirent* dir = (Ext2Dirent*)data;

				if (dir->Size == 0 || dir->NameLength == 0 || dir->Inode <= 0 || dir->Inode >= m_Superblock->InodeCount)
				{
					delete data;
					break;
				}

				while (dir->NameLength > 0 && dir->Inode > 0 && dir->Inode < m_Superblock->InodeCount)
				{
					DumpDirent(dir);
					dir = (Ext2Dirent*)((uint8_t*)dir + dir->Size);
				}

				delete data;
			}
		}

		void Ext2FileSystem::DumpDirent(const Ext2Dirent* dir)
		{
			KPrintf("\n-----Ext2FileSystem DUMP Dirent START-----\n");
			char* tmpname = new char[dir->NameLength + 1];
			Al::Memcpy(tmpname, dir->Name, dir->NameLength);
			tmpname[dir->NameLength] = 0;
			KPrintf("Name = %s\n", tmpname);
			delete[] tmpname;
			KPrintf("Inode = %d\n", dir->Inode);
			KPrintf("Size = %d\n", dir->Size);
			KPrintf("NameLength = %d\n", dir->NameLength);
			KPrintf("Type = %d\n", dir->Type);
			KPrintf("-----Ext2FileSystem DUMP Dirent END-------\n");
		}
	}
}
