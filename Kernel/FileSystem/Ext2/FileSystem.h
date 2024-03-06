#pragma once

#include <Kernel/FileSystem/FileSystem.h>
#include <Kernel/FileSystem/Ext2/Superblock.h>
#include <Kernel/FileSystem/Ext2/Inode.h>
#include <Standard/Vector.h>
#include <Standard/UniquePtr.h>

namespace Kernel
{
	namespace FileSystem
	{
		class Ext2FileSystem : public FileSystem
		{
		public:
			virtual int32_t Init(Std::UniquePtr<Disk> disk) override;
			virtual void Free() override;

		private:
			int32_t ParseSuperblock();
			int32_t ParseBlockGroupDescriptorTable();
			void* ReadBlock(size_t block);
			const Ext2BlockGroupDescriptor& GetBlockGroupDescriptor(uint32_t group);
			Ext2Inode* GetInode(uint32_t inode);

		public:
			// DEBUG
			void DumpSuperblock();
			void DumpBlockGroupDescriptorTable();
			void DumpInode(const Ext2Inode& inode);
			void DumpInodeDirs(const Ext2Inode& inode);
			void DumpDirEntry(const Ext2DirEntry& dir);

		private:
			Std::UniquePtr<Disk> m_Disk;
			Std::UniquePtr<Ext2Superblock> m_Superblock;
			Std::UniquePtr<Ext2Inode> m_RootInode;
			size_t m_FirstBlockGroupDescriptorTable;
			size_t m_BlockGroupDescriptorCount;
			Std::Vector<Ext2BlockGroupDescriptor> m_BlockGroupDescriptors;
		};
	}
}
