// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

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

		HLAPI static AssetHandle CreateCube(const glm::vec3 &size);
		HLAPI static AssetHandle CreateSphere(float radius);
		HLAPI static AssetHandle CreateCapsule(float radius, float height);
	};
}

