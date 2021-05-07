#pragma once

#include <vector>

#include "Engine/Core/DataTypes/HLString.h"
#include "Engine/Math/HLMath.h"
#include "ImageFormat.h"

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

		ImageFormat Format = ImageFormat::RGBA8;

		HLAPI virtual ~Texture() = default;
		HLAPI virtual uint32 GetDimensions() const = 0;
		HLAPI virtual uint32 GetWidth() const = 0;
		HLAPI virtual uint32 GetHeight() const = 0;
		HLAPI virtual void* GetData() const = 0;

		HLAPI virtual void WritePixel(uint32 row, uint32 column, const glm::ivec4& rgba) = 0;
		HLAPI virtual glm::ivec4 ReadPixel(uint32 row, uint32 column) = 0;
		HLAPI virtual void UpdateResourceData(void* data) = 0;
		HLAPI virtual void UpdateResourceData() = 0;
		HLAPI virtual uint32 GetMipLevelCount() = 0;
		HLAPI virtual HLRendererID GetRendererID() = 0;

		HLAPI virtual void Bind(uint32 slot) const = 0;
		HLAPI inline ImageFormat GetFormat() { return Format; }
	};

	class Texture2D : public Texture
	{
	public:
		HLAPI virtual uint32 GetDimensions() const override { return 2; };

		HLAPI static Ref<Texture> LoadFromFile(const HLString& filepath, ImageFormat format = ImageFormat::RGBA8, bool flip_on_load = true);
		HLAPI static Ref<Texture> CreateFromColor(const glm::vec3& rgb, ImageFormat format = ImageFormat::RGBA8);
		HLAPI static Ref<Texture> CreateFromColor(const glm::vec3& rgb, uint32 width, uint32 height, ImageFormat format = ImageFormat::RGBA8);
	};

	class Texture3D : public Texture
	{
	public:
		HLAPI virtual uint32 GetDimensions() const override { return 3; };

		HLAPI static Ref<Texture> LoadFromFiles(const std::vector<HLString>& filepaths);
		HLAPI static Ref<Texture> Create(ImageFormat format, uint32 width, uint32 height, const void *data = nullptr);
	};
}
