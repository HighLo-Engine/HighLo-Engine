#pragma once
#include "ISystemBase.h"

namespace highlo
{
	class RenderSystem : public ISystemBase
	{
	public:
		void OnUpdate(Timestep ts, ECS_Registry& registry) override;
	};
}
