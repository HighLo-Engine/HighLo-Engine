// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MonoAPI.h"

#include "Engine/Core/FileSystem.h"
#include "Engine/Scripting/ScriptEngine.h"

#ifdef HIGHLO_API_MONO_SCRIPTING

#include <mono/jit/jit.h>
#include <mono/metadata/object.h>
#include <mono/metadata/assembly.h>

#include "MonoScriptRegistry.h"
#include "MonoScriptCache.h"

namespace highlo
{
	struct ScriptingData
	{
		// Represents the core c# binary
		MonoDomain *RootDomain = nullptr;

		// Represents the clients c# binary
		MonoDomain *AppDomain = nullptr;

		// Represents the clients configuration
		ScriptEngineConfig Config;

		bool MonoInitialized = false;

		AssembliesArray LoadedAssemblies;
	};

	ScriptingData *s_ScriptingData = nullptr;

	namespace utils
	{
		static MonoAssembly *LoadCSharpAssembly(const FileSystemPath &path, bool forceDebugSymbols = false)
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
			if (s_ScriptingData->Config.LoadDebugSymbols || forceDebugSymbols)
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

		if (s_ScriptingData->Config.EnableDebugger)
		{
			// we need the debug symbols, if the debugger should work
			s_ScriptingData->Config.LoadDebugSymbols = true;
		}

		// If the user does not provide a custom config,
		// the default config inside s_ScriptingData->Config will be used.
		InitMono(&s_ScriptingData->Config);

		s_ScriptingData->LoadedAssemblies[0] = Ref<AssemblyInfo>::Create(); // here will live the core assembly (the HighLo C# library)
		s_ScriptingData->LoadedAssemblies[1] = Ref<AssemblyInfo>::Create(); // here will live the app assembly (the game code of the client)

		bool coreAssemblyLoaded = LoadCoreAssembly();
		HL_ASSERT(coreAssemblyLoaded, "Error loading the core assembly!");

		// Try to load app assembly, but if is fails do not break the program,
		// the user can try to reload it at a later time
		if (!s_ScriptingData->Config.AppAssemblyPath.IsEmpty())
		{
			bool appAssemblyLoaded = LoadAppAssembly(s_ScriptingData->Config.AppAssemblyPath);
			if (!appAssemblyLoaded)
			{
				HL_CORE_ERROR("Could not load the app assembly from {}", **s_ScriptingData->Config.AppAssemblyPath);
			}
		}

		MonoScriptCache::Init();
	}

	void MonoAPI::Shutdown()
	{
		MonoScriptCache::Shutdown();

		ShutdownMono();
		delete s_ScriptingData;
		s_ScriptingData = nullptr;
	}

	void MonoAPI::InitMono(const ScriptEngineConfig *config)
	{
		if (s_ScriptingData->MonoInitialized)
			return;

		HL_ASSERT(config);

		mono_set_assemblies_path("ThirdParty/mono/lib");

		if (s_ScriptingData->Config.EnableDebugger)
		{
			HLString portString = s_ScriptingData->Config.DebugPort;
			HLString debuggerAgentArguments = "--debugger-agent=transport=dt_socket,address=127.0.0.1:" + portString + ",server=y,suspend=n,loglevel=3,logfile=MonoDebugger.log";

			const char *options[2] = {
				debuggerAgentArguments.C_Str(),
				"--soft-breakpoints"
			};

			mono_jit_parse_options(2, (char **)options);
			mono_debug_init(MONO_DEBUG_FORMAT_MONO);
		}

		s_ScriptingData->RootDomain = mono_jit_init("HighLoRuntime");
		HL_ASSERT(s_ScriptingData->RootDomain);

		if (s_ScriptingData->Config.EnableDebugger)
		{
			mono_debug_domain_create(s_ScriptingData->RootDomain);
		}

		mono_thread_set_main(mono_thread_current());

		s_ScriptingData->MonoInitialized = true;
		HL_CORE_INFO("Mono successfully initialized!");
	}
	
	void MonoAPI::ShutdownMono()
	{
		if (!s_ScriptingData->MonoInitialized)
		{
			HL_CORE_WARN("You already tried to shutdown mono!");
			return;
		}

		s_ScriptingData->AppDomain = nullptr;

		mono_jit_cleanup(s_ScriptingData->RootDomain);
		s_ScriptingData->RootDomain = nullptr;

		s_ScriptingData->MonoInitialized = false;
	}

	bool MonoAPI::LoadCoreAssembly()
	{
		if (!s_ScriptingData->Config.CoreAssemblyPath.Exists())
			return false;

		s_ScriptingData->AppDomain = mono_domain_create_appdomain("HighLoScriptRuntime", nullptr);
		HL_ASSERT(s_ScriptingData->AppDomain);

		mono_domain_set(s_ScriptingData->AppDomain, true);

		Ref<AssemblyInfo> &rootAssemblyInfo = s_ScriptingData->LoadedAssemblies[0];
		rootAssemblyInfo->IsCoreAssembly = true;
		rootAssemblyInfo->Classes.clear();
		rootAssemblyInfo->FilePath = s_ScriptingData->Config.CoreAssemblyPath;

		rootAssemblyInfo->Assembly = utils::LoadCSharpAssembly(rootAssemblyInfo->FilePath);
		if (!rootAssemblyInfo->Assembly)
		{
			HL_CORE_ERROR("Could not load core assembly from {}", **rootAssemblyInfo->FilePath);
			return false;
		}

		rootAssemblyInfo->AssemblyImage = mono_assembly_get_image(rootAssemblyInfo->Assembly);
		if (!rootAssemblyInfo->AssemblyImage)
		{
			HL_CORE_ERROR("Could not load core assembly image from {}", **rootAssemblyInfo->FilePath);
			return false;
		}

		HL_CORE_INFO("Successfully loaded core assembly from {}", **rootAssemblyInfo->FilePath);

#if HL_DEBUG
		utils::PrintAssemblyTypes(rootAssemblyInfo->Assembly);
#endif

		return true;
	}

	bool MonoAPI::LoadAppAssembly(const FileSystemPath &filePath)
	{
		if (!filePath.Exists())
			return false;

		Ref<AssemblyInfo> &appAssemblyInfo = s_ScriptingData->LoadedAssemblies[1];
		if (appAssemblyInfo->Assembly)
		{
			// Already loaded, reload
			appAssemblyInfo->Assembly = nullptr;
			appAssemblyInfo->AssemblyImage = nullptr;
			return ReloadAssembly(filePath);
		}

		MonoAssembly *appAssembly = utils::LoadCSharpAssembly(filePath);
		if (!appAssembly)
		{
			// TODO: if it was not able to load it, the engine should generate the premake project at runtime
			//       and then automatically open visual studio in a new window and open the client's c# project.

			HL_CORE_ERROR("Could not load app assembly from {}", **filePath);
			return false;
		}

		MonoImage *image = mono_assembly_get_image(appAssembly);
		if (!image)
		{
			HL_CORE_ERROR("Could not load app assembly image from {}", **filePath);
			return false;
		}

		appAssemblyInfo->Assembly = appAssembly;
		appAssemblyInfo->AssemblyImage = image;
		appAssemblyInfo->Classes.clear();
		appAssemblyInfo->FilePath = filePath;
		appAssemblyInfo->IsCoreAssembly = false;

		HL_CORE_INFO("Successfully loaded app assembly from {}", **filePath);

#if HL_DEBUG
		utils::PrintAssemblyTypes(appAssemblyInfo->Assembly);
#endif

		MonoScriptRegistry::MakeRegistry();
		MonoScriptCache::GenerateCacheForAssembly(appAssemblyInfo);

		return true;
	}

	bool MonoAPI::ReloadAssembly(const FileSystemPath &filePath)
	{
		if (!LoadCoreAssembly())
			return false;

		if (!LoadAppAssembly(filePath))
			return false;

		s_ScriptingData->Config.AppAssemblyPath = filePath;
		return true;
	}

	void MonoAPI::UnloadAssembly(Ref<AssemblyInfo> &assemblyInfo)
	{
		assemblyInfo->Classes.clear();
		assemblyInfo->Classes.shrink_to_fit();
		assemblyInfo->Assembly = nullptr;
		assemblyInfo->AssemblyImage = nullptr;

		if (assemblyInfo->IsCoreAssembly)
			s_ScriptingData->LoadedAssemblies[0] = Ref<AssemblyInfo>::Create();
		else
			s_ScriptingData->LoadedAssemblies[1] = Ref<AssemblyInfo>::Create();
	}

	ScriptEngineConfig &MonoAPI::GetConfig()
	{
		return s_ScriptingData->Config;
	}

	const ScriptEngineConfig &MonoAPI::GetConfig() const
	{
		return s_ScriptingData->Config;
	}

	MonoDomain *MonoAPI::GetCoreDomain()
	{
		return s_ScriptingData->RootDomain;
	}

	MonoDomain *MonoAPI::GetAppDomain()
	{
		return s_ScriptingData->AppDomain;
	}
}

#endif // HIGHLO_API_MONO_SCRIPTING

