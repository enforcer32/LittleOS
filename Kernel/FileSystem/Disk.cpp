#include <Kernel/FileSystem/Disk.h>
#include <Al/CString.h>

namespace Kernel
{
	namespace FileSystem
	{
		int32_t Disk::Init(Al::UniquePtr<Drivers::ATA> ata)
		{
			m_ATA = Al::Move(ata);
			return 0;
		}

		size_t Disk::GetSectorSize() const
		{
			return m_ATA->GetSectorSize();
		}

		int32_t Disk::ReadSector(void* buf, size_t sector, size_t lba)
		{
			return m_ATA->Read28(buf, sector, lba);
		}

		int32_t Disk::WriteSector(const void* buf, size_t sector, size_t lba)
		{
			return m_ATA->Write28(buf, sector, lba);
		}

		int32_t Disk::Read(void* buf, size_t len, size_t offset)
		{
			size_t sectorSize = m_ATA->GetSectorSize();
			size_t sectors = (len / sectorSize);
			bool aligned = (len % sectorSize == 0);

			if (aligned)
				return m_ATA->Read28(buf, sectors, offset);

			sectors++;
			char* tmp = new char[sectors * sectorSize];
			m_ATA->Read28(tmp, sectors, offset);
			Al::Memcpy(buf, tmp, len);

			delete[] tmp;
			return len;
		}

		int32_t Disk::Write(const void* buf, size_t len, size_t offset)
		{
			size_t sectorSize = m_ATA->GetSectorSize();
			size_t sectors = (len / sectorSize);
			bool aligned = (len % sectorSize == 0);

			if (aligned)
				return m_ATA->Write28(buf, sectors, offset);

			sectors++;
			char* tmp = new char[sectors * sectorSize];
			m_ATA->Read28(tmp, sectors, offset);
			Al::Memcpy(tmp, buf, len);
			m_ATA->Write28(tmp, sectors, offset);

			delete[] tmp;
			return len;
		}
	}
}
