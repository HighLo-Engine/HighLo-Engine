// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Renderer/Meshes/StaticModel.h"
#include "Engine/Assets/Asset.h"

namespace highlo
{
	class MeshFactory
	{
	public:

		HLAPI static Ref<MeshFile> CreateCube(const glm::vec3 &size);
		HLAPI static Ref<MeshFile> CreateSphere(float radius);
		HLAPI static Ref<MeshFile> CreateCapsule(float radius, float height);
	};
}

