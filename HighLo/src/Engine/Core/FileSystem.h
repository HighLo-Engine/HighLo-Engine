// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.3 (2021-10-17) Refactored FileSystem to be a Singleton class
//     - 1.2 (2021-10-04) Refactored FileSystem: Excluded FileSystemWatcher functions to own class
//     - 1.1 (2021-09-15) Added HasEnvironmentVariable, SetEnvironmentVariable, GetEnvironmentVariable
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/DataTypes/String.h"
#include "Engine/Events/ApplicationEvents.h"
#include "FileSystemPath.h"

// disable Windows function
#undef SetEnvironmentVariable
#undef GetEnvironmentVariable

namespace highlo
{
	class FileSystem : public Singleton<FileSystem>
	{
	public:

		HLAPI bool FileExists(const FileSystemPath &path);
		HLAPI bool PathExists(const FileSystemPath &path);
		
		HLAPI bool RemoveFile(const FileSystemPath &path);
		HLAPI HLString Rename(const FileSystemPath &path, const FileSystemPath &newName);
		HLAPI bool Move(const FileSystemPath &filePath, const FileSystemPath &dest);
		HLAPI int64 GetFileSize(const FileSystemPath &path);
		
		HLAPI bool CreateFolder(const FileSystemPath &path);
		HLAPI bool RemoveFolder(const FileSystemPath &path);
		
		HLAPI Byte *ReadFile(const FileSystemPath &path, int64 *outSize);
		HLAPI HLString ReadTextFile(const FileSystemPath &path);
		
		HLAPI bool WriteFile(const FileSystemPath &path, Byte *buffer, int64 size);
		HLAPI bool WriteTextFile(const FileSystemPath &path, const HLString &text);

		HLAPI void OpenInExplorer(const FileSystemPath &path);
		HLAPI void OpenInBrowser(const HLString &url);

		HLAPI bool HasEnvironmentVariable(const HLString &key);
		HLAPI bool SetEnvironmentVariable(const HLString &key, const HLString &value);
		HLAPI HLString GetEnvironmentVariable(const HLString &key);
	};
}

