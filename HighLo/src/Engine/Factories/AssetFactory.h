// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-01-21) initial release
//

#pragma once

#include "MeshFactory.h"
#include "Engine/Assets/Asset.h"
#include "Engine/Assets/AssetManager.h"

namespace highlo
{
	class AssetFactory
	{
	public:

		HLAPI static AssetHandle CreateCube(const glm::vec3 &size);
		HLAPI static AssetHandle CreateSphere(float radius);
		HLAPI static AssetHandle CreateCapsule(float radius, float height);
	};
}

