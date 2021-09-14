// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "AssetLoader.h"

#include "AssimpLoader.h"

namespace highlo
{
	Ref<Model> AssetLoader::LoadStaticModel(const HLString &filepath, bool bShouldApplyCorrectionMatrix)
	{
		return AssimpLoader::LoadStatic(filepath, bShouldApplyCorrectionMatrix);
	}

	Ref<Mesh> AssetLoader::LoadAnimatedModel(const HLString &filepath, bool bShouldApplyCorrectionMatrix)
	{
		return AssimpLoader::LoadAnimated(filepath, bShouldApplyCorrectionMatrix);
	}
}
