#include <Kernel/FileSystem/Ext2/PathParser.h>
#include <Al/CString.h>

namespace Kernel
{
	namespace FileSystem
	{
		Ext2PathParser::Ext2PathParser(const Al::UniquePtr<Al::StaticString>& path)
			: m_Path(path->Clone())
		{
		}

		Al::UniquePtr<Ext2Path> Ext2PathParser::Parse()
		{
			Al::UniquePtr<Ext2Path> parsed = Al::MakeUnique<Ext2Path>();
			parsed->Path = Al::StaticString::Create(m_Path->Data());

			char* saveptr = NULL;
			auto tmpPath = m_Path->Clone();
			char* token = Al::Strtok_r((char*)tmpPath->Data(), "/", &saveptr);
			while (token)
			{
				parsed->Parts.PushBack(Al::StaticString::Create(token));
				token = Al::Strtok_r(NULL, "/", &saveptr);
			}

			parsed->Root = Al::StaticString::Create(m_Path->Data(), 1);
			parsed->Target = Al::StaticString::Create((parsed->Parts.Size() ? parsed->Parts[parsed->Parts.Size() - 1]->Data() : ""));
			parsed->Parts.PopBack();
			return parsed;
		}
	}
}
