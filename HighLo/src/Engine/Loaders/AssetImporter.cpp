// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "AssetImporter.h"

#include "Engine/Assets/AssetManager.h"
#include "Engine/Renderer/Renderer.h"

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
		// TODO: introduce a user param, where all custom parameters can be packed into

		switch (assetInfo.Type)
		{
			case AssetType::MeshAsset:
			{
				break;
			}

			case AssetType::DynamicMesh:
			{
				Ref<MeshFile> source = MeshFile::Create(AssetManager::Get()->GetFileSystemPath(assetInfo), false);
				asset = Ref<StaticModel>::Create(source);
				asset->Handle = assetInfo.Handle;
				loaded = asset->IsFlagSet(AssetFlag::None);
				break;
			}

			case AssetType::StaticMesh:
			{
				Ref<MeshFile> source = MeshFile::Create(AssetManager::Get()->GetFileSystemPath(assetInfo), false);
				asset = Ref<StaticModel>::Create(source);
				asset->Handle = assetInfo.Handle;
				loaded = asset->IsFlagSet(AssetFlag::None);
				break;
			}

			case AssetType::EnvMap:
				asset = Texture3D::LoadFromFiles(AssetManager::Get()->GetFileSystemPath(assetInfo));
				asset->Handle = assetInfo.Handle;
				loaded = asset->IsFlagSet(AssetFlag::None) && asset.As<Texture3D>()->IsLoaded();
				break;

			case AssetType::Font:
				asset = Font::Create(AssetManager::Get()->GetFileSystemPath(assetInfo), 16, FontType::TRUE_TYPE_FONT);
				asset->Handle = assetInfo.Handle;
				loaded = asset->IsFlagSet(AssetFlag::None);
				break;

			case AssetType::Material:
				// TODO
				break;

			case AssetType::Prefab:
				// TODO
				break;

			case AssetType::Scene:
				asset = Scene::CreateEmpty();
				asset->Handle = assetInfo.Handle;
				loaded = asset->IsFlagSet(AssetFlag::None);
				break;

			case AssetType::Texture:
				asset = Texture2D::LoadFromFile(AssetManager::Get()->GetFileSystemPath(assetInfo));
				asset->Handle = assetInfo.Handle;
				loaded = asset->IsFlagSet(AssetFlag::None) && asset.As<Texture2D>()->IsLoaded();
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

