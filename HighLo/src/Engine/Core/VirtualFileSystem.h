// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2021-10-17) Refactored VirtualFileSystem to be a Singleton class
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/DataTypes/Hashmap.h"
#include "Engine/Core/DataTypes/String.h"

namespace highlo
{
	class VirtualFileSystem : public Singleton<VirtualFileSystem>
	{
	public:

		HLAPI void Mount(const HLString &virtualPath, const HLString &physicalPath);
		HLAPI void Unmount(const HLString &path);

		HLAPI bool ResolvePhysicalPath(HLString &path, HLString &outPath);

		HLAPI Byte *ReadFile(const HLString &path, int64 *outSize);
		HLAPI HLString ReadTextFile(const HLString &path);
		
		HLAPI bool WriteFile(const HLString &path, Byte *buffer, int64 size);
		HLAPI bool WriteTextFile(const HLString &path, const HLString &text);

		HLAPI bool RemoveFile(const HLString &path);
		HLAPI int64 GetFileSize(const HLString &path);
		
		HLAPI bool FileExists(const HLString &path);
		HLAPI bool PathExists(const HLString &path);
		
		HLAPI bool CreateFolder(const HLString &path);
		HLAPI bool RemoveFolder(const HLString &path);

		HLAPI void OpenInExplorer(const HLString &path);
		HLAPI void OpenInBrowser(const HLString &url);

		HLAPI HLString GetAbsoluteFilePath(const HLString &path);

	private:

		HLHashmap<HLString, std::vector<HLString>> m_MountPoints;
	};
}
