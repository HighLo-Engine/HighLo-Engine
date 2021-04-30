#pragma once

namespace highlo
{
	enum class ImageFormat
	{
		None = 0,
		RGB,
		RGBA,
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