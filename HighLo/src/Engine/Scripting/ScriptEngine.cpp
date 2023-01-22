// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ScriptEngine.h"

#include <stack>

#include "Engine/Scene/Project.h"
#include "Engine/Assets/AssetManager.h"
#include "ScriptAsset.h"

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

	struct ScriptEngineData
	{
		Ref<Scene> SceneContext = nullptr;
		ScriptEntityMap EntityInstances;
		std::stack<Entity> RuntimeDuplicatedScriptEntities;
	};

	static ScriptEngineData *s_ScriptEngineData = nullptr;

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

		s_ScriptEngineData = new ScriptEngineData();
	}

	void ScriptEngine::Shutdown()
	{
		delete s_ScriptEngineData;
		s_ScriptEngineData = nullptr;

		s_ScriptingAPI->Shutdown();
	}

	void ScriptEngine::InitializeRuntime()
	{
	}

	void ScriptEngine::ShutdownRuntime()
	{
	}

	void ScriptEngine::LoadAppAssembly()
	{
	}

	void ScriptEngine::ReloadAppAssembly(const FileSystemPath &filePath)
	{
	}

	void ScriptEngine::UnloadAppAssembly()
	{
	}

	void ScriptEngine::SetSceneContext(const Ref<Scene> &scene)
	{
		HL_ASSERT(scene);
		s_ScriptEngineData->SceneContext = scene;
	}

	Ref<Scene> &ScriptEngine::GetSceneContext()
	{
		return s_ScriptEngineData->SceneContext;
	}

	void ScriptEngine::InitializeScriptEntity(Entity &e)
	{
	}

	void ScriptEngine::InitializeScriptEntityForRuntime(Entity &e)
	{
	}

	void ScriptEngine::DuplicateScriptInstance(Entity &e, Entity &targetEntity)
	{
	}

	void ScriptEngine::ShutdownScriptEntity(Entity &e, bool destroyInstance)
	{
	}

	bool ScriptEngine::IsEntityInstantiated(Entity &e, bool checkOnCreateCalled)
	{
		// TODO
		return false;
	}

	void *ScriptEngine::GetEntityInstanceOfScene(UUID sceneID, UUID entityID)
	{
		// TODO
		return nullptr;
	}

	const ScriptEntityMap &ScriptEngine::GetScriptSceneMap()
	{
		return s_ScriptEngineData->EntityInstances;
	}

	const std::unordered_map<UUID, void *> &ScriptEngine::GetEntityInstances(UUID sceneID)
	{
		HL_ASSERT(s_ScriptEngineData->EntityInstances.find(sceneID) != s_ScriptEngineData->EntityInstances.end());
		return s_ScriptEngineData->EntityInstances.at(sceneID);
	}

	uint32 ScriptEngine::GetScriptClassIDFromComponent(const ScriptComponent &scriptComp)
	{
		if (!AssetManager::Get()->IsAssetHandleValid(scriptComp.ScriptClass))
			return false;

		MonoAssembly *loadedAssembly = ScriptEngine::GetAppAssemblyInfo()->Assembly;
		if (!loadedAssembly)
			return false;

		Ref<ScriptAsset> scriptAsset = AssetManager::Get()->GetAsset<ScriptAsset>(scriptComp.ScriptClass);
		return scriptAsset->GetClassID();
	}

	bool ScriptEngine::IsModuleValid(AssetHandle scriptAssetHandle)
	{
		if (!AssetManager::Get()->IsAssetHandleValid(scriptAssetHandle))
			return false;

		MonoAssembly *loadedAssembly = ScriptEngine::GetAppAssemblyInfo()->Assembly;
		if (!loadedAssembly)
			return false;

		Ref<ScriptAsset> scriptAsset = AssetManager::Get()->GetAsset<ScriptAsset>(scriptAssetHandle);
		
		// TODO
		return false;
	}

	const Ref<AssemblyInfo> &ScriptEngine::GetCoreAssemblyInfo()
	{
		return s_ScriptingAPI->GetCoreAssemblyInfo();
	}

	const Ref<AssemblyInfo> &ScriptEngine::GetAppAssemblyInfo()
	{
		return s_ScriptingAPI->GetAppAssemblyInfo();
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

