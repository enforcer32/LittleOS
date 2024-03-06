#include <Kernel/FileSystem/MountManager.h>

namespace Kernel
{
	namespace FileSystem
	{
		MountManager::MountManager()
		{
			auto root = Al::MakeShared<MountInfo>();
			root->Location = Al::StaticString::Create("/");
			root->FileSystem = nullptr;
			m_Mounts.PushBack(root);
		}

		void MountManager::AddMount(Al::StringView location, const Al::SharedPtr<KFileSystem>& fileSystem)
		{
			m_Mounts.PushBack(Al::MakeShared<MountInfo>(Al::StaticString::Create(location), fileSystem));
		}

		void MountManager::AddMount(const MountInfo& mount)
		{
			m_Mounts.PushBack(Al::MakeShared<MountInfo>(mount));
		}

		void MountManager::RemoveMount(Al::StringView location)
		{
			for (size_t i = 0; i < m_Mounts.Size(); i++)
			{
				if (m_Mounts[i]->Location->View() == location)
				{
					m_Mounts[i]->FileSystem = nullptr;
					m_Mounts[i]->Location = nullptr;
				}
			}
		}

		void MountManager::ReMountFileSystem(Al::StringView location, const Al::SharedPtr<KFileSystem>& fileSystem)
		{
			for (size_t i = 0; i < m_Mounts.Size(); i++)
				if (m_Mounts[i]->Location->View() == location)
					m_Mounts[i]->FileSystem = fileSystem;
		}

		Al::SharedPtr<MountInfo> MountManager::GetMount(Al::StringView location)
		{
			for (size_t i = 0; i < m_Mounts.Size(); i++)
				if (m_Mounts[i]->Location->View() == location)
					return m_Mounts[i];
			return nullptr;
		}

		Al::SharedPtr<KFileSystem> MountManager::GetMountFileSystem(Al::StringView location)
		{
			for (size_t i = 0; i < m_Mounts.Size(); i++)
				if (m_Mounts[i]->Location->View() == location)
					return m_Mounts[i]->FileSystem;
			if (IsRootMounted())
				return m_Mounts[0]->FileSystem; // Bug?
			return nullptr;
		}

		bool MountManager::HasMount(Al::StringView location) const
		{
			for (size_t i = 0; i < m_Mounts.Size(); i++)
				if (m_Mounts[i]->Location->View() == location)
					return true;
			return false;
		}

		void MountManager::MountRoot(const Al::SharedPtr<KFileSystem>& fileSystem)
		{
			m_Mounts[0]->FileSystem = fileSystem;
		}

		bool MountManager::IsRootMounted() const
		{
			return m_Mounts[0]->IsMounted();
		}
	}
}
