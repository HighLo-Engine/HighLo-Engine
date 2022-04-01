// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ECS_SystemManager.h"

namespace highlo
{
	void ECS_SystemManager::Update()
	{
		for (auto& system : m_Systems)
			system->OnUpdate(Time::GetTimestep(), ECS_Registry::Get());
	}

	void ECS_SystemManager::Shutdown()
	{
		for (auto& system : m_Systems)
			system->OnDestroy(ECS_Registry::Get());
	}
}
