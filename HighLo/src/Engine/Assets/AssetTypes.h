// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2021-09-21) Added Font Assetype
//     - 1.1 (2021-09-19) Added Prefab Assetype
//     - 1.0 (2021-09-14) initial release
//

#pragma once

namespace highlo
{
	enum class AssetFlag : uint16
	{
		None = 0,
		Missing = HL_BIT(0),
		Invalid = HL_BIT(1)
	};

	enum class AssetType : uint16
	{
		None = 0,
		Scene = 1,
		Mesh = 2,
		StaticMesh = 3,
		Material = 4,
		Texture = 5,
		EnvMap = 6,
		Prefab = 7,
		Font = 8,

		MeshAsset = 9,
	};

	namespace utils
	{
		inline AssetType AssetTypeFromString(const HLString &assetType)
		{
			if (assetType == "None") return AssetType::None;
			if (assetType == "Scene") return AssetType::Scene;
			if (assetType == "Mesh") return AssetType::Mesh;
			if (assetType == "Material") return AssetType::Material;
			if (assetType == "Texture") return AssetType::Texture;
			if (assetType == "EnvMap") return AssetType::EnvMap;
			if (assetType == "MeshAsset") return AssetType::MeshAsset;

			HL_ASSERT(false, "Unknown Asset");
			return AssetType::None;
		}

		inline HLString AssetTypeToString(AssetType assetType)
		{
			switch (assetType)
			{
				case AssetType::None:		return "None";
				case AssetType::Scene:		return "Scene";
				case AssetType::Mesh:		return "Mesh";
				case AssetType::Material:	return "Material";
				case AssetType::Texture:	return "Texture";
				case AssetType::EnvMap:		return "EnvMap";
				case AssetType::MeshAsset:	return "MeshAsset";
			}

			HL_ASSERT(false, "Unknown Asset");
			return "None";
		}
	}
}

