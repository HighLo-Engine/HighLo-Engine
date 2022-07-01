// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MeshLoader.h"

// currently we only support assimp as a Mesh loader
#define HIGHLO_API_ASSIMP_LOADER

#ifdef HIGHLO_API_ASSIMP_LOADER
#include "Engine/ThirdParty/Assimp/AssimpMeshLoader.h"
#endif // HIGHLO_API_ASSIMP_LOADER

namespace highlo
{
	Ref<MeshLoader> MeshLoader::Create(const FileSystemPath &filePath, const Ref<Shader> &shader)
	{
	#ifdef HIGHLO_API_ASSIMP_LOADER
		return Ref<AssimpMeshLoader>::Create(filePath, shader);
	#endif // HIGHLO_API_ASSIMP_LOADER
	}
}

