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

		ScriptEngineConfig Config;
	};

	ScriptingData *s_ScriptingData = nullptr;

	namespace utils
	{
		static MonoAssembly *LoadCSharpAssembly(const FileSystemPath &path)
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

			// Check if the user wants to load debug symbols as well
			if (s_ScriptingData->Config.LoadDebugSymbols)
			{
				// First we need to check, whether a pdb file exists
				FileSystemPath pdbPath = path.ParentPath() / path.Filename();
				FileSystemPath pdbFile = pdbPath.String() + ".pdb";

				// Then load the pdb file
				if (pdbFile.Exists())
				{
					HL_CORE_INFO("Loading debug symbols from {}", **pdbFile);

					int64 size;
					mono_byte *data = (mono_byte*)FileSystem::Get()->ReadFile(pdbFile, &size);
					mono_debug_open_image_from_memory(image, data, (int32)size);
					delete[] data;
				}
			}

			MonoAssembly *assembly = mono_assembly_load_from_full(image, path.String().C_Str(), &status, 0);
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
		HL_ASSERT(!s_ScriptingData, "You are trying to initialize mono more than one time!");
		s_ScriptingData = new ScriptingData();

		if (config)
		{
			s_ScriptingData->Config = *config;
		}

		// If the user does not provide a custom config,
		// the default config inside s_ScriptingData->Config will be used.
		InitMono(&s_ScriptingData->Config);
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

