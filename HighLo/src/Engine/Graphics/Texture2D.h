// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.4 (2022-01-17) splitted Texture3D into it's own file
//     - 1.3 (2021-11-23) Changed FilePath paramater from String to FileSystemPath
//     - 1.2 (2021-10-17) Made all Texture classes a AssetType
//     - 1.1 (2021-09-21) Added Create method that also contains the pixel data
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Texture.h"

#include <vector>

namespace highlo
{
	class Texture2D : public Texture
	{
	public:

		HLAPI virtual uint32 GetDimensions() const override { return 2; };
		HLAPI virtual TextureType GetType() const override { return TextureType::Texture2D; }

		HLAPI static Ref<Texture2D> LoadFromFile(const FileSystemPath &filePath, bool flipOnLoad = false);
		HLAPI static Ref<Texture2D> CreateFromColor(const glm::vec3 &rgb, TextureFormat format = TextureFormat::RGBA8);
		HLAPI static Ref<Texture2D> CreateFromColor(const glm::vec3 &rgb, uint32 width, uint32 height, TextureFormat format = TextureFormat::RGBA8);
		HLAPI static Ref<Texture2D> Create(TextureFormat format, uint32 width, uint32 height);
		HLAPI static Ref<Texture2D> Create(TextureFormat format, uint32 width, uint32 height, const void *data, TextureProperties props = TextureProperties());
		HLAPI static Ref<Texture2D> CreateFromSpecification(const TextureSpecification &specification);

		HLAPI virtual HLRendererID GetSamplerRendererID() const = 0;

		HLAPI virtual void CreatePerLayerImageViews() = 0;
		HLAPI virtual void CreatePerSpecificLayerImageViews(const std::vector<uint32> &layerIndices) = 0;
		HLAPI virtual void CreateSampler(TextureProperties properties) = 0;
		HLAPI virtual void DestroySampler() = 0;

		HLAPI static AssetType GetStaticType() { return AssetType::Texture; }
		HLAPI virtual AssetType GetAssetType() const override { return GetStaticType(); }

		HLAPI virtual void SetData(void *data, uint32 data_size) = 0;
	};
}

