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
			int32_t Read28(uint32_t lba, uint32_t sector, void* outbuf);
			int32_t Write28(uint32_t lba, uint32_t sector, void* inbuf);

		private:
			int32_t Identify(void* outbuf);
			void SelectDrive();
			void Delay400NS();
			void Poll();
			int8_t ReadSector28(uint32_t lba, void* outbuf);
			int8_t WriteSector28(uint32_t lba, void* inbuf);
		
		private:
			bool m_Initialized;
			ATABus m_Bus;
			ATADrive m_Drive;
			uint16_t m_Port;
		};
	}
}
