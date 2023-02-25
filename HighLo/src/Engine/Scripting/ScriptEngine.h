// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-07-14) initial release
//

#pragma once

#include "ScriptingAPI.h"
#include "ScriptingConfig.h"

#include "Engine/Scene/Scene.h"

namespace highlo
{
	using ScriptEntityMap = std::unordered_map<UUID, std::unordered_map<UUID, void*>>;

	/// <summary>
	/// The script engine is the global connector between the scripting module and the rest of the engine.
	/// </summary>
	class ScriptEngine
	{
	public:

		HLAPI static void Init(ScriptEngineConfig *config = nullptr);
		HLAPI static void Shutdown();

		HLAPI static void InitializeRuntime();
		HLAPI static void ShutdownRuntime();

		HLAPI static void LoadAppAssembly();
		HLAPI static void ReloadAppAssembly(const FileSystemPath &filePath);
		HLAPI static void UnloadAppAssembly();

		HLAPI static void SetSceneContext(const Ref<Scene> &scene);
		HLAPI static Ref<Scene> &GetSceneContext();

		HLAPI static void InitializeScriptEntity(Entity &e);
		HLAPI static void InitializeScriptEntityForRuntime(Entity &e);
		HLAPI static void DuplicateScriptInstance(Entity &e, Entity &targetEntity);
		HLAPI static void ShutdownScriptEntity(Entity &e, bool destroyInstance = true);

		HLAPI static bool IsEntityInstantiated(Entity &e, bool checkOnCreateCalled = true);
		HLAPI static void *GetEntityInstanceOfScene(UUID sceneID, UUID entityID);
		HLAPI static const ScriptEntityMap &GetScriptSceneMap();
		HLAPI static const std::unordered_map<UUID, void*> &GetEntityInstances(UUID sceneID);

		HLAPI static uint32 GetScriptClassIDFromComponent(const ScriptComponent &scriptComp);
		HLAPI static bool IsModuleValid(AssetHandle scriptAssetHandle);



		HLAPI static const Ref<AssemblyInfo> &GetCoreAssemblyInfo();
		HLAPI static const Ref<AssemblyInfo> &GetAppAssemblyInfo();

		HLAPI static ScriptEngineConfig &GetConfig();

	private:

		/// <summary>
		/// Finds the path to the clients DLL file.
		/// </summary>
		/// <returns></returns>
		static FileSystemPath FindClientAssembly(const HLString &projectName);

		/// <summary>
		/// Finds the path to the clients debug pdb file.
		/// </summary>
		/// <returns></returns>
		static FileSystemPath FindClientDebugSymbols(const HLString &projectName);

	private:

		static UniqueRef<ScriptingAPI> s_ScriptingAPI;
	};
}

