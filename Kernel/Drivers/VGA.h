#pragma once

#include <stdint.h>

namespace Kernel
{
	namespace Drivers
	{
		enum class VGAColor : uint8_t
		{
			Black = 0,
			Blue = 1,
			Green = 2,
			Cyan = 3,
			Red = 4,
			Magenta = 5,
			Brown = 6,
			LightGrey = 7,
			DarkGrey = 8,
			LightBlue = 9,
			LightGreen = 10,
			LightCyan = 11,
			LightRed = 12,
			LightMagenta = 13,
			LightBrown = 14,
			White = 15
		};

		class VGA
		{
		public:
			static int32_t Init();
			static void ClearScreen();

			static void PutChar(uint8_t ch);
			static void PutCharColor(uint8_t ch, VGAColor color);
			static void PutStr(const char* str);

			static VGAColor GetColor();
			static void SetColor(VGAColor color);

			static void EnableCursor(uint8_t start, uint8_t end);
			static void DisableCursor();
			static void SetCursor(uint16_t offset);
			static uint16_t GetCursor();

		private:
			static void PutCharLocation(uint16_t col, uint16_t row, uint8_t ch, VGAColor color);
			static void ScrollDown();

		private:
			static uint16_t* s_VideoAddress;
			static uint16_t s_Col, s_Row;
			static VGAColor s_Color;
		};
	}
}
