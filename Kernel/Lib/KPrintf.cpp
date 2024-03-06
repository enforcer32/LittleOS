#include <Kernel/Lib/KPrintf.h>
#include <Kernel/Drivers/VGA.h>
#include <Al/TypeConverter.h>

namespace Kernel
{
	void KPrint(const char* str)
	{
		Drivers::VGA::PutStr(str);
	}

	void KPutChar(char ch)
	{
		Drivers::VGA::PutChar(ch);
	}

	int32_t KPrintf(const char* fmt, ...)
	{
		va_list ap;
		const char* ptr;
		char* c;
		int32_t val;

		va_start(ap, fmt);
		for (ptr = fmt; *ptr; ptr++) {
			if (*ptr != '%') {
				KPutChar(*ptr);
				continue;
			}

			switch (*++ptr) {
			case 'd':
			case 'u':
			case 'i': {
				val = va_arg(ap, int);
				char tmp[12];
				Al::TypeConverter::IntToStr(val, tmp, sizeof(tmp), 10);
				KPrint(tmp);
				break;
			}
			case 'x': {
				val = va_arg(ap, int);
				char tmp[12];
				Al::TypeConverter::IntToStr(val, tmp, sizeof(tmp), 16);
				KPrint(tmp);
				break;
			}
			case 'c': {
				char ch = va_arg(ap, int);
				KPutChar(ch);
				break;
			}
			case 's':
				c = va_arg(ap, char*);
				KPrint(c);
				break;
			default:
				KPutChar(*ptr);
				break;
			}
		}
		va_end(ap);

		return 0;
	}
}
