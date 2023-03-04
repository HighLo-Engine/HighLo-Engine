// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2021-11-02) Added More getters for ProjectConfig values
//     - 1.0 (2021-09-15) initial release
//

#pragma once

#include "Engine/Core/FileSystemPath.h"

namespace highlo
{
// TODO: The values should not be hardcoded, they should be loaded from the serialized scene file
	struct ProjectConfig
	{
		HLString Name = "Unnamed Project";

		HLString AssetDirectory = "assets/";
		HLString AssetRegistryPath = "assets/assets.registry";

		HLString MeshPath = "assets/models/";
		HLString MeshSourcePath = "";

		HLString StartScene;

		bool ReloadScriptAssemblyOnPlay;

		HLString ProjectFileName = "Unnamed_highlo_project.hlproject";
		HLString ProjectDirectory = "";
	};

	class Project : public IsSharedReference
	{
	public:

		HLAPI Project();
		HLAPI ~Project();

		HLAPI const ProjectConfig &GetConfig() const { return m_Config; }

		HLAPI static Ref<Project> GetActive() { return s_ActiveProject; }
		HLAPI static void SetActive(Ref<Project> project);

		HLAPI static const HLString &GetProjectName()
		{
			HL_ASSERT(s_ActiveProject);
			return s_ActiveProject->GetConfig().Name;
		}

		HLAPI static FileSystemPath GetProjectDirectory()
		{
			HL_ASSERT(s_ActiveProject);
			return FileSystemPath(s_ActiveProject->GetConfig().ProjectDirectory);
		}

		HLAPI static FileSystemPath GetAssetDirectory()
		{
			HL_ASSERT(s_ActiveProject);
			//return FileSystemPath(s_ActiveProject->GetConfig().ProjectDirectory) / FileSystemPath(s_ActiveProject->GetConfig().AssetDirectory);
			return FileSystemPath(s_ActiveProject->GetConfig().AssetDirectory);
		}

		HLAPI static FileSystemPath GetAssetRegistryPath()
		{
			HL_ASSERT(s_ActiveProject);
			return FileSystemPath(s_ActiveProject->GetConfig().ProjectDirectory) / FileSystemPath(s_ActiveProject->GetConfig().AssetRegistryPath);
		}

		HLAPI static FileSystemPath GetMeshPath()
		{
			HL_ASSERT(s_ActiveProject);
			return FileSystemPath(s_ActiveProject->GetConfig().ProjectDirectory) / FileSystemPath(s_ActiveProject->GetConfig().MeshPath);
		}

		HLAPI static FileSystemPath GetCacheDirectory()
		{
			HL_ASSERT(s_ActiveProject);
			return FileSystemPath(s_ActiveProject->GetConfig().ProjectDirectory) / "Cache";
		}

	private:

		inline static Ref<Project> s_ActiveProject;

		ProjectConfig m_Config;
	};
}

