// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ScriptCache.h"

namespace highlo
{
	void ScriptCache::Init()
	{
	}

	void ScriptCache::Shutdown()
	{
	}

	void ScriptCache::GenerateCacheForAssembly(Ref<AssemblyInfo> &assemblyInfo)
	{
		HL_CORE_TRACE("Generating Cache for {}", **assemblyInfo->FilePath);
	}
}

