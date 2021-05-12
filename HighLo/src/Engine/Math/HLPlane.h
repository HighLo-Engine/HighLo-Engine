#pragma once
#include "Engine/Core/HLCore.h"
#include <glm/glm.hpp>

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
