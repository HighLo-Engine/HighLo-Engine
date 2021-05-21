#pragma once
#include "Engine/Renderer/Model.h"

namespace highlo
{
	class AssetLoader
	{
	public:
		HLAPI static Ref<Model> LoadStaticModel(const HLString& filepath, bool bShouldApplyCorrectionMatrix = false);
		HLAPI static Ref<Mesh> LoadAnimatedModel(const HLString& filepath, bool bShouldApplyCorrectionMatrix = false);
	};
}

