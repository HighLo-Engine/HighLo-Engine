#include "HighLoPch.h"
#include "OpenGLTexture.h"

#ifdef HIGHLO_API_OPENGL

#include "Engine/Core/HLLog.h"
#include "Engine/Utils/ImageUtils.h"
#include "OpenGLImage.h"
#include <stb_image.h>

namespace highlo
{
	OpenGLTexture2D* OpenGLTexture2D::LoadFromFile(const HLString& filepath, ImageFormat format, bool flip_on_load)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(flip_on_load);

		stbi_uc* data = stbi_load(*filepath, &width, &height, &channels, STBI_rgb_alpha);
		if (!data)
		{
			stbi_image_free(data);
			HL_CORE_ERROR("Texture2D> [-] Failed to load texture2D: " + filepath + " [-]");
			return nullptr;
		}

		OpenGLTexture2D* instance = new OpenGLTexture2D(data, width, height, format);
		HL_CORE_INFO("Texture2D>    [+] Loaded " + filepath + " [+]");

		return instance;
	}

	OpenGLTexture2D* OpenGLTexture2D::CreateFromColor(const glm::vec3& rgb, ImageFormat format)
	{
		OpenGLTexture2D* instance = nullptr;

		if (format == ImageFormat::RGBA8)
		{
			unsigned char data[4] = { (unsigned char)rgb.x, (unsigned char)rgb.y, (unsigned char)rgb.z, (unsigned char)255 };
			instance = new OpenGLTexture2D(data, 1, 1, format);

			instance->m_ImageData = malloc(4);
			memcpy_s(instance->m_ImageData, 4, data, 4);
		}
		else
		{
			uint16 data[4] = { (uint16)rgb.x, (uint16)rgb.y, (uint16)rgb.z, (uint16)65535 };
			instance = new OpenGLTexture2D(data, 1, 1, format);

			instance->m_ImageData = malloc(8);
			memcpy_s(instance->m_ImageData, 8, data, 8);
		}

		return instance;
	}
	
	OpenGLTexture2D* OpenGLTexture2D::CreateFromColor(const glm::vec3& rgb, uint32 width, uint32 height, ImageFormat format)
	{
		OpenGLTexture2D* instance = nullptr;

#pragma warning( push )
#pragma warning( disable : 6386)
		if (format == ImageFormat::RGBA8)
		{
			unsigned char* data = new unsigned char[(uint64)width * (uint64)height * (uint64)4];

			for (uint64 r = 0; r < (uint64)width * 4; r++)
			{
				for (uint64 c = 0; c < (uint64)height; c += 4)
				{
					uint64 idx = r * width + c;
					data[idx] = (unsigned char)(unsigned int)rgb.r;
					data[idx + 1] = (unsigned char)(unsigned int)rgb.g;
					data[idx + 2] = (unsigned char)(unsigned int)rgb.b;
					data[idx + 3] = (unsigned char)255;
				}
			}

			instance = new OpenGLTexture2D(data, width, height, format);

			instance->m_ImageData = new unsigned char[(uint64)width * (uint64)height * (uint64)4];
			memcpy_s(instance->m_ImageData, ((uint64)width * (uint64)height * (uint64)4), data, ((uint64)width * (uint64)height * (uint64)4));

			delete[] data;
		}
		else
		{
			uint16* data = new uint16[(uint64)width * (uint64)height * (uint64)4];

			for (uint64 r = 0; r < (uint64)width * 4; r++)
			{
				for (uint64 c = 0; c < (uint64)height; c += 4)
				{
					uint64 idx = r * width + c;
					data[idx] = (uint16)(unsigned int)rgb.r;
					data[idx + 1] = (uint16)(unsigned int)rgb.g;
					data[idx + 2] = (uint16)(unsigned int)rgb.b;
					data[idx + 3] = (uint16)65535;
				}
			}

			instance = new OpenGLTexture2D(data, width, height, format);

			instance->m_ImageData = new uint16[(uint64)width * (uint64)height * (uint64)4];
			memcpy_s(instance->m_ImageData, ((uint64)width * (uint64)height * (uint64)8), data, ((uint64)width * (uint64)height * (uint64)8));

			delete[] data;
		}
#pragma warning( pop )

		return instance;
	}
	
	OpenGLTexture2D::OpenGLTexture2D(void* img_data, uint32 width, uint32 height, ImageFormat format)
		: m_ImageData(img_data), m_Width(width), m_Height(height)
	{
		switch (format)
		{
			case ImageFormat::RGBA8:
			{
				m_InternalFormat = GL_RGBA8;
				break;
			}
			case ImageFormat::RGBA16:
			{
				m_InternalFormat = GL_RGBA16;
				break;
			}
			default:
			{
				m_InternalFormat = GL_RGBA8;
				break;
			}
		}

		m_DataFormat = GL_RGBA;

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		
		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0.0f);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_ID);
	}
	
	void* OpenGLTexture2D::GetData() const
	{
		return m_ImageData;
	}
	
	void OpenGLTexture2D::UpdateResourceData()
	{
		UpdateResourceData(m_ImageData);
	}
	
	void OpenGLTexture2D::UpdateResourceData(void* data)
	{
		glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}
	
	void OpenGLTexture2D::WritePixel(uint32 row, uint32 column, const glm::ivec4& rgba)
	{
		uint64 idx = (((uint64)row * (uint64)m_Width) + (uint64)column) * 4;

		if (idx >= (uint64)m_Width * (uint64)m_Height * 4 || idx < 0) return;

		if (Format == ImageFormat::RGBA8)
		{
			((unsigned char*)m_ImageData)[idx] = (unsigned char)rgba.r;
			((unsigned char*)m_ImageData)[idx + 1] = (unsigned char)rgba.g;
			((unsigned char*)m_ImageData)[idx + 2] = (unsigned char)rgba.b;
			((unsigned char*)m_ImageData)[idx + 3] = (unsigned char)rgba.a;
		}
		else
		{
			((uint16*)m_ImageData)[idx] = (uint16)rgba.r;
			((uint16*)m_ImageData)[idx + 1] = (uint16)rgba.g;
			((uint16*)m_ImageData)[idx + 2] = (uint16)rgba.b;
			((uint16*)m_ImageData)[idx + 3] = (uint16)rgba.a;
		}
	}
	
	glm::ivec4 OpenGLTexture2D::ReadPixel(uint32 row, uint32 column)
	{
		uint64 idx = (((uint64)row * (uint64)m_Width) + (uint64)column) * 4;

		if (idx >= (uint64)m_Width * (uint64)m_Height * 4 || idx < 0) return { 0, 0, 0, 0 };

		glm::ivec4 rgba = { 0, 0, 0, 0 };

		if (Format == ImageFormat::RGBA8)
		{
			rgba.r = (uint32)(((unsigned char*)m_ImageData)[idx]);
			rgba.g = (uint32)(((unsigned char*)m_ImageData)[idx + 1]);
			rgba.b = (uint32)(((unsigned char*)m_ImageData)[idx + 2]);
			rgba.a = (uint32)(((unsigned char*)m_ImageData)[idx + 3]);
		}
		else if (Format == ImageFormat::RGBA16)
		{
			rgba.r = (uint32)(((uint16*)m_ImageData)[idx]);
			rgba.g = (uint32)(((uint16*)m_ImageData)[idx + 1]);
			rgba.b = (uint32)(((uint16*)m_ImageData)[idx + 2]);
			rgba.a = (uint32)(((uint16*)m_ImageData)[idx + 3]);
		}

		return rgba;
	}

	uint32 OpenGLTexture2D::GetMipLevelCount()
	{
		return utils::CalculateMipCount(m_Width, m_Height);
	}

	HLRendererID OpenGLTexture2D::GetRendererID()
	{
		return m_ID;
	}
	
	void OpenGLTexture2D::Bind(uint32 slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	// ====================================================================================================================
	//												3D Texture
	// ====================================================================================================================

	OpenGLTexture3D::OpenGLTexture3D(const std::vector<HLString> &filePaths)
	{
		// @TODO
		HL_ASSERT(false);
	}

	OpenGLTexture3D::OpenGLTexture3D(ImageFormat format, uint32 width, uint32 height, const void *data)
	{
		m_Width = width;
		m_Height = height;
		m_Format = format;

		if (data)
		{
			uint32 size = width * height * 4 * 6; // six layers
			m_Buffer = Allocator::Copy(data, size);
		}

		uint32 levels = utils::CalculateMipCount(width, height);
		
		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_ID);
		glTextureStorage2D(m_ID, levels, utils::OpenGLImageInternalFormat(m_Format), m_Width, m_Height);

		if (m_Buffer.m_Data)
			glTextureSubImage3D(m_ID, 0, 0, 0, 0, m_Width, m_Height, 6, utils::OpenGLImageFormat(m_Format), utils::OpenGLFormatDataType(m_Format), m_Buffer.m_Data);

		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, levels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
	}

	OpenGLTexture3D::~OpenGLTexture3D()
	{
		glDeleteTextures(1, &m_ID);
	}

	uint32 OpenGLTexture3D::GetWidth() const
	{
		return m_Width;
	}

	uint32 OpenGLTexture3D::GetHeight() const
	{
		return m_Height;
	}

	void *OpenGLTexture3D::GetData() const
	{
		return m_Buffer.m_Data;
	}

	void OpenGLTexture3D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
	{
		// @TODO
	}

	glm::ivec4 OpenGLTexture3D::ReadPixel(uint32 row, uint32 column)
	{
		// @TODO
		return {};
	}

	void OpenGLTexture3D::UpdateResourceData(void *data)
	{
		// @TODO
	}

	void OpenGLTexture3D::UpdateResourceData()
	{
		// @TODO
	}

	uint32 OpenGLTexture3D::GetMipLevelCount()
	{
		return utils::CalculateMipCount(m_Width, m_Height);
	}

	HLRendererID OpenGLTexture3D::GetRendererID()
	{
		return m_ID;
	}

	void OpenGLTexture3D::Bind(uint32 slot) const
	{
		glBindTextureUnit(slot, m_ID);
	}
}

#endif
