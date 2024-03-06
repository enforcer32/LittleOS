#pragma once

#include <Kernel/FileSystem/FileSystem.h>
#include <Al/Memory.h>
#include <Al/StringView.h>
#include <Al/StaticString.h>

namespace Kernel
{
	namespace FileSystem
	{
		enum class FileMode
		{
			OtherExecute = 0x001,
			OtherWrite = 0x002,
			OtherRead = 0x004,
			GroupExecute = 0x008,
			GroupWrite = 0x010,
			GroupRead = 0x020,
			UserExecute = 0x040,
			UserWrite = 0x080,
			UserRead = 0x100,
		};

		class File
		{
		public:
			File(const Al::SharedPtr<KFileSystem>& fileSystem, Al::StringView filepath);
			int32_t Read(void* buf, size_t len, size_t offset);
			int32_t Write(const void* buf, size_t len, size_t offset);

			Al::SharedPtr<KFileSystem> GetFileSystem() const;
			Al::StringView GetFilePath() const;

		private:
			Al::SharedPtr<KFileSystem> m_FileSystem;
			Al::UniquePtr<Al::StaticString> m_FilePath;
		};
	}
}
