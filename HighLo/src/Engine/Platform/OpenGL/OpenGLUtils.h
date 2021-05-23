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
				case TextureFormat::RGB:		return GL_RGB;
				case TextureFormat::SRGB:		return GL_RGB;
				case TextureFormat::RGBA:		return GL_RGBA;
				case TextureFormat::RGBA16F:	return GL_RGBA;
				case TextureFormat::RGBA32F:	return GL_RGBA;
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
				case TextureFormat::RGB:		return GL_UNSIGNED_BYTE;
				case TextureFormat::RGBA:		return GL_UNSIGNED_BYTE;
				case TextureFormat::SRGB:		return GL_UNSIGNED_BYTE;
				case TextureFormat::RGBA16F:	return GL_FLOAT;
				case TextureFormat::RGBA32F:	return GL_FLOAT;
			}

			HL_ASSERT(false);
			return 0;
		}

		inline GLenum OpenGLSamplerWrap(TextureWrap wrap)
		{
			switch (wrap)
			{
				case TextureWrap::Clamp:	return GL_CLAMP_TO_EDGE;
				case TextureWrap::Repeat:	return GL_REPEAT;
			}

			HL_ASSERT(false);
			return 0;
		}

		inline GLenum OpenGLSamplerFilter(TextureFilter filter, bool mipmap)
		{
			switch (filter)
			{
				case TextureFilter::Linear:		return mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
				case TextureFilter::Nearest:	return mipmap ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
			}

			HL_ASSERT(false);
			return 0;
		}
	}
}

#endif // HIGHLO_API_OPENGL
