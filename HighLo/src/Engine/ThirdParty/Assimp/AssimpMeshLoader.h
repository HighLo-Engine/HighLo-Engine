// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-06-21) initial release
//

#pragma once

#include <glm/glm.hpp>

#include "Engine/Animation/Skeleton.h"
#include "Engine/Animation/Animation.h"

#include "Engine/Core/FileSystemPath.h"

struct aiNode;
struct aiAnimation;
struct aiNodeAnim;
struct aiScene;

namespace highlo
{
	class AssimpMeshImporter
	{
	public:

		AssimpMeshImporter(const FileSystemPath &path);

		Ref<MeshFile> ImportToMeshSource();
		bool ImportSkeleton(UniqueReference<Skeleton> &skeleton);
		bool ImportAnimations(const uint32 animationIndex, const Skeleton &skeleton, std::vector<UniqueReference<Animation>> &animations);
		bool IsCompatibleSkeleton(const uint32 animationIndex, const Skeleton &skeleton);
		uint32 GetAnimationCount();
	
	private:

		void TraverseNodes(Ref<MeshFile> &meshSource, void *assimpNode, uint32 nodeIndex, const glm::mat4 &parentTransform = glm::mat4(1.0f), uint32 level = 0);
	
	private:
	
		FileSystemPath m_Path;
	};
}

