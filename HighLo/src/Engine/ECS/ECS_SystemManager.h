#pragma once
#include "ISystemBase.h"

namespace highlo
{
	class ECS_SystemManager
	{
	public:
		template <typename T>
		void RegisterSystem()
		{
			auto instance = Ref<T>::Create();
			instance->OnCreate(ECS_Registry::Get());

			m_Systems.push_back(instance);
		}

		void Update();
		void Shutdown();

	private:
		std::vector<Ref<ISystemBase>> m_Systems;
	};
}
