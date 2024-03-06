#include <Kernel/Drivers/ATA.h>
#include <Kernel/Lib/KPrintf.h>
#include <Kernel/CPU/Port.h>
#include <Kernel/Memory/KMalloc.h>
#include <Kernel/Lib/Errno.h>

/* DATA PORTS */
#define ATA_PRIMARY_IO 0x1F0
#define ATA_SECONDARY_IO 0x170

/* Registers */
#define ATA_REG_DATA 0x0
#define ATA_REG_ERROR 0x1
#define ATA_REG_FEATURES 0x1
#define ATA_REG_SECCOUNT0 0x2
#define ATA_REG_LBALO 0x3
#define ATA_REG_LBAMID 0x4
#define ATA_REG_LBAHI 0x5
#define ATA_REG_HDDEVSEL 0x6
#define ATA_REG_COMMAND 0x7
#define ATA_REG_STATUS 0x7
#define ATA_REG_ALTSTATUS 0xC

/* Commands */
#define ATA_CMD_IDENTIFY 0xEC
#define ATA_CMD_READ_PIO 0x20
#define ATA_CMD_WRITE_PIO 0x30
#define ATA_CMD_CACHE_FLUSH 0xE7

/* STATUS */
#define ATA_SR_BSY 0x80
#define ATA_SR_DRDY 0x40
#define ATA_SR_DF 0x20
#define ATA_SR_DSC 0x10
#define ATA_SR_DRQ 0x08
#define ATA_SR_CORR 0x04
#define ATA_SR_IDX 0x02
#define ATA_SR_ERR 0x01

/* INFO */
#define ATA_IDENT_DEVICETYPE 0
#define ATA_IDENT_CYLINDERS 2
#define ATA_IDENT_HEADS 6
#define ATA_IDENT_SECTORS 12
#define ATA_IDENT_SERIAL 20
#define ATA_IDENT_MODEL 54
#define ATA_IDENT_CAPABILITIES 98
#define ATA_IDENT_FIELDVALID 106
#define ATA_IDENT_MAX_LBA 120
#define ATA_IDENT_COMMANDSETS 164
#define ATA_IDENT_MAX_LBA_EXT 200

#define ATA_SECTOR_SIZE 512

namespace Kernel
{
	namespace Drivers
	{
		int32_t ATA::Init(ATABus bus, ATADrive drive)
		{
			KPrintf("Initializing ATA Interface for Bus: %d, Drive: %d\n", bus, drive);

			m_Bus = bus;
			m_Drive = drive;

			uint8_t* data = new uint8_t[ATA_SECTOR_SIZE];
			if (!data)
				return -ENOMEM;

			if (Identify(data) == 0)
			{
				char* model = new char[40];
				if (!model)
					return -ENOMEM;

				for (size_t i = 0; i < 40; i += 2) {
					model[i] = data[ATA_IDENT_MODEL + i + 1];
					model[i + 1] = data[ATA_IDENT_MODEL + i];
				}

				if (!data[0])
				{
					KPrintf("Failed to Detect ATA for Bus: %d, Drive: %d\n", bus, drive);
					return -1;
				}
				
				KPrintf("Detected ATA Device: %s\n", model);
			}

			m_Initialized = true;
			return 0;
		}

		int32_t ATA::Read28(uint32_t lba, uint32_t sector, void* outbuf)
		{
			if (!m_Initialized)
			{
				KPrintf("ATA Not Initialized\n");
				return -1;
			}

			uint8_t* ptr = (uint8_t*)outbuf;
			for (size_t i = 0; i < sector; i++)
			{
				ReadSector28(lba, ptr);
				ptr += ATA_SECTOR_SIZE;
			}

			return sector * ATA_SECTOR_SIZE;
		}
		
		int32_t ATA::Write28(uint32_t lba, uint32_t sector, void* inbuf)
		{
			uint8_t* ptr = (uint8_t*)inbuf;
			for (size_t i = 0; i < sector; i++)
			{
				WriteSector28(lba, ptr);
				ptr += ATA_SECTOR_SIZE;
			}

			return sector * ATA_SECTOR_SIZE;
		}

		int32_t ATA::Identify(void* outbuf)
		{
			SelectDrive();

			CPU::Port::outb(m_Port + ATA_REG_SECCOUNT0, 0);
			CPU::Port::outb(m_Port + ATA_REG_LBALO, 0);
			CPU::Port::outb(m_Port + ATA_REG_LBAMID, 0);
			CPU::Port::outb(m_Port + ATA_REG_LBAHI, 0);
			CPU::Port::outb(m_Port + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);

			uint8_t status = CPU::Port::insb(m_Port + ATA_REG_STATUS);
			if (status)
			{
				while ((CPU::Port::insb(m_Port + ATA_REG_STATUS) & ATA_SR_BSY) != 0)
				{
					// NON ATA DRIVE
					if (CPU::Port::insb(m_Port + ATA_REG_LBAMID) != 0 || CPU::Port::insb(m_Port + ATA_REG_LBAHI) != 0)
						return -1;
				}

			retry:
				status = CPU::Port::insb(m_Port + ATA_REG_STATUS);
				if (status & ATA_SR_ERR) {
					KPrintf("Drive %d on Bus %d has error\n", m_Drive, m_Bus);
					return status;
				}

				while (!(status & ATA_SR_DRQ))
					goto retry;

				uint16_t* ptr = (uint16_t*)outbuf;
				for (uint32_t i = 0; i < 256; i++) {
					*ptr = CPU::Port::insw(m_Port + ATA_REG_DATA);
					ptr++;
				}
			}

			return 0;
		}

		void ATA::SelectDrive()
		{
			if (m_Bus == ATABus::Primary)
			{
				m_Port = ATA_PRIMARY_IO;
				if (m_Drive == ATADrive::Master)
					CPU::Port::outb(m_Port + ATA_REG_HDDEVSEL, 0xA0);
				else if (m_Drive == ATADrive::Slave)
					CPU::Port::outb(m_Port + ATA_REG_HDDEVSEL, 0xB0);
			}
			else if (m_Bus == ATABus::Secondary)
			{
				m_Port = ATA_SECONDARY_IO;
				if (m_Drive == ATADrive::Master)
					CPU::Port::outb(m_Port + ATA_REG_HDDEVSEL, 0xA0);
				else if (m_Drive == ATADrive::Slave)
					CPU::Port::outb(m_Port + ATA_REG_HDDEVSEL, 0xB0);
			}
		}

		void ATA::Delay400NS()
		{
			for (size_t i = 0; i < 14; i++)
				CPU::Port::insb(m_Port + ATA_REG_ALTSTATUS);
		}

		void ATA::Poll()
		{
			Delay400NS();

		pool:
			uint8_t status = CPU::Port::insb(m_Port + ATA_REG_STATUS);
			if (status & ATA_SR_BSY)
				goto pool;

		retry:
			status = CPU::Port::insb(m_Port + ATA_REG_STATUS);
			if (status & ATA_SR_ERR)
				KPrintf("ATA ERR DEVICE FAILURE\n");

			if (!(status & ATA_SR_DRQ))
				goto retry;
		}

		int8_t ATA::ReadSector28(uint32_t lba, void* outbuf)
		{
			uint8_t seldrive = (m_Drive == ATADrive::Master ? 0xE0 : 0xF0);
			CPU::Port::outb(m_Port + ATA_REG_HDDEVSEL, (seldrive | (uint8_t)(lba >> 24 & 0x0F)));
			CPU::Port::outb(m_Port + 1, 0x0);
			CPU::Port::outb(m_Port + ATA_REG_SECCOUNT0, 1);
			CPU::Port::outb(m_Port + ATA_REG_LBALO, (uint8_t)(lba));
			CPU::Port::outb(m_Port + ATA_REG_LBAMID, (uint8_t)(lba >> 8));
			CPU::Port::outb(m_Port + ATA_REG_LBAHI, (uint8_t)(lba >> 16));
			CPU::Port::outb(m_Port + ATA_REG_COMMAND, ATA_CMD_READ_PIO);

			Poll();

			uint16_t* ptr = (uint16_t*)outbuf;
			for (size_t i = 0; i < 256; i++) {
				*ptr = CPU::Port::insw(m_Port + ATA_REG_DATA);
				ptr++;
			}

			Delay400NS();
			return 0;
		}

		int8_t ATA::WriteSector28(uint32_t lba, void* inbuf)
		{
			uint8_t seldrive = (m_Drive == ATADrive::Master ? 0xE0 : 0xF0);
			CPU::Port::outb(m_Port + ATA_REG_HDDEVSEL, (seldrive | (uint8_t)(lba >> 24 & 0x0F)));
			CPU::Port::outb(m_Port + 1, 0x0);
			CPU::Port::outb(m_Port + ATA_REG_SECCOUNT0, 1);
			CPU::Port::outb(m_Port + ATA_REG_LBALO, (uint8_t)(lba));
			CPU::Port::outb(m_Port + ATA_REG_LBAMID, (uint8_t)(lba >> 8));
			CPU::Port::outb(m_Port + ATA_REG_LBAHI, (uint8_t)(lba >> 16));
			CPU::Port::outb(m_Port + ATA_REG_COMMAND, ATA_CMD_WRITE_PIO);

			Poll();

			uint16_t* ptr = (uint16_t*)inbuf;
			for (size_t i = 0; i < 256; i++) {
				CPU::Port::outw(m_Port + ATA_REG_DATA, *ptr);
				ptr++;
			}

			CPU::Port::outb(m_Port + ATA_REG_COMMAND, ATA_CMD_CACHE_FLUSH);

			Delay400NS();
			return 0;
		}
	}
}
