#pragma once

#include <Kernel/Drivers/ATA.h>
#include <Al/Memory.h>

namespace Kernel
{
	namespace FileSystem
	{
		class Disk
		{
		public:
			int32_t Init(Al::UniquePtr<Drivers::ATA> ata);
			size_t GetSectorSize() const;
			int32_t ReadSector(void* buf, size_t sector, size_t lba);
			int32_t WriteSector(const void* buf, size_t sector, size_t lba);
			int32_t Read(void* buf, size_t len, size_t offset);
			int32_t Write(const void* buf, size_t len, size_t offset);

		private:
			Al::UniquePtr<Drivers::ATA> m_ATA;
		};
	}
}
