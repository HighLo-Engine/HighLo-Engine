// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MonoAPI.h"

#include "Engine/Core/FileSystem.h"
#include "Engine/Scripting/ScriptEngine.h"

#ifdef HIGHLO_API_MONO_SCRIPTING

#include <mono/jit/jit.h>
#include <mono/metadata/object.h>
#include <mono/metadata/assembly.h>

namespace highlo
{
	struct ScriptingData
	{
		MonoDomain *RootDomain = nullptr;
		MonoDomain *AppDomain = nullptr;

		MonoAssembly *CoreAssembly = nullptr;
		MonoAssembly *AppAssembly = nullptr;
	};

	ScriptingData *s_ScriptingData = nullptr;

	namespace utils
	{
		static MonoAssembly *LoadCSharpAssembly(const HLString &path)
		{
			int64 fileSize;
			Byte *data = FileSystem::Get()->ReadFile(path, &fileSize);

			MonoImageOpenStatus status;
			MonoImage *image = mono_image_open_from_data_full((char*)data, (uint32)fileSize, 1, &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				const char *errorMessage = mono_image_strerror(status);
				HL_CORE_ERROR("{0}", errorMessage);
				return nullptr;
			}

			MonoAssembly *assembly = mono_assembly_load_from_full(image, path.C_Str(), &status, 0);
			HL_ASSERT(assembly);

			mono_image_close(image);
			delete[] data;

			return assembly;
		}

		static void PrintAssemblyTypes(MonoAssembly *assembly)
		{
			MonoImage *image = mono_assembly_get_image(assembly);
			const MonoTableInfo *typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32 numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for (int32 i = 0; i < numTypes; i++)
			{
				uint32 cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char *nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char *name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				HL_CORE_TRACE("{0}.{1}", nameSpace, name);
			}
		}

	}

	void MonoAPI::Init(const ScriptEngineConfig *config)
	{
		s_ScriptingData = new ScriptingData();
		InitMono(config);
	}

	void MonoAPI::Shutdown()
	{
		ShutdownMono();
		delete s_ScriptingData;
		s_ScriptingData = nullptr;
	}

	void MonoAPI::InitMono(const ScriptEngineConfig *config)
	{
		HL_ASSERT(config);

		mono_set_assemblies_path("ThirdParty/mono/lib");

		MonoDomain *rootDomain = mono_jit_init("HighLoRuntime");
		HL_ASSERT(rootDomain);

		s_ScriptingData->RootDomain = rootDomain;

		MonoDomain *appDomain = mono_domain_create_appdomain("HighloAppDomain", nullptr);
		HL_ASSERT(appDomain);

		mono_domain_set(appDomain, true);
		s_ScriptingData->AppDomain = appDomain;

		// TODO: Same as above
		MonoAssembly *assembly = utils::LoadCSharpAssembly(config->CoreAssemblyPath.String());
		HL_ASSERT(assembly);

		s_ScriptingData->CoreAssembly = assembly;

		utils::PrintAssemblyTypes(assembly);
	}
	
	void MonoAPI::ShutdownMono()
	{
		// mono_domain_unload(s_ScriptingData->AppDomain);
		s_ScriptingData->AppDomain = nullptr;

		// mono_jit_cleanup(s_ScriptingData->RootDomain);
		s_ScriptingData->RootDomain = nullptr;
	}
}

#endif // HIGHLO_API_MONO_SCRIPTING

