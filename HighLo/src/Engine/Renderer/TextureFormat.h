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
}