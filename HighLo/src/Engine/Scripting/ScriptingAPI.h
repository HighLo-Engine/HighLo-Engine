// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-07-14) initial release
//

#pragma once

#include "ScriptingConfig.h"
#include "AssemblyInfo.h"

namespace highlo
{
	/// <summary>
	/// The Scripting API class represents the underlying scripting api (for C# it is currently Mono, Lua and Javascript will be added too).
	/// </summary>
	class ScriptingAPI
	{
	public:

		HLAPI virtual void Init(const ScriptEngineConfig *config = nullptr) = 0;
		HLAPI virtual void Shutdown() = 0;

		HLAPI virtual bool ReloadAssembly(const FileSystemPath &filePath) = 0;
		HLAPI virtual void UnloadAssembly(Ref<AssemblyInfo> &assemblyInfo) = 0;

		HLAPI virtual const Ref<AssemblyInfo> &GetCoreAssemblyInfo() = 0;
		HLAPI virtual const Ref<AssemblyInfo> &GetAppAssemblyInfo() = 0;

		HLAPI virtual ScriptEngineConfig &GetConfig() = 0;
		HLAPI virtual const ScriptEngineConfig &GetConfig() const = 0;
	};
}

