#pragma once

#include <Standard/Types.h>

namespace Kernel
{
	namespace Drivers
	{
		enum class ATABus
		{
			Primary = 0x0,
			Secondary = 0x1
		};

		enum class ATADrive
		{
			Master = 0x0,
			Slave = 0x1
		};

		class ATA
		{
		public:
			int32_t Init(ATABus bus, ATADrive drive);
			size_t GetSectorSize() const;
			int32_t Read28(void* buf, size_t sector, size_t lba);
			int32_t Write28(const void* buf, size_t sector, size_t lba);

		private:
			int32_t Identify(void* buf);
			void SelectDrive();
			void Delay400NS();
			void Poll();
			void Flush();
			int8_t ReadSector28(void* buf, size_t lba);
			int8_t WriteSector28(const void* buf, size_t lba);
		
		private:
			size_t m_SectorSize;
			ATABus m_Bus;
			ATADrive m_Drive;
			uint16_t m_Port;
		};
	}
}
