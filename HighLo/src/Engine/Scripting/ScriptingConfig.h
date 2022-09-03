// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-09-02) initial release
//

#pragma once

#include "Engine/Core/FileSystemPath.h"

namespace highlo
{
	struct ScriptEngineConfig
	{
		/// <summary>
		/// Determines the path to the main C# scripting dll. 
		/// The main scripting dll contains the C# library which can be accessed through own C# projects
		/// </summary>
		FileSystemPath CoreAssemblyPath = "../../../Resources/Scripts/HighLo-C#.dll";

		/// <summary>
		/// Determines the path to the Sandbox C# scripting dll.
		/// This dll is used to test the C# scripting implementation.
		/// 
		/// This can be considered to be a debug option, so it's value may be removed in the future
		/// (or rather changed to be the client's scripting dll)
		/// </summary>
		FileSystemPath AppAssemblyPath = "../../../Resources/Scripts/Sandbox.dll";


	};
}

