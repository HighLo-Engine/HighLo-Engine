// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include <glm/glm.hpp>

#include "Engine/Core/Core.h"

namespace highlo
{
	struct HLPlane
	{
		glm::vec3 Normal;
		float Distance;

		HLAPI HLPlane()
			: Normal(0, 1, 0), Distance(0) {}

		HLAPI HLPlane(const glm::vec3& normal, float distance)
			: Normal(normal), Distance(distance) {}

		HLAPI void Normalize()
		{
			float flScale = 1 / glm::length(Normal);
			Normal.x *= flScale;
			Normal.y *= flScale;
			Normal.z *= flScale;
			Distance *= flScale;
		}
	};
}
