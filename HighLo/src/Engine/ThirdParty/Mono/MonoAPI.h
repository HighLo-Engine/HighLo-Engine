// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-07-14) initial release
//

#pragma once

#include "Engine/Scripting/ScriptingAPI.h"

#ifdef HIGHLO_API_MONO_SCRIPTING

#define DISABLE_AOT
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/exception.h>
#include <mono/metadata/mono-debug.h>
#include <mono/metadata/threads.h>

namespace highlo
{
	class MonoAPI : public ScriptingAPI
	{
	public:

		virtual void Init(const ScriptEngineConfig *config = nullptr) override;
		virtual void Shutdown() override;



	private:

		void InitMono(const ScriptEngineConfig *config = nullptr);
		void ShutdownMono();
	};
}

#endif // HIGHLO_API_MONO_SCRIPTING

