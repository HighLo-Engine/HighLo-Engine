#pragma once

namespace highlo
{
	enum class TextureFormat
	{
		None = 0,
		RGB,
		RGBA,
		RGBA8,
		RGBA16,
		RGBA32,
		RGBA16F,
		RGBA32F,
		RG32F,
		SRGB,
		DEPTH32F,
		DEPTH24STENCIL8,
		RED_INTEGER,

		// Default Depth buffer format
		Depth = DEPTH24STENCIL8
	};

	enum class TextureUsage
	{
		None = 0,
		Texture,
		Attachment
	};

	enum class TextureWrap
	{
		None = 0,
		Clamp,
		Repeat
	};

	enum class TextureFilter
	{
		None = 0,
		Linear,
		Nearest
	};

	enum class TextureType
	{
		None = 0,
		Texture2D,
		Texture3D
	};

	struct TextureProperties
	{
		TextureWrap SamplerWrap = TextureWrap::Repeat;
		TextureFilter SamplerFilter = TextureFilter::Linear;
		bool GenerateMips = true;
		bool SRGB = false;
	};

	struct TextureSpecification
	{
		TextureFormat Format = TextureFormat::RGBA;
		TextureUsage Usage = TextureUsage::Texture;
		uint32 Width = 0;
		uint32 Height = 0;
		uint32 Mips = 1;
		uint32 Layers = 1;
	};
}