#pragma once

#include <Kernel/FileSystem/FileSystem.h>
#include <Kernel/FileSystem/File.h>
#include <Kernel/FileSystem/Ext2/Superblock.h>
#include <Kernel/FileSystem/Ext2/Inode.h>
#include <Al/Vector.h>
#include <Al/Memory.h>

namespace Kernel
{
	namespace FileSystem
	{
		class Ext2FileSystem : public KFileSystem
		{
		public:
			virtual int32_t Init(Al::UniquePtr<Disk> disk) override;
			virtual Al::SharedPtr<File> Open(Al::StringView path) override;
			virtual int32_t Read(const Al::SharedPtr<File>& file, void* buf, size_t len, size_t offset) override;
			virtual int32_t Write(const Al::SharedPtr<File>& file, const void* buf, size_t len, size_t offset) override;

		private:
			int32_t ParseSuperblock();
			int32_t ParseBlockGroupDescriptors();
			const Ext2BlockGroupDescriptor& GetBlockGroupDescriptor(uint32_t group) const;

			Al::SharedPtr<Ext2Inode> GetInode(uint32_t inode);
			Al::SharedPtr<Ext2Inode> GetInodeFromDir(const Al::SharedPtr<Ext2Inode>& inodebuf, Al::StringView name);
			Al::SharedPtr<Ext2Inode> GetInodeFromPath(Al::StringView path);

			void* ReadBlock(uint32_t block);
			void* ReadFile(const Al::SharedPtr<Ext2Inode>& inodebuf, size_t size);
			int32_t ReadFile(const Al::SharedPtr<Ext2Inode>& inodebuf, void* buf, size_t len);

		public:
			// DEBUG
			void DumpSuperblock();
			void DumpBlockGroupDescriptors();
			void DumpInode(const Al::SharedPtr<Ext2Inode>& inodebuf);
			void DumpDirInode(const Al::SharedPtr<Ext2Inode>& inodebuf);
			void DumpDirent(const Ext2Dirent* dir);

		private:
			Al::UniquePtr<Disk> m_Disk;
			Al::UniquePtr<Ext2Superblock> m_Superblock;
			uint32_t m_FirstBlockGroup;
			uint32_t m_BlockGroupDescriptorCount;
			Al::Vector<Ext2BlockGroupDescriptor> m_BlockGroupDescriptors;
			Al::SharedPtr<Ext2Inode> m_RootInode;
		};
	}
}
