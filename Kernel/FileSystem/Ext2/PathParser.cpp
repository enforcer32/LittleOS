#include <Kernel/FileSystem/Ext2/PathParser.h>
#include <Standard/CString.h>

namespace Kernel
{
	namespace FileSystem
	{
		Ext2PathParser::Ext2PathParser(const Std::UniquePtr<Std::StaticString>& path)
			: m_Path(path->Clone())
		{
		}

		Std::UniquePtr<Ext2Path> Ext2PathParser::Parse()
		{
			Std::UniquePtr<Ext2Path> parsed = Std::MakeUnique<Ext2Path>();
			parsed->Path = Std::StaticString::Create(m_Path->Data());

			char* saveptr = NULL;
			auto tmpPath = m_Path->Clone();
			char* token = Std::Strtok_r((char*)tmpPath->Data(), "/", &saveptr);
			while (token)
			{
				parsed->Parts.PushBack(Std::StaticString::Create(token));
				token = Std::Strtok_r(NULL, "/", &saveptr);
			}

			parsed->Root = Std::StaticString::Create(m_Path->Data(), 1);
			parsed->Target = Std::StaticString::Create((parsed->Parts.Size() ? parsed->Parts[parsed->Parts.Size() - 1]->Data() : ""));
			parsed->Parts.PopBack();
			return parsed;
		}
	}
}
