// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-15) Added HasEnvironmentVariable, SetEnvironmentVariable, GetEnvironmentVariable
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/DataTypes/String.h"
#include "Engine/Events/ApplicationEvents.h"

// disable Windows function
#undef SetEnvironmentVariable
#undef GetEnvironmentVariable

namespace highlo
{
	class FileSystem
	{
	public:

		using FileSystemChangedCallbackFn = std::function<void(FileSystemChangedEvent)>;

		HLAPI static bool FileExists(const HLString &path);
		HLAPI static bool PathExists(const HLString &path);
		
		HLAPI static bool RemoveFile(const HLString &path);
		HLAPI static HLString Rename(const HLString &path, const HLString &newName);
		HLAPI static bool Move(const HLString &filePath, const HLString &dest);
		HLAPI static int64 GetFileSize(const HLString &path);
		
		HLAPI static bool CreateFolder(const HLString &path);
		HLAPI static bool RemoveFolder(const HLString &path);
		
		HLAPI static Byte *ReadFile(const HLString &path, int64 *outSize);
		HLAPI static HLString ReadTextFile(const HLString &path);
		
		HLAPI static bool WriteFile(const HLString &path, Byte *buffer, int64 size);
		HLAPI static bool WriteTextFile(const HLString &path, const HLString &text);

		HLAPI static void OpenInExplorer(const HLString &path);
		HLAPI static void OpenInBrowser(const HLString &url);
		HLAPI static void SetChangeCallback(const FileSystemChangedCallbackFn &callback);
		
		HLAPI static void StartWatching();
		HLAPI static void StopWatching();

		HLAPI static bool HasEnvironmentVariable(const HLString &key);
		HLAPI static bool SetEnvironmentVariable(const HLString &key, const HLString &value);
		HLAPI static HLString GetEnvironmentVariable(const HLString &key);

	private:

		static unsigned long Watch(void *param);

		static FileSystemChangedCallbackFn s_Callback;
	};
}
