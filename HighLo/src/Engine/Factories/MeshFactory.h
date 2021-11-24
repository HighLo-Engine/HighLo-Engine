// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Renderer/Model.h"

namespace highlo
{
	class MeshFactory
	{
	public:

		HLAPI static Ref<Mesh>	CreateCube(const glm::vec3 &size);
		HLAPI static Model		CreateCubeModel(const glm::vec3 &size);
		HLAPI static Ref<Mesh>	CreateSphere(float radius);
		HLAPI static Ref<Mesh>	CreateCapsule(float radius, float height);
	};
}

