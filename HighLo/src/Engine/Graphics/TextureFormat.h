// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2021-09-21) made TextureProperties a member of the TextureSpecification
//     - 1.0 (2021-09-14) initial release
//

#pragma once

namespace highlo
{
	enum class TextureFormat
	{
		None = 0,
		RG8,
		RGB,
		RGBA,
		RGBA8,
		RG16F,
		RGBA16,
		RGBA32,
		RGBA16F,
		RGBA32F,
		RG32F,
		SRGB,
		B10R11G11UF,
		DEPTH32F,
		DEPTH24STENCIL8,
		DEPTH32FSTENCIL8UINT,
		RED_INTEGER,
		RED32F,
		RED8UN,
		RED8UI,
		RED16UI,
		RED32UI,

		// Default Depth buffer format
		Depth = DEPTH24STENCIL8
	};

	enum class TextureUsage
	{
		None = 0,
		Texture,
		Attachment,
		Storage,
		FontAtlas,
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
		Nearest,
		Cubic
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
		bool Storage = false;

		HLString DebugName = "";
	};

	struct TextureSpecification
	{
		TextureFormat Format = TextureFormat::RGBA;
		TextureUsage Usage = TextureUsage::Texture;
		TextureProperties Properties = TextureProperties();
		uint32 Width = 1;
		uint32 Height = 1;
		uint32 Mips = 1;
		uint32 Layers = 1;
		bool Deinterleaved = false;

		HLString DebugName = "";
	};
}
