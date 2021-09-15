// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-15) initial release
//

#pragma once

namespace highlo
{
	struct ProjectConfig
	{
		HLString Name;

		HLString AssetDirectory;
		HLString AssetRegistryPath;

		HLString MeshPath;
		HLString MeshSourcePath;

		HLString StartScene;

		bool ReloadScriptAssemblyOnPlay;

		HLString ProjectFileName;
		HLString ProjectDirectory;
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

		HLAPI static const HLString &GetProjectDirectory()
		{
			HL_ASSERT(s_ActiveProject);
			return s_ActiveProject->GetConfig().ProjectDirectory;
		}



	private:

		inline static Ref<Project> s_ActiveProject;

		ProjectConfig m_Config;
	};
}

