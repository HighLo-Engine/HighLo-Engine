// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-07-14) initial release
//

#pragma once

#include "ScriptingAPI.h"
#include "Engine/Core/FileSystemPath.h"

namespace highlo
{
	struct ScriptEngineConfig
	{
		FileSystemPath CoreAssemblyPath;
	};

	/// <summary>
	/// The script engine is the global connector between the scripting module and the rest of the engine.
	/// </summary>
	class ScriptEngine
	{
	public:

		HLAPI static void Init(const ScriptEngineConfig *config = nullptr);
		HLAPI static void Shutdown();



	private:

		static UniqueRef<ScriptingAPI> s_ScriptingAPI;
	};
}

