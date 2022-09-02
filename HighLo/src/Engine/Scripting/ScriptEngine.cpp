// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ScriptEngine.h"

#include "ScriptCache.h"

#ifdef HIGHLO_API_MONO_SCRIPTING
#include "Engine/ThirdParty/Mono/MonoAPI.h"
#elif HIGHLO_API_LUA_SCRIPTING
#endif // HIGHLO_API_MONO

namespace highlo
{
#ifdef HIGHLO_API_MONO_SCRIPTING
	UniqueRef<ScriptingAPI> ScriptEngine::s_ScriptingAPI = UniqueRef<MonoAPI>::Create();
#elif HIGHLO_API_LUA_SCRIPTING
	UniqueRef<ScriptingAPI> ScriptEngine::s_ScriptingAPI = UniqueRef<LuaAPI>::Create();
#endif // HIGHLO_API_MONO

	void ScriptEngine::Init(const ScriptEngineConfig *config)
	{
		ScriptCache::Init();
		s_ScriptingAPI->Init();
	}

	void ScriptEngine::Shutdown()
	{
		s_ScriptingAPI->Shutdown();
		ScriptCache::Shutdown();
	}
}

