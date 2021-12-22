// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.3 (2021-11-23) Changed FilePath paramater from String to FileSystemPath
//     - 1.2 (2021-10-17) Made all Texture classes a AssetType
//     - 1.1 (2021-09-21) Added Create method that also contains the pixel data
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include <vector>

#include "Engine/Core/DataTypes/String.h"
#include "Engine/Math/Math.h"
#include "Engine/Core/Allocator.h"
#include "Engine/Core/FileSystemPath.h"
#include "TextureFormat.h"
#include "Engine/Assets/Asset.h"

namespace highlo
{
#define HL_TEXTURE_UNIT_DIFFUSE 0
#define HL_TEXTURE_UNIT_NORMAL	1

	enum class TextureShaderLocation
	{
		DOMAIN_SHADER,
		PIXEL_SHADER
	};

	class Texture : public Asset
	{
	public:

		HLString Name = "";
		TextureShaderLocation ShaderLocation = TextureShaderLocation::PIXEL_SHADER;

		HLAPI virtual ~Texture() {}
		HLAPI virtual uint32 GetDimensions() const = 0;
		HLAPI virtual TextureType GetType() const = 0;
		HLAPI virtual uint32 GetWidth() const = 0;
		HLAPI virtual uint32 GetHeight() const = 0;
		HLAPI virtual Allocator GetData() = 0;

		HLAPI virtual void Bind(uint32 slot) const = 0;
		HLAPI virtual void Unbind(uint32 slot) const = 0;

		HLAPI virtual void Release() = 0;
		HLAPI virtual void Invalidate() = 0;
		HLAPI virtual bool IsLoaded() const = 0;

		HLAPI virtual void Lock() = 0;
		HLAPI virtual void Unlock() = 0;

		HLAPI virtual void WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba) = 0;
		HLAPI virtual glm::ivec4 ReadPixel(uint32 row, uint32 column) = 0;
		HLAPI virtual void UpdateResourceData(void *data) = 0;
		HLAPI virtual void UpdateResourceData() = 0;
		HLAPI virtual uint32 GetMipLevelCount() = 0;

		HLAPI virtual TextureFormat GetFormat() = 0;
		HLAPI inline HLRendererID GetRendererID() const { return RendererID; }
		HLAPI inline uint64 GetHash() const { return (uint64)RendererID; }

		HLAPI virtual TextureSpecification &GetSpecification() = 0;
		HLAPI virtual const TextureSpecification &GetSpecification() const = 0;

		HLAPI inline bool operator==(const Texture &other) const { return RendererID == other.RendererID; }
		HLAPI inline bool operator!=(const Texture &other) const { return !(*this == other); }

	protected:
		HLRendererID RendererID = 0;
	};

	class Texture2D : public Texture
	{
	public:

		HLAPI virtual uint32 GetDimensions() const override { return 2; };
		HLAPI virtual TextureType GetType() const override { return TextureType::Texture2D; }

		HLAPI static Ref<Texture2D> LoadFromFile(const FileSystemPath &filePath, TextureFormat format = TextureFormat::RGBA8, bool flipOnLoad = false);
		HLAPI static Ref<Texture2D> CreateFromColor(const glm::vec3 &rgb, TextureFormat format = TextureFormat::RGBA8);
		HLAPI static Ref<Texture2D> CreateFromColor(const glm::vec3 &rgb, uint32 width, uint32 height, TextureFormat format = TextureFormat::RGBA8);
		HLAPI static Ref<Texture2D> Create(TextureFormat format, uint32 width, uint32 height);
		HLAPI static Ref<Texture2D> Create(TextureFormat format, uint32 width, uint32 height, const void *data, TextureProperties props = TextureProperties());

		HLAPI virtual HLRendererID GetSamplerRendererID() const = 0;

		HLAPI virtual void CreatePerLayerImageViews() = 0;
		HLAPI virtual void CreateSampler(TextureProperties properties) = 0;

		HLAPI static AssetType GetStaticType() { return AssetType::Texture; }
		HLAPI virtual AssetType GetAssetType() const override { return GetStaticType(); }
	};

	class Texture3D : public Texture
	{
	public:

		HLAPI virtual uint32 GetDimensions() const override { return 3; };
		HLAPI virtual TextureType GetType() const override { return TextureType::Texture3D; }

		HLAPI static Ref<Texture3D> LoadFromFiles(const FileSystemPath &filepath, bool flipOnLoad = false);
		HLAPI static Ref<Texture3D> Create(TextureFormat format, uint32 width, uint32 height, const void *data = nullptr);

		HLAPI static AssetType GetStaticType() { return AssetType::EnvMap; }
		HLAPI virtual AssetType GetAssetType() const override { return GetStaticType(); }
	};
}

