#pragma once

#include <Kernel/FileSystem/FileSystem.h>
#include <Kernel/FileSystem/Ext2/Superblock.h>
#include <Kernel/FileSystem/Ext2/Inode.h>
#include <Standard/Vector.h>
#include <Standard/Memory.h>
#include <Standard/StaticString.h>

namespace Kernel
{
	namespace FileSystem
	{
		class Ext2FileSystem : public FileSystem
		{
		public:
			virtual int32_t Init(Std::UniquePtr<Disk> disk) override;
			Std::SharedPtr<Ext2Inode> GetInode(uint32_t inode);
			Std::SharedPtr<Ext2Inode> GetInodeFromDir(const Std::SharedPtr<Ext2Inode>& inodebuf, const Std::UniquePtr<Std::StaticString>& name);
			Std::SharedPtr<Ext2Inode> GetInodeFromPath(const Std::UniquePtr<Std::StaticString>& path);

			void* ReadBlock(uint32_t block);
			void* ReadFile(const Std::SharedPtr<Ext2Inode>& inodebuf, size_t size);

		private:
			int32_t ParseSuperblock();
			int32_t ParseBlockGroupDescriptors();
			const Ext2BlockGroupDescriptor& GetBlockGroupDescriptor(uint32_t group) const;

		public:
			// DEBUG
			void DumpSuperblock();
			void DumpBlockGroupDescriptors();
			void DumpInode(const Std::SharedPtr<Ext2Inode>& inodebuf);
			void DumpDirInode(const Std::SharedPtr<Ext2Inode>& inodebuf);
			void DumpDirent(const Ext2Dirent* dir);

		private:
			Std::UniquePtr<Disk> m_Disk;
			Std::UniquePtr<Ext2Superblock> m_Superblock;
			uint32_t m_FirstBlockGroup;
			uint32_t m_BlockGroupDescriptorCount;
			Std::Vector<Ext2BlockGroupDescriptor> m_BlockGroupDescriptors;
			Std::SharedPtr<Ext2Inode> m_RootInode;
		};
	}
}
