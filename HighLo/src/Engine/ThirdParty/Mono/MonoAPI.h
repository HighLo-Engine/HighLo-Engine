// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-07-14) initial release
//

#pragma once

#include "Engine/Scripting/ScriptingAPI.h"

#ifdef HIGHLO_API_MONO_SCRIPTING

namespace highlo
{
	class MonoAPI : public ScriptingAPI
	{
	public:

		virtual void Init() override;
		virtual void Shutdown() override;



	private:

		void InitMono();
		void ShutdownMono();
	};
}

#endif // HIGHLO_API_MONO_SCRIPTING

