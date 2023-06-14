// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VirtualFileSystem.h"

#include "Engine/Core/FileSystem.h"

namespace highlo
{
	void VirtualFileSystem::Mount(const HLString &virtualPath, const HLString &physicalPath)
	{
		if (m_MountPoints.HasKey(virtualPath))
		{
			m_MountPoints[virtualPath].push_back(physicalPath);
		}
		else
		{
			std::vector<HLString> res;
			res.push_back(physicalPath);
			m_MountPoints.Put(virtualPath, res);
		}
		}

	void VirtualFileSystem::Unmount(const HLString &path)
	{
		m_MountPoints[path].clear();
	}

	bool VirtualFileSystem::ResolvePhysicalPath(HLString &path, HLString &outPath)
	{
		if (path.IsEmpty())
			return false;

		if (path[0] != '/')
		{
			outPath = path;
			return FileSystem::Get()->FileExists(path);
		}

		path = path.Substr(1);
		std::vector<HLString> dirs = path.Split('/');
		const HLString &virtualDir = dirs.front();
		const HLString &fileName = dirs.back();

		if (!m_MountPoints.HasKey(virtualDir) || m_MountPoints.IsEmpty())
			return false;

		for (const HLString &physicalPath : m_MountPoints[virtualDir])
		{
			HLString p = physicalPath + fileName;
			if (FileSystem::Get()->FileExists(p) || FileSystem::Get()->FolderExists(physicalPath))
			{
				outPath = p;
				return true;
			}
		}

		return false;
	}

	Byte *VirtualFileSystem::ReadFile(const HLString &path, int64 *outSize)
	{
		HLString physicalPath;
		return ResolvePhysicalPath(HLString(path), physicalPath) ? FileSystem::Get()->ReadFile(physicalPath, outSize) : nullptr;
	}

	HLString VirtualFileSystem::ReadTextFile(const HLString &path)
	{
		HLString physicalPath;
		return ResolvePhysicalPath(HLString(path), physicalPath) ? FileSystem::Get()->ReadTextFile(physicalPath) : HLString();
	}

	bool VirtualFileSystem::WriteFile(const HLString &path, Byte *buffer, int64 size)
	{
		HLString physicalPath;
		return ResolvePhysicalPath(HLString(path), physicalPath) ? FileSystem::Get()->WriteFile(physicalPath, buffer, size) : false;
	}

	bool VirtualFileSystem::WriteTextFile(const HLString &path, const HLString &text)
	{
		HLString physicalPath;
		return ResolvePhysicalPath(HLString(path), physicalPath) ? FileSystem::Get()->WriteTextFile(physicalPath, text) : false;
	}

	bool VirtualFileSystem::RemoveFile(const HLString &path)
	{
		HLString physicalPath;
		return ResolvePhysicalPath(HLString(path), physicalPath) ? FileSystem::Get()->RemoveFile(physicalPath) : false;
	}

	int64 VirtualFileSystem::GetFileSize(const HLString &path)
	{
		HLString physicalPath;
		return ResolvePhysicalPath(HLString(path), physicalPath) ? FileSystem::Get()->GetFileSize(physicalPath) : -1;
	}

	bool VirtualFileSystem::FileExists(const HLString &path)
	{
		HLString physicalPath;
		return ResolvePhysicalPath(HLString(path), physicalPath) ? FileSystem::Get()->FileExists(physicalPath) : false;
	}

	bool VirtualFileSystem::PathExists(const HLString &path)
	{
		HLString physicalPath;
		return ResolvePhysicalPath(HLString(path), physicalPath) ? FileSystem::Get()->FolderExists(physicalPath) : false;
	}

	bool VirtualFileSystem::CreateFolder(const HLString &path)
	{
		HLString physicalPath;
		return ResolvePhysicalPath(HLString(path), physicalPath) ? FileSystem::Get()->CreateFolder(physicalPath) : false;
	}

	bool VirtualFileSystem::RemoveFolder(const HLString &path)
	{
		HLString physicalPath;
		return ResolvePhysicalPath(HLString(path), physicalPath) ? FileSystem::Get()->RemoveFolder(physicalPath) : false;
	}

	void VirtualFileSystem::OpenInExplorer(const HLString &path)
	{
		FileSystem::Get()->OpenInExplorer(path);
	}

	void VirtualFileSystem::OpenInBrowser(const HLString &url)
	{
		FileSystem::Get()->OpenInBrowser(url);
	}

	HLString VirtualFileSystem::GetAbsoluteFilePath(const HLString &path)
	{
		HLString physicalPath;
		HLString p = path;
		ResolvePhysicalPath(p, physicalPath);
		return physicalPath;
	}
}
