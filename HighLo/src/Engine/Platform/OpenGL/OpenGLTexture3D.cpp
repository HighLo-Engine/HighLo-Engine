// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLTexture3D.h"

#ifdef HIGHLO_API_OPENGL

#include <stb_image.h>

#include "Engine/Utils/ImageUtils.h"
#include "Engine/Renderer/Renderer.h"

#include "OpenGLUtils.h"

#define TEXTURE3D_LOG_PREFIX "Texture3D>    "

namespace highlo
{
	OpenGLTexture3D::OpenGLTexture3D(const FileSystemPath &filePath, bool flipOnLoad)
	{
		m_Specification.Format = TextureFormat::RGBA32F;

		glGenTextures(1, &RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, RendererID);

		stbi_set_flip_vertically_on_load(flipOnLoad);

		int32 width, height, nrComponents;
		Byte *data = (Byte*)stbi_loadf(*filePath.String(), &width, &height, &nrComponents, 0);

		if (data)
		{
			m_Buffer = Allocator::Copy(data, width * height * nrComponents * sizeof(float));
			m_Loaded = true;
			stbi_image_free(data);

			uint32 levels = utils::CalculateMipCount(width, height);
			m_Specification.Width = width;
			m_Specification.Height = height;

			glTextureStorage2D(RendererID, levels, utils::OpenGLTextureInternalFormat(m_Specification.Format), m_Specification.Width, m_Specification.Height);
			glTextureSubImage3D(RendererID, 0, 0, 0, 0, m_Specification.Width, m_Specification.Height, 6, utils::OpenGLTextureFormat(m_Specification.Format), utils::OpenGLFormatDataType(m_Specification.Format), m_Buffer.Data);
		}
		else
		{
			HL_CORE_ERROR(TEXTURE3D_LOG_PREFIX "[-] Failed to load Texture3D: {0} (reason: {1}) [-]", **filePath, stbi_failure_reason());
			stbi_image_free(data);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(m_Specification.Properties.SamplerFilter, false));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(m_Specification.Properties.SamplerFilter, false));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(m_Specification.Properties.SamplerWrap));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(m_Specification.Properties.SamplerWrap));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, utils::OpenGLSamplerWrap(m_Specification.Properties.SamplerWrap));
	}

	OpenGLTexture3D::OpenGLTexture3D(TextureFormat format, uint32 width, uint32 height, const void *data)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;
		m_Specification.Format = format;

		uint32 levels = utils::CalculateMipCount(width, height);

		glGenTextures(1, &RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, RendererID);
		glTextureStorage2D(RendererID, levels, utils::OpenGLTextureInternalFormat(m_Specification.Format), m_Specification.Width, m_Specification.Height);

		if (data)
		{
			m_Buffer = Allocator::Copy(data, width * height * 4 * 6); // Six Layers
			glTextureSubImage3D(RendererID, 0, 0, 0, 0, m_Specification.Width, m_Specification.Height, 6, utils::OpenGLTextureFormat(m_Specification.Format), utils::OpenGLFormatDataType(m_Specification.Format), m_Buffer.Data);
			m_Loaded = true;
		}

		glTextureParameteri(RendererID, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(m_Specification.Properties.SamplerFilter, levels > 1));
		glTextureParameteri(RendererID, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(m_Specification.Properties.SamplerFilter, false));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(m_Specification.Properties.SamplerWrap));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(m_Specification.Properties.SamplerWrap));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, utils::OpenGLSamplerWrap(m_Specification.Properties.SamplerWrap));
	}

	OpenGLTexture3D::~OpenGLTexture3D()
	{
		Release();

		if (m_Buffer)
			m_Buffer.Release();
	}

	Allocator OpenGLTexture3D::GetData()
	{
		HL_ASSERT(m_Locked, "Texture must be locked!");
		return m_Buffer;
	}

	void OpenGLTexture3D::Release()
	{
		if (RendererID)
		{
			glDeleteTextures(1, &RendererID);
			RendererID = 0;
			m_Loaded = false;
		}
	}

	void OpenGLTexture3D::Invalidate()
	{
		Release();

		uint32 levels = utils::CalculateMipCount(m_Specification.Width, m_Specification.Height);

		glGenTextures(1, &RendererID);
		glTextureStorage2D(RendererID, levels, utils::OpenGLTextureInternalFormat(m_Specification.Format), m_Specification.Width, m_Specification.Height);

		if (m_Buffer.Data)
		{
			glTextureSubImage3D(RendererID, 0, 0, 0, 0, m_Specification.Width, m_Specification.Height, 6, utils::OpenGLTextureFormat(m_Specification.Format), utils::OpenGLFormatDataType(m_Specification.Format), m_Buffer.Data);
			m_Loaded = true;
		}

		glTextureParameteri(RendererID, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(m_Specification.Properties.SamplerFilter, levels > 1));
		glTextureParameteri(RendererID, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(m_Specification.Properties.SamplerFilter, false));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(m_Specification.Properties.SamplerWrap));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(m_Specification.Properties.SamplerWrap));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, utils::OpenGLSamplerWrap(m_Specification.Properties.SamplerWrap));
	}

	void OpenGLTexture3D::Resize(const glm::uvec2 &size)
	{
		Resize(size.x, size.y);
	}

	void OpenGLTexture3D::Resize(const uint32 width, const uint32 height)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
	}

	void OpenGLTexture3D::Lock()
	{
		m_Locked = true;
	}

	void OpenGLTexture3D::Unlock()
	{
		m_Locked = false;
		glTextureSubImage3D(RendererID, 0, 0, 0, 0, m_Specification.Width, m_Specification.Height, 6, utils::OpenGLTextureFormat(m_Specification.Format), utils::OpenGLFormatDataType(m_Specification.Format), m_Buffer.Data);
	}

	void OpenGLTexture3D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
	{
		// TODO: Implement
		HL_ASSERT(false, "Not implemented yet");
	}

	glm::ivec4 OpenGLTexture3D::ReadPixel(uint32 row, uint32 column)
	{
		// TODO: Implement
		HL_ASSERT(false, "Not implemented yet");
		return {};
	}

	void OpenGLTexture3D::UpdateResourceData(void *data)
	{
		// TODO: Implement
		HL_ASSERT(false, "Not implemented yet");
	}

	void OpenGLTexture3D::UpdateResourceData()
	{
		UpdateResourceData(m_Buffer.Data);
	}

	uint32 OpenGLTexture3D::GetMipLevelCount()
	{
		return utils::CalculateMipCount(m_Specification.Width, m_Specification.Height);
	}

	std::pair<uint32, uint32> OpenGLTexture3D::GetMipSize(uint32 mip)
	{
		return utils::GetMipSize(mip, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLTexture3D::GenerateMips(bool readonly)
	{
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}

	void OpenGLTexture3D::Bind(uint32 slot) const
	{
		glBindTextureUnit(slot, RendererID);
	}

	void OpenGLTexture3D::Unbind(uint32 slot) const
	{
		glBindTextureUnit(slot, 0);
	}
}

#endif // HIGHLO_API_OPENGL

