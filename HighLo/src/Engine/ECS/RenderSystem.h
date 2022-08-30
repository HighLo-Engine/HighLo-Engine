// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

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

