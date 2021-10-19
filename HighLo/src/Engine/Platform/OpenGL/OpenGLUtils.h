// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

#include "Engine/Core/Log.h"
#include "Engine/Core/Assert.h"
#include "Engine/Renderer/TextureFormat.h"

namespace highlo
{
	namespace utils
	{
		inline GLenum OpenGLTextureFormat(TextureFormat format)
		{
			switch (format)
			{
				case TextureFormat::RGB:
				case TextureFormat::SRGB:
					return GL_RGB;

				case TextureFormat::RGBA:
				case TextureFormat::RGBA8:
				case TextureFormat::RGBA16:
				case TextureFormat::RGBA16F:
				case TextureFormat::RGBA32:
				case TextureFormat::RGBA32F:
					return GL_RGBA;
				
				case TextureFormat::Depth:
					return GL_DEPTH24_STENCIL8;

				case TextureFormat::DEPTH32F:
					return GL_DEPTH_COMPONENT32F;

				case TextureFormat::RED_INTEGER:
					return GL_RED_INTEGER;
			}

		HL_ASSERT(false);
		return 0;
		}

		inline GLenum OpenGLTextureInternalFormat(TextureFormat format)
		{
			switch (format)
			{
				case TextureFormat::RGB:
					return GL_RGB8;

				case TextureFormat::RGBA:
				case TextureFormat::RGBA8:
					return GL_RGBA8;

				case TextureFormat::SRGB:
					return GL_SRGB8;

				case TextureFormat::RGBA16F:
					return GL_RGBA16F;

				case TextureFormat::RGBA32F:
					return GL_RGBA32F;

				case TextureFormat::DEPTH24STENCIL8:
					return GL_DEPTH24_STENCIL8;

				case TextureFormat::DEPTH32F:
					return GL_DEPTH_COMPONENT32F;
			}

			HL_ASSERT(false);
			return 0;
		}

		inline GLenum OpenGLFormatDataType(TextureFormat format)
		{
			switch (format)
			{
				case TextureFormat::RGB:
				case TextureFormat::SRGB:
				case TextureFormat::RGBA:
					return GL_UNSIGNED_BYTE;

				case TextureFormat::RGBA16F:
				case TextureFormat::RGBA32F:
					return GL_FLOAT;
			}

			HL_ASSERT(false);
			return 0;
		}

		inline GLenum OpenGLSamplerWrap(TextureWrap wrap)
		{
			switch (wrap)
			{
				case TextureWrap::Clamp:
					return GL_CLAMP_TO_EDGE;

				case TextureWrap::Repeat:
					return GL_REPEAT;
			}

			HL_ASSERT(false);
			return 0;
		}

		inline GLenum OpenGLSamplerFilter(TextureFilter filter, bool mipmap)
		{
			switch (filter)
			{
				case TextureFilter::Linear:	
					return mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

				case TextureFilter::Nearest:
					return mipmap ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
			}

			HL_ASSERT(false);
			return 0;
		}
	}
}

#endif // HIGHLO_API_OPENGL
