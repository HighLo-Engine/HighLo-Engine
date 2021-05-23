#pragma once

#include "Entity.h"
#include "Components.h"

namespace highlo
{
	class ISystemBase : public IsSharedReference
	{
	public:

		HLAPI virtual void OnCreate(ECS_Registry& registry) {}
		HLAPI virtual void OnDestroy(ECS_Registry& registry) {}
		HLAPI virtual void OnUpdate(Timestep ts, ECS_Registry& registry) {}
	};
}
