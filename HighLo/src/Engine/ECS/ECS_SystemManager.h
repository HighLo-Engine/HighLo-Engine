#pragma once
#include "ISystemBase.h"

namespace highlo
{
	class ECS_SystemManager
	{
	public:
		template <typename T>
		void RegisterSystem(const HLString& name = "")
		{
			auto instance = Ref<T>::Create();
			instance->OnCreate(ECS_Registry::Get());

			m_Systems.push_back(instance);

			if (!name.IsEmpty())
				m_SystemMappings[name] = instance;
		}

		HLAPI Ref<ISystemBase> GetSystem(const HLString& name)
		{
			if (m_SystemMappings.find(name) != m_SystemMappings.end())
				return m_SystemMappings[name];
			else
				return nullptr;
		}

		void Update();
		void Shutdown();

	private:
		std::map<HLString, Ref<ISystemBase>> m_SystemMappings;
		std::vector<Ref<ISystemBase>> m_Systems;
	};
}
