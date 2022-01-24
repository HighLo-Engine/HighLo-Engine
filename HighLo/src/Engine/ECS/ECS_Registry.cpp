// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ECS_Registry.h"

namespace highlo
{
	ECS_Registry *s_RegistryInstance = nullptr;

	ECS_Registry::ECS_Registry()
	{
		s_RegistryInstance = this;
	}

	ECS_Registry &ECS_Registry::Get()
	{
		return *s_RegistryInstance;
	}
}

