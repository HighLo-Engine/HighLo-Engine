// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
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
	class FileSystem
	{
	public:

		HLAPI static bool FileExists(const FileSystemPath &path);
		HLAPI static bool PathExists(const FileSystemPath &path);
		
		HLAPI static bool RemoveFile(const FileSystemPath &path);
		HLAPI static HLString Rename(const FileSystemPath &path, const FileSystemPath &newName);
		HLAPI static bool Move(const FileSystemPath &filePath, const FileSystemPath &dest);
		HLAPI static int64 GetFileSize(const FileSystemPath &path);
		
		HLAPI static bool CreateFolder(const FileSystemPath &path);
		HLAPI static bool RemoveFolder(const FileSystemPath &path);
		
		HLAPI static Byte *ReadFile(const FileSystemPath &path, int64 *outSize);
		HLAPI static HLString ReadTextFile(const FileSystemPath &path);
		
		HLAPI static bool WriteFile(const FileSystemPath &path, Byte *buffer, int64 size);
		HLAPI static bool WriteTextFile(const FileSystemPath &path, const HLString &text);

		HLAPI static void OpenInExplorer(const FileSystemPath &path);
		HLAPI static void OpenInBrowser(const HLString &url);

		HLAPI static bool HasEnvironmentVariable(const HLString &key);
		HLAPI static bool SetEnvironmentVariable(const HLString &key, const HLString &value);
		HLAPI static HLString GetEnvironmentVariable(const HLString &key);
	};
}

