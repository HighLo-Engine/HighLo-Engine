// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "ISystemBase.h"

namespace highlo
{
	class ECS_SystemManager
	{
	public:

		template <typename T>
		HLAPI void RegisterSystem(const HLString &name = "")
		{
			auto instance = Ref<T>::Create();
			instance->OnCreate(ECS_Registry::Get());

			m_Systems.push_back(instance);

			if (!name.IsEmpty())
				m_SystemMappings[name] = instance;
		}

		HLAPI Ref<ISystemBase> GetSystem(const HLString &name)
		{
			if (m_SystemMappings.find(name) != m_SystemMappings.end())
				return m_SystemMappings[name];
			else
				return nullptr;
		}

		HLAPI void Update();
		HLAPI void Shutdown();

	private:

		std::map<HLString, Ref<ISystemBase>> m_SystemMappings;
		std::vector<Ref<ISystemBase>> m_Systems;
	};
}

