// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

namespace highlo
{
	enum class HL_UB_SLOT
	{
		VS_SCENE_BUFFER = 0,
		VS_OBJECT_BUFFER = 1,
		MATERIAL_DATA_BUFFER = 2,
		VS_CAMERA_BUFFER = 3,
		BONE_TRANSFORMS_BUFFER = 4,
		ENVIRONMENT_DATA_BUFFER = 7
	};
}
