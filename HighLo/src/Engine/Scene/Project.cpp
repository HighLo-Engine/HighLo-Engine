// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Project.h"

#include "Engine/Assets/AssetManager.h"

namespace highlo
{
	Project::Project()
	{
	}
	
	Project::~Project()
	{
	}
	
	void Project::SetActive(Ref<Project> project)
	{
		if (s_ActiveProject)
			AssetManager::Get()->Shutdown();

		s_ActiveProject = project;
		if (s_ActiveProject)
		{
			AssetManager::Get()->Init();
		}
	}
}

