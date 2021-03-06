// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-07-14) initial release
//

#pragma once

#include "ScriptingAPI.h"

namespace highlo
{
	class ScriptEngine
	{
	public:

		HLAPI static void Init();
		HLAPI static void Shutdown();


	private:

		static UniqueRef<ScriptingAPI> s_ScriptingAPI;
	};
}

