#pragma once

#include <Al/Types.h>
#include <Kernel/FileSystem/Disk.h>

namespace Kernel
{
	namespace FileSystem
	{
		class FileSystem
		{
		public:
			virtual ~FileSystem() = default;

			virtual int32_t Init(Al::UniquePtr<Disk> disk) = 0;
		};
	}
}
