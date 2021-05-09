#pragma once

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

#include "Engine/Core/HLLog.h"
#include "Engine/Core/HLAssert.h"
#include "Engine/Renderer/TextureFormat.h"

namespace highlo
{
	namespace utils
	{
		inline GLenum OpenGLImageFormat(TextureFormat format)
		{
			switch (format)
			{
				case TextureFormat::RGB:
				case TextureFormat::SRGB:
					return GL_RGB;

				case TextureFormat::RGBA:
				case TextureFormat::RGBA16F:
				case TextureFormat::RGBA32F:
					return GL_RGBA;
			}

		HL_ASSERT(false);
		return 0;
		}

		inline GLenum OpenGLImageInternalFormat(TextureFormat format)
		{
			switch (format)
			{
				case TextureFormat::RGB:				return GL_RGB8;
				case TextureFormat::SRGB:				return GL_SRGB8;
				case TextureFormat::RGBA:				return GL_RGBA8;
				case TextureFormat::RGBA16F:			return GL_RGBA16F;
				case TextureFormat::RGBA32F:			return GL_RGBA32F;
				case TextureFormat::DEPTH24STENCIL8:	return GL_DEPTH24_STENCIL8;
				case TextureFormat::DEPTH32F:			return GL_DEPTH_COMPONENT32F;
			}

			HL_ASSERT(false);
			return 0;
		}

		inline GLenum OpenGLFormatDataType(TextureFormat format)
		{
			switch (format)
			{
				case TextureFormat::RGB:
				case TextureFormat::RGBA:
				case TextureFormat::SRGB:
					return GL_UNSIGNED_BYTE;

				case TextureFormat::RGBA16F:
				case TextureFormat::RGBA32F:
					return GL_FLOAT;
			}

			HL_ASSERT(false);
			return 0;
		}
	}
}

#endif