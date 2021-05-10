#pragma once
#include "Entity.h"
#include "Components.h"

namespace highlo
{
	class ISystemBase : public IsSharedReference
	{
	public:
		virtual void OnCreate(ECS_Registry& registry) {}
		virtual void OnDestroy(ECS_Registry& registry) {}
		virtual void OnUpdate(Timestep ts, ECS_Registry& registry) {}
	};
}
