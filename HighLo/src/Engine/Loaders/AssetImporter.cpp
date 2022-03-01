// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "AssetImporter.h"

#include "Engine/Assets/AssetManager.h"

namespace highlo
{
	void AssetImporter::Init()
	{
	}
	
	void AssetImporter::Shutdown()
	{
	}
	
	bool AssetImporter::TryLoadData(const AssetMetaData &assetInfo, Ref<Asset> &asset)
	{
		bool loaded = false;

		switch (assetInfo.Type)
		{
			case AssetType::DynamicMesh:
				break;

			case AssetType::StaticMesh:
				break;

			case AssetType::MeshAsset:
				break;

			case AssetType::EnvMap:
				break;

			case AssetType::Font:
				break;

			case AssetType::Material:
				break;

			case AssetType::Prefab:
				break;

			case AssetType::Scene:
				break;

			case AssetType::Texture:
				break;
		}

		return loaded;
	}

	void AssetImporter::Serialize(const AssetMetaData &assetInfo, const Ref<Asset> &asset)
	{
// TODO: write into config file
	}
	
	void AssetImporter::Serialize(const Ref<Asset> &asset)
	{
		const AssetMetaData &assetInfo = AssetManager::Get()->GetMetaData(asset->Handle);
		Serialize(assetInfo, asset);
	}
}

