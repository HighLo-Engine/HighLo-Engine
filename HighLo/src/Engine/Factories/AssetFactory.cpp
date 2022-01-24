// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "AssetFactory.h"

namespace highlo
{
	AssetHandle AssetFactory::CreateCube(const glm::vec3 &size)
	{
		Ref<MeshFile> mesh = MeshFactory::CreateCube(size);
		return AssetManager::Get()->CreateMemoryOnlyAsset<StaticModel>(mesh);
	}
	
	AssetHandle AssetFactory::CreateSphere(float radius)
	{
		Ref<MeshFile> mesh = MeshFactory::CreateSphere(radius);
		return AssetManager::Get()->CreateMemoryOnlyAsset<StaticModel>(mesh);
	}

	AssetHandle AssetFactory::CreateCapsule(float radius, float height)
	{
		Ref<MeshFile> mesh = MeshFactory::CreateCapsule(radius, height);
		return AssetManager::Get()->CreateMemoryOnlyAsset<StaticModel>(mesh);
	}
}

