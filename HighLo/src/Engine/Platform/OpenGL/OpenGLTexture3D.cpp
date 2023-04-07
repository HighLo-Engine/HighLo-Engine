// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLTexture3D.h"

#ifdef HIGHLO_API_OPENGL

#include <stb_image.h>

#include "Engine/Utils/ImageUtils.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Core/FileSystem.h"

#include "OpenGLUtils.h"

#define TEXTURE3D_LOG_PREFIX "Texture3D>    "

namespace highlo
{
	OpenGLTexture3D::OpenGLTexture3D(const FileSystemPath &filePath, bool flipOnLoad)
		: m_FilePath(filePath)
	{
		HL_ASSERT(false, "Not implemented");
	}

	OpenGLTexture3D::OpenGLTexture3D(TextureFormat format, uint32 width, uint32 height, const void *data)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;
		m_Specification.Format = format;

		uint32 levels = utils::CalculateMipCount(width, height);
		uint32 bpp = utils::GetImageFormatBPP(format);

		if (data)
		{
			m_Buffer = Allocator::Copy(data, width * height * 4 * 6); // Six Layers
			m_Loaded = true;
		}

		Ref<OpenGLTexture3D> instance = this;
		Renderer::Submit([instance, levels]() mutable
		{
			glGenTextures(1, &instance->RendererID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, instance->RendererID);

			glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &instance->RendererID);
			glTextureStorage2D(
				instance->RendererID, 
				levels, 
				utils::OpenGLTextureInternalFormat(instance->m_Specification.Format), 
				instance->m_Specification.Width, 
				instance->m_Specification.Height);
			
			if (instance->m_Buffer.Data)
				glTextureSubImage3D(instance->RendererID, 0, 0, 0, 0, instance->m_Specification.Width, instance->m_Specification.Height, 6, utils::OpenGLTextureFormat(instance->m_Specification.Format), utils::OpenGLFormatDataType(instance->m_Specification.Format), instance->m_Buffer.Data);

			glTextureParameteri(instance->RendererID, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(instance->m_Specification.Properties.SamplerFilter, instance->m_Specification.Properties.GenerateMips));
			glTextureParameteri(instance->RendererID, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(instance->m_Specification.Properties.SamplerFilter, false));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
		});
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
		GLuint rendererID = RendererID;
		m_Loaded = false;
		
		Renderer::SubmitWithoutResources([rendererID]()
		{
			if (rendererID)
			{
				glDeleteTextures(1, &rendererID);
			}
		});
	}

	void OpenGLTexture3D::Invalidate()
	{
		Release();

		Ref<OpenGLTexture3D> instance = this;
		Renderer::Submit([instance]() mutable
		{
			uint32 levels = utils::CalculateMipCount(instance->m_Specification.Width, instance->m_Specification.Height);

			glGenTextures(1, &instance->RendererID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, instance->RendererID);
			glTextureStorage2D(instance->RendererID, levels, utils::OpenGLTextureInternalFormat(instance->m_Specification.Format), instance->m_Specification.Width, instance->m_Specification.Height);

			if (instance->m_Buffer.Data)
			{
				glTextureSubImage3D(instance->RendererID, 0, 0, 0, 0, instance->m_Specification.Width, instance->m_Specification.Height, 6, utils::OpenGLTextureFormat(instance->m_Specification.Format), utils::OpenGLFormatDataType(instance->m_Specification.Format), instance->m_Buffer.Data);
				instance->m_Loaded = true;
			}

			glTextureParameteri(instance->RendererID, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(instance->m_Specification.Properties.SamplerFilter, levels > 1));
			glTextureParameteri(instance->RendererID, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(instance->m_Specification.Properties.SamplerFilter, false));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
		});
	}

	void OpenGLTexture3D::RT_Invalidate()
	{
		if (RendererID)
		{
			glDeleteTextures(1, &RendererID);
			RendererID = 0;
			m_Loaded = false;
		}

		uint32 levels = utils::CalculateMipCount(m_Specification.Width, m_Specification.Height);

		glGenTextures(1, &RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, RendererID);
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

		Ref<OpenGLTexture3D> instance = this;
		Renderer::Submit([instance]()
		{
			glTextureSubImage3D(
				instance->RendererID,
				0, 0, 0, 0, 
				instance->m_Specification.Width,
				instance->m_Specification.Height,
				6, 
				utils::OpenGLTextureFormat(instance->m_Specification.Format),
				utils::OpenGLFormatDataType(instance->m_Specification.Format),
				instance->m_Buffer.Data);
		});
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
		Renderer::Submit([]()
		{
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		});
	}

	void OpenGLTexture3D::Bind(uint32 slot) const
	{
		GLuint rendererID = RendererID;
		Renderer::Submit([rendererID, slot]()
		{
			glBindTextureUnit(slot, rendererID);
		});
	}

	void OpenGLTexture3D::Unbind(uint32 slot) const
	{
		Renderer::Submit([slot]()
		{
			glBindTextureUnit(slot, 0);
		});
	}
}

#endif // HIGHLO_API_OPENGL

