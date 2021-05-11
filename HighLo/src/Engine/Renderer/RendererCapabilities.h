#pragma once

#include "Engine/Core/DataTypes/HLString.h"

namespace highlo
{
	struct RendererCapabilities
	{
		HLString Vendor;
		HLString Device;
		HLString Version;

		int32 MaxSamples = 0;
		int32 MaxTextureUnits = 0;
		float MaxAnisotropy = 0.0f;
	};
}
