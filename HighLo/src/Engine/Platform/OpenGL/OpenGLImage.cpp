#include "HighLoPch.h"
#include "OpenGLImage.h"

#ifdef HIGHLO_API_OPENGL

#include "engine/utils/ImageUtils.h"

namespace highlo
{
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