// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-10-04) initial release
//

#pragma once

#include "Engine/Events/ApplicationEvents.h"

namespace highlo
{
	class FileSystemWatcher : public Singleton<FileSystemWatcher>
	{
	public:

		using FileSystemChangedCallbackFn = std::function<void(FileSystemChangedEvent)>;

		HLAPI void Start(const HLString &filePath);
		HLAPI void Stop();

		HLAPI void SetChangeCallback(const FileSystemChangedCallbackFn &callback);
		HLAPI void SetWatchPath(const HLString &filePath);
		HLAPI void DisableWatchUntilNextAction();

	private:

		static ULONG Watch(void *param);
		static FileSystemChangedCallbackFn s_Callback;
		
	};
}

