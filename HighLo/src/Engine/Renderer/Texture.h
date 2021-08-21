#pragma once

#include <vector>

#include "Engine/Core/DataTypes/String.h"
#include "Engine/Math/Math.h"
#include "Engine/Core/Allocator.h"
#include "TextureFormat.h"

namespace highlo
{
#define HL_TEXTURE_UNIT_DIFFUSE 0
#define HL_TEXTURE_UNIT_NORMAL	1

	enum class TextureShaderLocation
	{
		DOMAIN_SHADER,
		PIXEL_SHADER
	};

	class Texture : public IsSharedReference
	{
	public:

		HLString Name = "";
		TextureShaderLocation ShaderLocation = TextureShaderLocation::PIXEL_SHADER;
		HLRendererID RendererID = 0;

		TextureFormat Format = TextureFormat::RGBA8;

		HLAPI virtual ~Texture() = default;
		HLAPI virtual uint32 GetDimensions() const = 0;
		HLAPI virtual uint32 GetWidth() const = 0;
		HLAPI virtual uint32 GetHeight() const = 0;
		HLAPI virtual Allocator GetData() = 0;

		HLAPI virtual void Bind(uint32 slot) const = 0;
		HLAPI virtual void Release() = 0;
		HLAPI virtual void Invalidate() = 0;

		HLAPI virtual void WritePixel(uint32 row, uint32 column, const glm::ivec4& rgba) = 0;
		HLAPI virtual glm::ivec4 ReadPixel(uint32 row, uint32 column) = 0;
		HLAPI virtual void UpdateResourceData(void* data) = 0;
		HLAPI virtual void UpdateResourceData() = 0;
		HLAPI virtual uint32 GetMipLevelCount() = 0;

		HLAPI inline TextureFormat GetFormat() { return Format; }
		HLAPI inline HLRendererID GetRendererID() const { return RendererID; }
		HLAPI inline uint64 GetHash() const { return (uint64)RendererID; }

		HLAPI inline bool operator==(const Texture& other) const { return RendererID == other.RendererID; }
		HLAPI inline bool operator!=(const Texture& other) const { return !(*this == other); }
	};

	class Texture2D : public Texture
	{
	public:

		HLAPI virtual uint32 GetDimensions() const override { return 2; };

		HLAPI static Ref<Texture> LoadFromFile(const HLString& filepath, TextureFormat format = TextureFormat::RGBA8, bool flip_on_load = true);
		HLAPI static Ref<Texture> CreateFromColor(const glm::vec3& rgb, TextureFormat format = TextureFormat::RGBA8);
		HLAPI static Ref<Texture> CreateFromColor(const glm::vec3& rgb, uint32 width, uint32 height, TextureFormat format = TextureFormat::RGBA8);
		HLAPI static Ref<Texture> Create(TextureFormat format, uint32 width, uint32 height);

		HLAPI virtual HLRendererID GetSamplerRendererID() const = 0;

		HLAPI virtual void CreatePerLayerImageViews() = 0;
		HLAPI virtual void CreateSampler(TextureProperties properties) = 0;

		HLAPI virtual TextureSpecification &GetSpecification() = 0;
		HLAPI virtual const TextureSpecification &GetSpecification() const = 0;
	};

	class Texture3D : public Texture
	{
	public:

		HLAPI virtual uint32 GetDimensions() const override { return 3; };

		HLAPI static Ref<Texture> LoadFromFiles(const std::vector<HLString>& filepaths);
		HLAPI static Ref<Texture> Create(TextureFormat format, uint32 width, uint32 height, const void *data = nullptr);
	};
}
