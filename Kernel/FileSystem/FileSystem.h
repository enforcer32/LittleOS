#pragma once

#include <Kernel/FileSystem/Disk.h>
#include <Al/StringView.h>
//#include <Kernel/FileSystem/File.h>

namespace Kernel
{
	namespace FileSystem
	{
		class File;

		class KFileSystem
		{
		public:
			bool operator!=(const KFileSystem& rhs) const
			{
				return *this != rhs;
			}

			virtual ~KFileSystem() = default;

			virtual int32_t Init(Al::UniquePtr<Disk> disk) = 0;
			virtual Al::SharedPtr<File> Open(Al::StringView path) = 0;
			virtual int32_t Read(const Al::SharedPtr<File>& file, void* buf, size_t len, size_t offset) = 0;
			virtual int32_t Write(const Al::SharedPtr<File>& file, const void* buf, size_t len, size_t offset) = 0;
		};
	}
}
