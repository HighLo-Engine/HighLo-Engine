// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-07-14) initial release
//

#pragma once

#include "ScriptingConfig.h"

namespace highlo
{
	class ScriptingAPI
	{
	public:

		HLAPI virtual void Init(const ScriptEngineConfig *config = nullptr) = 0;
		HLAPI virtual void Shutdown() = 0;


	};
}

