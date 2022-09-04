// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-07-14) initial release
//

#pragma once

#include "AssemblyInfo.h"

namespace highlo
{
	class ScriptCache
	{
	public:

		HLAPI static void Init();
		HLAPI static void Shutdown();

		HLAPI static void ClearCache();

		HLAPI static void GenerateCacheForAssembly(Ref<AssemblyInfo> &assemblyInfo);
	};
}

