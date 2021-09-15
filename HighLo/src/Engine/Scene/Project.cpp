// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Project.h"

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
		s_ActiveProject = project;
	}
}

