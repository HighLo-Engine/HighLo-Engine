// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Math.h"
#include "Plane.h"
#include "AABB.h"

namespace highlo
{
#define HL_FRUSTUM_NEAR  0
#define HL_FRUSTUM_FAR   1
#define HL_FRUSTUM_LEFT  2
#define HL_FRUSTUM_RIGHT 3
#define HL_FRUSTUM_UP    4
#define HL_FRUSTUM_DOWN  5

	struct HLFrustum
	{
		HLPlane Planes[6];

		HLAPI HLFrustum()
		{
			Init(glm::mat4(1.0f));
		}

		HLAPI HLFrustum(const glm::mat4 &mat)
		{
			Init(mat);
		}

		HLAPI void Init(const glm::mat4 &mat);

		HLAPI bool SphereIntersection(const glm::vec3 &center, float radius);

		HLAPI int32 AABBIntersection(const AABB &aabb);
	};
}
