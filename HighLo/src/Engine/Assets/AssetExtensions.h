// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include <unordered_map>

#include "Engine/Core/DataTypes/String.h"
#include "AssetTypes.h"

namespace highlo
{
	inline static std::unordered_map<HLString, AssetType> s_AssetExtesions =
	{
		// Engine specific file extensions
		{ "hlscene", AssetType::Scene },
		{ "hlmesh", AssetType::StaticMesh },
		{ "hldmesh", AssetType::DynamicMesh },
		{ "hlmaterial", AssetType::Material },
		{ "hlfont", AssetType::Font },
		
		// global file extensions
		{ "fbx", AssetType::MeshAsset },
		{ "obj", AssetType::MeshAsset },
		{ "gltf", AssetType::MeshAsset },
		{ "glb", AssetType::MeshAsset },

		{ "png", AssetType::Texture },
		{ "jpg", AssetType::Texture },
		{ "jpeg", AssetType::Texture },
		{ "tiff", AssetType::Texture },
		{ "hdr", AssetType::EnvMap },
		{ "ttf", AssetType::Font },
	};
}