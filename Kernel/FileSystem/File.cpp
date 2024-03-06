#include <Kernel/FileSystem/File.h>

namespace Kernel
{
	namespace FileSystem
	{
		File::File(const Al::SharedPtr<KFileSystem>& fileSystem, Al::StringView filepath)
			: m_FileSystem(fileSystem), m_FilePath(Al::StaticString::Create(filepath))
		{
		}

		int32_t File::Read(void* buf, size_t len, size_t offset)
		{
			return m_FileSystem->Read(Al::SharedPtr<File>(this), buf, len, offset);
		}

		int32_t File::Write(const void* buf, size_t len, size_t offset)
		{
			return m_FileSystem->Write(Al::SharedPtr<File>(this), buf, len, offset);
		}

		Al::SharedPtr<KFileSystem> File::GetFileSystem() const
		{
			return m_FileSystem;
		}

		Al::StringView File::GetFilePath() const
		{
			return m_FilePath->View();
		}
	}
}
