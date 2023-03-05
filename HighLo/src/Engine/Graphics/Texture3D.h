// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-01-17) initial release
//

#pragma once

#include "Texture.h"

namespace highlo
{
	class Texture3D : public Texture
	{
	public:

		HLAPI virtual uint32 GetDimensions() const override { return 3; };
		HLAPI virtual TextureType GetType() const override { return TextureType::Texture3D; }

		HLAPI static Ref<Texture3D> LoadFromFile(const FileSystemPath &filepath, bool flipOnLoad = false);
		HLAPI static Ref<Texture3D> Create(TextureFormat format, uint32 width, uint32 height, const void *data = nullptr);
		HLAPI static Ref<Texture3D> CreateFromSpecification(const TextureSpecification &spec);

		HLAPI static AssetType GetStaticType() { return AssetType::EnvMap; }
		HLAPI virtual AssetType GetAssetType() const override { return GetStaticType(); }
	};
}

