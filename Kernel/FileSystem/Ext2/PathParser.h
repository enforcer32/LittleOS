#pragma once

#include <Al/Memory.h>
#include <Al/StaticString.h>
#include <Al/Vector.h>

namespace Kernel
{
	namespace FileSystem
	{
		struct Ext2Path
		{
			Al::UniquePtr<Al::StaticString> Path;
			Al::Vector<Al::UniquePtr<Al::StaticString>> Parts;
			Al::UniquePtr<Al::StaticString> Root;
			Al::UniquePtr<Al::StaticString> Target;

			operator bool() { return *Root.Get() == "/"; }
		};

		class Ext2PathParser
		{
		public:
			Ext2PathParser(Al::StringView path);
			Al::UniquePtr<Ext2Path> Parse();

		private:
			Al::UniquePtr<Al::StaticString> m_Path;
		};
	}
}
