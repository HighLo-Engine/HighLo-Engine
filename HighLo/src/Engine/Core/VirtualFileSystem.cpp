// Copyright (c) 2021 Albert Slepak and Can Karka. All rights reserved.

#include "HighLoPch.h"
#include "VirtualFileSystem.h"

#include "Engine/Core/FileSystem.h"

namespace highlo
{
	VirtualFileSystem *VirtualFileSystem::s_Instance = nullptr;

	void VirtualFileSystem::Init()
	{
		s_Instance = new VirtualFileSystem();
	}

	void VirtualFileSystem::Shutdown()
	{
		delete s_Instance;
	}

	void VirtualFileSystem::Mount(const HLString &virtualPath, const HLString &physicalPath)
	{
		HL_ASSERT(s_Instance, "FileSystem not created!");

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
		HL_ASSERT(s_Instance, "FileSystem not created!");
		m_MountPoints[path].clear();
	}

	bool VirtualFileSystem::ResolvePhysicalPath(HLString &path, HLString &outPath)
	{
		if (path.IsEmpty())
			return false;

		if (path[0] != '/')
		{
			outPath = path;
			return FileSystem::FileExists(path);
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
			if (FileSystem::FileExists(p) || FileSystem::PathExists(physicalPath))
			{
				outPath = p;
				return true;
			}
		}

		return false;
	}

	Byte *VirtualFileSystem::ReadFile(const HLString &path, int64 *outSize)
	{
		HL_ASSERT(s_Instance, "FileSystem not created!");
		HLString physicalPath;
		return ResolvePhysicalPath(HLString(path), physicalPath) ? FileSystem::ReadFile(physicalPath, outSize) : nullptr;
	}

	HLString VirtualFileSystem::ReadTextFile(const HLString &path)
	{
		HL_ASSERT(s_Instance, "FileSystem not created!");
		HLString physicalPath;
		return ResolvePhysicalPath(HLString(path), physicalPath) ? FileSystem::ReadTextFile(physicalPath) : HLString();
	}

	HLString VirtualFileSystem::GetFileNameFromPath(const HLString &path)
	{
		HLString result;
		int32 pos = path.FirstIndexOf('/');
		int32 i = 1;
		while (pos != HLString::NPOS)
		{
			result = path.Substr(pos + 1);
			pos = path.FirstIndexOf('/', i);
			++i;
		}

		if (result.Contains("."))
			result = result.Substr(0, result.IndexOf("."));

		return result;
	}

	HLString VirtualFileSystem::GetFileExtension(const HLString &path)
	{
		HLString result;
		int32 pos = path.FirstIndexOf('/');
		int32 i = 1;
		while (pos != HLString::NPOS)
		{
			result = path.Substr(pos + 1);
			pos = path.FirstIndexOf('/', i);
			++i;
		}

		if (!result.Contains("."))
			return "-1";

		result = result.Substr(result.IndexOf(".") + 1);
		return result;
	}

	bool VirtualFileSystem::WriteFile(const HLString &path, Byte *buffer, int64 size)
	{
		HL_ASSERT(s_Instance, "FileSystem not created!");
		HLString physicalPath;
		return ResolvePhysicalPath(HLString(path), physicalPath) ? FileSystem::WriteFile(physicalPath, buffer, size) : false;
	}

	bool VirtualFileSystem::WriteTextFile(const HLString &path, const HLString &text)
	{
		HL_ASSERT(s_Instance, "FileSystem not created!");
		HLString physicalPath;
		return ResolvePhysicalPath(HLString(path), physicalPath) ? FileSystem::WriteTextFile(physicalPath, text) : false;
	}

	bool VirtualFileSystem::RemoveFile(const HLString &path)
	{
		HL_ASSERT(s_Instance, "FileSystem not created!");
		HLString physicalPath;
		return ResolvePhysicalPath(HLString(path), physicalPath) ? FileSystem::RemoveFile(physicalPath) : false;
	}

	int64 VirtualFileSystem::GetFileSize(const HLString &path)
	{
		HL_ASSERT(s_Instance, "FileSystem not created!");
		HLString physicalPath;
		return ResolvePhysicalPath(HLString(path), physicalPath) ? FileSystem::GetFileSize(physicalPath) : -1;
	}

	bool VirtualFileSystem::FileExists(const HLString &path)
	{
		HL_ASSERT(s_Instance, "FileSystem not created!");
		HLString physicalPath;
		return ResolvePhysicalPath(HLString(path), physicalPath) ? FileSystem::FileExists(physicalPath) : false;
	}

	bool VirtualFileSystem::PathExists(const HLString &path)
	{
		HL_ASSERT(s_Instance, "FileSystem not created!");
		HLString physicalPath;
		return ResolvePhysicalPath(HLString(path), physicalPath) ? FileSystem::PathExists(physicalPath) : false;
	}

	bool VirtualFileSystem::CreateFolder(const HLString &path)
	{
		HL_ASSERT(s_Instance, "FileSystem not created!");
		HLString physicalPath;
		return ResolvePhysicalPath(HLString(path), physicalPath) ? FileSystem::CreateFolder(physicalPath) : false;
	}

	bool VirtualFileSystem::RemoveFolder(const HLString &path)
	{
		HL_ASSERT(s_Instance, "FileSystem not created!");
		HLString physicalPath;
		return ResolvePhysicalPath(HLString(path), physicalPath) ? FileSystem::RemoveFolder(physicalPath) : false;
	}

	void VirtualFileSystem::OpenInExplorer(const HLString &path)
	{
		FileSystem::OpenInExplorer(path);
	}

	void VirtualFileSystem::OpenInBrowser(const HLString &url)
	{
		FileSystem::OpenInBrowser(url);
	}

	HLString VirtualFileSystem::GetAbsoluteFilePath(const HLString &path)
	{
		HLString physicalPath;
		HLString p = path;
		ResolvePhysicalPath(p, physicalPath);
		return physicalPath;
	}

	VirtualFileSystem *VirtualFileSystem::Get()
	{
		return s_Instance;
	}
}