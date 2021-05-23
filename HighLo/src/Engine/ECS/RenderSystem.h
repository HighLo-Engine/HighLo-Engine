#pragma once

#include "ISystemBase.h"

namespace highlo
{
	class RenderSystem : public ISystemBase
	{
	public:

		HLAPI void OnUpdate(Timestep ts, ECS_Registry& registry) override;

		uint64 FrameEntitiesRendered;
		uint64 FrameMeshesRendered;
		uint64 FrameTrianglesRendered;
	};
}
