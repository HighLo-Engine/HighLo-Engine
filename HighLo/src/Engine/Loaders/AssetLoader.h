// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Renderer/Model.h"

namespace highlo
{
	class AssetLoader
	{
	public:
		HLAPI static Ref<Model> LoadStaticModel(const HLString &filepath, bool bShouldApplyCorrectionMatrix = false);
		HLAPI static Ref<Mesh> LoadAnimatedModel(const HLString &filepath, bool bShouldApplyCorrectionMatrix = false);
	};
}

