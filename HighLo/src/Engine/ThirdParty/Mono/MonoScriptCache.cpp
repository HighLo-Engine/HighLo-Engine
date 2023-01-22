// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MonoScriptCache.h"

#ifdef HIGHLO_API_MONO_SCRIPTING

#include <mono/metadata/assembly.h>
#include <mono/metadata/class.h>
#include <mono/metadata/object.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/tokentype.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/appdomain.h>

namespace highlo
{
	struct Cache
	{

	};

	static Cache *s_ScriptingCache = nullptr;

	void MonoScriptCache::Init()
	{
		s_ScriptingCache = new Cache();
	}

	void MonoScriptCache::Shutdown()
	{
		ClearCache();

		delete s_ScriptingCache;
		s_ScriptingCache = nullptr;
	}
	
	void MonoScriptCache::ClearCache()
	{
	}
	
	void MonoScriptCache::GenerateCacheForAssembly(Ref<AssemblyInfo> &assemblyInfo)
	{
		HL_CORE_TRACE("Generating Cache for {}", **assemblyInfo->FilePath);
	}
}

#endif // HIGHLO_API_MONO_SCRIPTING

