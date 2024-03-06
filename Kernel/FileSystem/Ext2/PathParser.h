#pragma once

#include <Standard/Memory.h>
#include <Standard/StaticString.h>
#include <Standard/Vector.h>

namespace Kernel
{
	namespace FileSystem
	{
		struct Ext2Path
		{
			Std::UniquePtr<Std::StaticString> Path;
			Std::Vector<Std::UniquePtr<Std::StaticString>> Parts;
			Std::UniquePtr<Std::StaticString> Root;
			Std::UniquePtr<Std::StaticString> Target;

			operator bool() { return *Root.Get() == "/"; }
		};

		class Ext2PathParser
		{
		public:
			Ext2PathParser(const Std::UniquePtr<Std::StaticString>& path);
			Std::UniquePtr<Ext2Path> Parse();

		private:
			Std::UniquePtr<Std::StaticString> m_Path;
		};
	}
}
