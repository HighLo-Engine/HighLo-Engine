// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-07-14) initial release
//

#pragma once

#include "ScriptingAPI.h"
#include "ScriptingConfig.h"

namespace highlo
{
	/// <summary>
	/// The script engine is the global connector between the scripting module and the rest of the engine.
	/// </summary>
	class ScriptEngine
	{
	public:

		HLAPI static void Init(ScriptEngineConfig *config = nullptr);
		HLAPI static void Shutdown();



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

