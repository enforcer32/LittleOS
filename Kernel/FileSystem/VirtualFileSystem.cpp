#include <Kernel/FileSystem/VirtualFileSystem.h>
#include <Kernel/FileSystem/Ext2/FileSystem.h>
#include <Kernel/FileSystem/Disk.h>
#include <Kernel/Lib/KPrintf.h>
#include <Kernel/Lib/KPanic.h>

namespace Kernel
{
	namespace FileSystem
	{
		MountManager* VirtualFileSystem::s_MountManager = nullptr;

		int32_t VirtualFileSystem::Init()
		{
			s_MountManager = new MountManager;

			auto ata = Al::MakeUnique<Drivers::ATA>();
			ata->Init(Drivers::ATABus::Primary, Drivers::ATADrive::Slave);
			Al::UniquePtr<Disk> disk = Al::MakeUnique<Disk>();
			disk->Init(Al::Move(ata));

			Al::SharedPtr<KFileSystem> ext2fs((KFileSystem*)(new Ext2FileSystem));
			if (ext2fs->Init(Al::Move(disk)) != 0)
				KPanic("Failed to Initialize Ext2FileSystem\n");

			MountRoot(ext2fs);
			KPrintf("VirtualFileSystem: Mounted Ext2FileSystem as RootFS!\n");
			return 0;
		}

		void VirtualFileSystem::MountRoot(const Al::SharedPtr<KFileSystem>& fileSystem)
		{
			s_MountManager->MountRoot(fileSystem);
		}

		void VirtualFileSystem::Mount(const MountInfo& mount)
		{
			s_MountManager->AddMount(mount);
		}

		void VirtualFileSystem::UnMount(Al::StringView location)
		{
			s_MountManager->RemoveMount(location);
		}

		Al::SharedPtr<MountInfo> VirtualFileSystem::GetMount(Al::StringView location)
		{
			return s_MountManager->GetMount(location);
		}

		Al::SharedPtr<KFileSystem> VirtualFileSystem::GetMountFileSystem(Al::StringView location)
		{
			return s_MountManager->GetMountFileSystem(location);
		}

		Al::SharedPtr<File> VirtualFileSystem::Open(Al::StringView path)
		{
			Al::StringView root = path.Substr(0, path.Find('/', 1));
			auto fileSystem = GetMountFileSystem(root);
			if (!fileSystem)
			{
				KPrintf("VirtualFileSystem: No Mounted FileSystem for: %s\n", path.Data());
				return nullptr;
			}

			return fileSystem->Open(path);
		}
	}
}
