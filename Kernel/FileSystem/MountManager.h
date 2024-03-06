#pragma once

#include <Kernel/FileSystem/FileSystem.h>
#include <Al/Vector.h>
#include <Al/StaticString.h>
#include <Al/StringView.h>

namespace Kernel
{
	namespace FileSystem
	{
		struct MountInfo
		{
			MountInfo()
			{
			}

			MountInfo(Al::UniquePtr<Al::StaticString> location, const Al::SharedPtr<KFileSystem>& fileSystem)
				: Location(Al::Move(location)), FileSystem(fileSystem)
			{
			}

			MountInfo(const MountInfo& mountinfo)
				: Location(mountinfo.Location->Clone()), FileSystem(mountinfo.FileSystem)
			{
			}

			Al::UniquePtr<Al::StaticString> Location;
			Al::SharedPtr<KFileSystem> FileSystem;

			bool IsMounted() const
			{
				return FileSystem != nullptr;
			}
		};

		class MountManager
		{
		public:
			MountManager();
			void AddMount(Al::StringView location, const Al::SharedPtr<KFileSystem>& fileSystem);
			void AddMount(const MountInfo& mount);
			void RemoveMount(Al::StringView location);
			void ReMountFileSystem(Al::StringView location, const Al::SharedPtr<KFileSystem>& fileSystem);
			Al::SharedPtr<MountInfo> GetMount(Al::StringView location);
			Al::SharedPtr<KFileSystem> GetMountFileSystem(Al::StringView location);
			void MountRoot(const Al::SharedPtr<KFileSystem>& fileSystem);
			bool HasMount(Al::StringView location) const;
			bool IsRootMounted() const;

		private:
			Al::Vector<Al::SharedPtr<MountInfo>> m_Mounts;
		};
	}
}
