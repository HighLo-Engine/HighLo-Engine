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
	using AssembliesArray = std::array<Ref<AssemblyInfo>, 2>;

	class MonoAPI : public ScriptingAPI
	{
	public:

		virtual void Init(const ScriptEngineConfig *config = nullptr) override;
		virtual void Shutdown() override;

		virtual bool ReloadAssembly(const FileSystemPath &filePath) override;
		virtual void UnloadAssembly(Ref<AssemblyInfo> &assemblyInfo) override;



		virtual ScriptEngineConfig &GetConfig() override;
		virtual const ScriptEngineConfig &GetConfig() const override;

		// Mono API specific
		static MonoDomain *GetCoreDomain();
		static MonoDomain *GetAppDomain();

	private:

		void InitMono(const ScriptEngineConfig *config = nullptr);
		void ShutdownMono();

		bool LoadCoreAssembly();
		bool LoadAppAssembly(const FileSystemPath &filePath);
	};
}

#endif // HIGHLO_API_MONO_SCRIPTING

