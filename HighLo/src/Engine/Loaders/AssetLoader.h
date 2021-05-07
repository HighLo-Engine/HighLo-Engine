#pragma once
#include "Engine/Renderer/Model.h"

namespace highlo
{
	class AssetLoader
	{
	public:
		HLAPI static Model LoadStaticModel(const HLString& filepath, bool bShouldApplyCorrectionMatrix = false);
	};
}

