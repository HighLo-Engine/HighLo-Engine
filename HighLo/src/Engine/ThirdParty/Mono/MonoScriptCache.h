// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-09-05) initial release
//

#pragma once

#include "Engine/Scripting/AssemblyInfo.h"

#ifdef HIGHLO_API_MONO_SCRIPTING

namespace highlo
{
	class MonoScriptCache
	{
	public:

		HLAPI static void Init();
		HLAPI static void Shutdown();

		HLAPI static void ClearCache();

		HLAPI static void GenerateCacheForAssembly(Ref<AssemblyInfo> &assemblyInfo);
	};
}

#endif // HIGHLO_API_MONO_SCRIPTING

