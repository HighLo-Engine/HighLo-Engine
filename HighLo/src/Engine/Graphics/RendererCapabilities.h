// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/DataTypes/String.h"

namespace highlo
{
	struct RendererCapabilities
	{
		HLString Vendor;
		HLString Device;
		HLString Version;

		int32 MaxSamples = 0;
		int32 MaxTextureUnits = 0;
		int32 MaxTextures = 0;
		float MaxAnisotropy = 0.0f;
	};
}

