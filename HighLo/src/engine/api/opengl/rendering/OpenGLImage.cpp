#include "HighLoPch.h"
#include "OpenGLImage.h"

#ifdef HIGHLO_API_OPENGL

#include "engine/utils/ImageUtils.h"

namespace highlo
{
	namespace utils
	{
		inline GLenum OpenGLImageFormat(ImageFormat format)
		{
			switch (format)
			{
				case ImageFormat::RGB:
				case ImageFormat::SRGB:
					return GL_RGB;

				case ImageFormat::RGBA:
				case ImageFormat::RGBA16F:
				case ImageFormat::RGBA32F:
					return GL_RGBA;
			}

			HL_ASSERT(false);
			return 0;
		}

		inline GLenum OpenGLImageInternalFormat(ImageFormat format)
		{
			switch (format)
			{
				case ImageFormat::RGB:				return GL_RGB8;
				case ImageFormat::SRGB:				return GL_SRGB8;
				case ImageFormat::RGBA:				return GL_RGBA8;
				case ImageFormat::RGBA16F:			return GL_RGBA16F;
				case ImageFormat::RGBA32F:			return GL_RGBA32F;
				case ImageFormat::DEPTH24STENCIL8:	return GL_DEPTH24_STENCIL8;
				case ImageFormat::DEPTH32F:			return GL_DEPTH_COMPONENT32F;
			}

			HL_ASSERT(false);
			return 0;
		}

		inline GLenum OpenGLFormatDataType(ImageFormat format)
		{
			switch (format)
			{
				case ImageFormat::RGB:
				case ImageFormat::RGBA:
				case ImageFormat::SRGB:
					return GL_UNSIGNED_BYTE;

				case ImageFormat::RGBA16F:
				case ImageFormat::RGBA32F:
					return GL_FLOAT;
			}

			HL_ASSERT(false);
			return 0;
		}
	}

	OpenGLImage2D::OpenGLImage2D(ImageFormat format, uint32 width, uint32 height, Allocator buffer)
		: m_Format(format), m_Width(width), m_Height(height), m_ImageData(buffer) {}

	OpenGLImage2D::OpenGLImage2D(ImageFormat format, uint32 width, uint32 height, const void *data)
		: m_Format(format), m_Width(width), m_Height(height)
	{
		if (data)
			m_ImageData = Allocator::Copy(data, utils::GetImageMemorySize(format, width, height));
	}

	OpenGLImage2D::~OpenGLImage2D()
	{
		m_ImageData.Release();

		if (m_RendererID)
		{
			glDeleteTextures(1, &m_RendererID);
		}
	}

	void OpenGLImage2D::Invalidate()
	{
		if (m_RendererID)
			Release();

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		GLenum glInternalFormat = utils::OpenGLImageInternalFormat(m_Format);
		uint32 mipCount = utils::CalculateMipCount(m_Width, m_Height);
		glTextureStorage2D(m_RendererID, mipCount, glInternalFormat, m_Width, m_Height);
		if (m_ImageData)
		{
			GLenum glFormat = utils::OpenGLImageFormat(m_Format);
			GLenum glDataType = utils::OpenGLFormatDataType(m_Format);
			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, glFormat, glDataType, m_ImageData.m_Data);
			glGenerateTextureMipmap(m_RendererID);
		}

		// Sampler
		// TODO: should be seperate from Image2D
		glCreateSamplers(1, &m_SamplerRendererID);
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_MIN_FILTER, m_ImageData ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	void OpenGLImage2D::Release()
	{
		if (m_RendererID)
		{
			glDeleteTextures(1, &m_RendererID);
			m_RendererID = 0;
		}

		m_ImageData.Release();
	}
}

#endif // HIGHLO_API_OPENGL