// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-06-21) initial release
//

#pragma once

#include "Engine/Animation/Animation.h"
#include "Engine/Animation/Skeleton.h"

#include <assimp/scene.h>

namespace highlo
{
	UniqueReference<Skeleton> ImportSkeleton(const HLString &filename);
	UniqueReference<Skeleton> ImportSkeleton(const aiScene *scene);

	UniqueReference<Animation> ImportAnimation(const HLString &filename, const Skeleton &skeleton);
	std::vector<HLString> GetAnimationNames(const aiScene *scene);
	UniqueReference<Animation> ImportAnimation(const aiScene *scene, const HLString &animationName, const Skeleton &skeleton);
}

