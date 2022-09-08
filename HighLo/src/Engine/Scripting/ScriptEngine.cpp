// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ScriptEngine.h"

#include "Engine/Scene/Project.h"

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

	void ScriptEngine::Init(ScriptEngineConfig *config)
	{
		// TODO: Use the real project name
		HLString projectName = "DemoProject";
		FileSystemPath appPath = FindClientAssembly(projectName);
		FileSystemPath appPdbPath = FindClientDebugSymbols(projectName);

		if (config)
		{
			config->AppAssemblyPath = appPath;
		}

		s_ScriptingAPI->Init(config);
	}

	void ScriptEngine::Shutdown()
	{
		s_ScriptingAPI->Shutdown();
	}

	ScriptEngineConfig &ScriptEngine::GetConfig()
	{
		return s_ScriptingAPI->GetConfig();
	}
	
	FileSystemPath ScriptEngine::FindClientAssembly(const HLString &projectName)
	{
		FileSystemPath result = "";
		FileSystemPath searchPath = "Resources/Scripts/";
		std::vector<File> possibleDLLs = searchPath.FilterFileListByExtension("dll");

		for (File &file : possibleDLLs)
		{
			if (file.FileName == projectName)
			{
				result = file;
				break;
			}
		}

		return result;
	}
	
	FileSystemPath ScriptEngine::FindClientDebugSymbols(const HLString &projectName)
	{
		FileSystemPath result = "";
		FileSystemPath searchPath = "Resources/Scripts/";
		std::vector<File> possiblePDBs = searchPath.FilterFileListByExtension("pdb");

		for (File &file : possiblePDBs)
		{
			if (file.FileName == projectName)
			{
				result = file;
				break;
			}
		}

		return result;
	}
}

