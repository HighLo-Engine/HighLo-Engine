// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

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

		HLAPI void Start(const HLString &filePath);
		HLAPI void Stop();

		HLAPI void SetWatchPath(const HLString &filePath);
		HLAPI void DisableWatchUntilNextAction();

	private:

		static ULONG Watch(void *param);
		
	};
}

