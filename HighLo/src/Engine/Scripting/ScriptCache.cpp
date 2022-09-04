// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ScriptCache.h"

namespace highlo
{
	struct Cache
	{

	};

	static Cache *s_ScriptingCache = nullptr;

	void ScriptCache::Init()
	{
		s_ScriptingCache = new Cache();
	}

	void ScriptCache::Shutdown()
	{
		ClearCache();

		delete s_ScriptingCache;
		s_ScriptingCache = nullptr;
	}

	void ScriptCache::ClearCache()
	{

	}

	void ScriptCache::GenerateCacheForAssembly(Ref<AssemblyInfo> &assemblyInfo)
	{
		HL_CORE_TRACE("Generating Cache for {}", **assemblyInfo->FilePath);
	}
}

