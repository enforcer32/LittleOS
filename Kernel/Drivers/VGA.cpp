#include <Kernel/Drivers/VGA.h>
#include <Kernel/CPU/Port.h>
#include <Al/CString.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_VIDEO_ADDRESS 0xB8000

#define VGA_CTRL_PORT 0x3D4
#define VGA_DATA_PORT 0x3D5

#define VGA_ENCODE(ch, color) ((color << 8) | ch)
#define VGA_GET_OFFSET(col, row) ((row * VGA_WIDTH + col) * 2)

namespace Kernel
{
	namespace Drivers
	{
		uint16_t* VGA::s_VideoAddress = nullptr;
		uint16_t VGA::s_Col = 0;
		uint16_t VGA::s_Row = 0;
		VGAColor VGA::s_Color = VGAColor::LightBrown;

		int32_t VGA::Init()
		{
			s_VideoAddress = (uint16_t*)VGA_VIDEO_ADDRESS;
			s_Col = 0;
			s_Row = 0;
			s_Color = VGAColor::LightBrown;
			ClearScreen();
			PutStr("Initializing VGA Driver...\n");
			return 0;
		}

		void VGA::ClearScreen()
		{
			for (uint16_t y = 0; y < VGA_HEIGHT; y++)
				for (uint16_t x = 0; x < VGA_WIDTH; x++)
					PutCharLocation(x, y, ' ', s_Color);
			SetCursor(0);
		}

		void VGA::PutChar(uint8_t ch)
		{
			PutCharColor(ch, s_Color);
		}

		void VGA::PutCharColor(uint8_t ch, VGAColor color)
		{
			if (s_Row >= VGA_HEIGHT)
				ScrollDown();

			if (ch == '\n')
			{
				s_Col = 0;
				s_Row += 1;
				return;
			}

			PutCharLocation(s_Col, s_Row, ch, color);
			s_Col += 1;

			if (s_Col >= VGA_WIDTH)
			{
				s_Col = 0;
				s_Row += 1;
			}

			SetCursor(VGA_GET_OFFSET(s_Col, s_Row));
		}

		void VGA::PutStr(const char* str)
		{
			for (uint32_t i = 0; i < Al::Strlen(str); i++)
				PutChar(str[i]);
		}

		VGAColor VGA::GetColor()
		{
			return s_Color;
		}

		void VGA::SetColor(VGAColor color)
		{
			s_Color = color;
		}

		void VGA::EnableCursor(uint8_t start, uint8_t end)
		{
			CPU::Port::outb(VGA_CTRL_PORT, 0x0A);
			CPU::Port::outb(VGA_DATA_PORT, (CPU::Port::insb(VGA_DATA_PORT) & 0xC0) | start);
			CPU::Port::outb(VGA_CTRL_PORT, 0x0B);
			CPU::Port::outb(VGA_DATA_PORT, (CPU::Port::insb(VGA_DATA_PORT) & 0xE0) | end);
		}

		void VGA::DisableCursor()
		{
			CPU::Port::outb(VGA_CTRL_PORT, 0x0A);
			CPU::Port::outb(VGA_DATA_PORT, 0x20);
		}

		void VGA::SetCursor(uint16_t offset)
		{
			offset /= 2;
			CPU::Port::outb(VGA_CTRL_PORT, 0x0F);
			CPU::Port::outb(VGA_DATA_PORT, (offset & 0xFF));
			CPU::Port::outb(VGA_CTRL_PORT, 0x0E);
			CPU::Port::outb(VGA_DATA_PORT, ((offset >> 8) & 0xFF));
		}

		uint16_t VGA::GetCursor()
		{
			uint16_t offset = 0;
			CPU::Port::outb(VGA_CTRL_PORT, 0x0F);
			offset = CPU::Port::insb(VGA_DATA_PORT);
			CPU::Port::outb(VGA_CTRL_PORT, 0x0E);
			offset |= (CPU::Port::insb(VGA_DATA_PORT) << 8);
			return offset;
		}

		void VGA::PutCharLocation(uint16_t col, uint16_t row, uint8_t ch, VGAColor color)
		{
			s_VideoAddress[(row * VGA_WIDTH) + col] = VGA_ENCODE(ch, (uint8_t)color);
		}

		void VGA::ScrollDown()
		{
			uint8_t* start = (uint8_t*)s_VideoAddress + VGA_WIDTH * 2;
			uint32_t size = s_Row * VGA_WIDTH * 2;
			Al::Memcpy(s_VideoAddress, start, size);
			start = (uint8_t*)s_VideoAddress + size;
			Al::Memsetw(start, VGA_ENCODE(' ', (uint8_t)VGAColor::Black), size);
			s_Row--;
		}
	}
}
