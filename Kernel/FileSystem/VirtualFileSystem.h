#pragma once

#include <Kernel/FileSystem/MountManager.h>
#include <Kernel/FileSystem/File.h>
#include <Al/Memory.h>
#include <Al/StringView.h>

namespace Kernel
{
	namespace FileSystem
	{
		class VirtualFileSystem
		{
		public:
			static int32_t Init();
			static void MountRoot(const Al::SharedPtr<KFileSystem>& fileSystem);
			static void Mount(const MountInfo& mount);
			static void UnMount(Al::StringView location);
			static Al::SharedPtr<MountInfo> GetMount(Al::StringView location);
			static Al::SharedPtr<KFileSystem> GetMountFileSystem(Al::StringView location);

			static Al::SharedPtr<File> Open(Al::StringView path);

		private:
			static MountManager* s_MountManager;
		};
	}
}
