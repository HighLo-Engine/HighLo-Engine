#include "HighLoPch.h"
#include "OpenGLTexture.h"

#ifdef HIGHLO_API_OPENGL

#include <stb_image.h>

#include "Engine/Core/HLLog.h"
#include "Engine/Utils/ImageUtils.h"
#include "OpenGLUtils.h"

namespace highlo
{
	OpenGLTexture2D* OpenGLTexture2D::LoadFromFile(const HLString& filepath, TextureFormat format, bool flip_on_load)
	{
		int32 width, height, channels;
		stbi_set_flip_vertically_on_load(flip_on_load);

		stbi_uc* data = stbi_load(*filepath, &width, &height, &channels, STBI_rgb_alpha);
		if (!data)
		{
			stbi_image_free(data);
			HL_CORE_ERROR("Texture2D> [-] Failed to load texture2D: " + filepath + " [-]");
			return nullptr;
		}

		OpenGLTexture2D* instance = new OpenGLTexture2D(data, width, height, format);
		instance->Format = format;
		HL_CORE_INFO("Texture2D>    [+] Loaded " + filepath + " [+]");

		return instance;
	}

	OpenGLTexture2D* OpenGLTexture2D::CreateFromColor(const glm::vec3& rgb, TextureFormat format)
	{
		OpenGLTexture2D* instance = nullptr;

		if (format == TextureFormat::RGBA8)
		{
			Byte data[4] = { (Byte)rgb.x, (Byte)rgb.y, (Byte)rgb.z, (Byte)255 };
			instance = new OpenGLTexture2D(data, 1, 1, format);

			void *dataToStore = malloc(4);
			memcpy_s(dataToStore, 4, data, 4);
			instance->m_Buffer = Allocator::Copy(dataToStore, 4);
		}
		else
		{
			uint16 data[4] = { (uint16)rgb.x, (uint16)rgb.y, (uint16)rgb.z, (uint16)65535 };
			instance = new OpenGLTexture2D(data, 1, 1, format);

			void *dataToStore = malloc(8);
			memcpy_s(dataToStore, 8, data, 8);
			instance->m_Buffer = Allocator::Copy(dataToStore, 8);
		}

		return instance;
	}
	
	OpenGLTexture2D* OpenGLTexture2D::CreateFromColor(const glm::vec3& rgb, uint32 width, uint32 height, TextureFormat format)
	{
		OpenGLTexture2D* instance = nullptr;

#pragma warning( push )
#pragma warning( disable : 6386)
		if (format == TextureFormat::RGBA8)
		{
			Byte *data = new Byte[(uint64)width * (uint64)height * (uint64)4];

			for (uint64 r = 0; r < (uint64)width * 4; r++)
			{
				for (uint64 c = 0; c < (uint64)height; c += 4)
				{
					uint64 idx = r * width + c;
					data[idx] = (Byte)(uint32)rgb.r;
					data[idx + 1] = (Byte)(uint32)rgb.g;
					data[idx + 2] = (Byte)(uint32)rgb.b;
					data[idx + 3] = (Byte)255;
				}
			}

			instance = new OpenGLTexture2D(data, width, height, format);

			Byte *dataToStore = new Byte[(uint64)width * (uint64)height * (uint64)4];
			instance->Format = format;
			memcpy_s(dataToStore, ((uint64)width * (uint64)height * (uint64)4), data, ((uint64)width * (uint64)height * (uint64)4));
			instance->m_Buffer = Allocator::Copy(dataToStore, (uint64)width * (uint64)height * (uint64)4);

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
					data[idx] = (uint16)(uint32)rgb.r;
					data[idx + 1] = (uint16)(uint32)rgb.g;
					data[idx + 2] = (uint16)(uint32)rgb.b;
					data[idx + 3] = (uint16)65535;
				}
			}

			instance = new OpenGLTexture2D(data, width, height, format);

			uint16 *dataToStore = new uint16[(uint64)width * (uint64)height * (uint64)4];
			instance->Format = format;
			memcpy_s(dataToStore, ((uint64)width * (uint64)height * (uint64)8), data, ((uint64)width * (uint64)height * (uint64)8));
			instance->m_Buffer = Allocator::Copy(dataToStore, (uint64)width * (uint64)height * (uint64)4);

			delete[] data;
		}
#pragma warning( pop )

		return instance;
	}
	
	OpenGLTexture2D::OpenGLTexture2D(void* img_data, uint32 width, uint32 height, TextureFormat format)
		: m_Width(width), m_Height(height)
	{
		m_Buffer = Allocator::Copy(img_data, (uint64)width * (uint64)height * (uint64)4);

		switch (format)
		{
			case TextureFormat::RGBA8:
			{
				m_InternalFormat = GL_RGBA8;
				break;
			}

			case TextureFormat::RGBA16:
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

		Format = format;
		m_DataFormat = GL_RGBA;

		glGenTextures(1, &RendererID);
		glBindTexture(GL_TEXTURE_2D, RendererID);
		
		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0.0f);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	OpenGLTexture2D::OpenGLTexture2D(TextureFormat format, uint32 width, uint32 height)
	{
		switch (format)
		{
			case TextureFormat::RGBA8:
			{
				m_InternalFormat = GL_RGBA8;
				break;
			}

			case TextureFormat::RGBA16:
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

		m_Buffer.Allocate(width * height * 4);
		Format = format;
		m_Width = width;
		m_Height = height;
		m_DataFormat = GL_RGBA;

		glGenTextures(1, &RendererID);
		glBindTexture(GL_TEXTURE_2D, RendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0.0f);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &RendererID);
		if (m_Buffer)
			stbi_image_free(m_Buffer.m_Data);
	}
	
	void* OpenGLTexture2D::GetData() const
	{
		return m_Buffer.m_Data;
	}

	Allocator OpenGLTexture2D::GetWriteableBuffer()
	{
		return m_Buffer;
	}

	void OpenGLTexture2D::Release()
	{
		if (RendererID)
		{
			glDeleteTextures(1, &RendererID);
			RendererID = 0;
		}
	}

	void OpenGLTexture2D::Invalidate()
	{
		if (RendererID)
			Release();

		glCreateTextures(GL_TEXTURE_2D, 1, &RendererID);
		GLenum glInternalFormat = utils::OpenGLImageInternalFormat(Format);
		uint32 mipCount = utils::CalculateMipCount(m_Width, m_Height);
		glTextureStorage2D(RendererID, mipCount, glInternalFormat, m_Width, m_Height);

		if (m_Buffer)
		{
			GLenum glFormat = utils::OpenGLImageFormat(Format);
			GLenum glDataType = utils::OpenGLFormatDataType(Format);
			glTextureSubImage2D(RendererID, 0, 0, 0, m_Width, m_Height, glFormat, glDataType, m_Buffer.m_Data);
			glGenerateTextureMipmap(RendererID);
		}
	}
	
	void OpenGLTexture2D::UpdateResourceData()
	{
		UpdateResourceData(m_Buffer.m_Data);
	}
	
	void OpenGLTexture2D::UpdateResourceData(void* data)
	{
		glTextureSubImage2D(RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}
	
	void OpenGLTexture2D::WritePixel(uint32 row, uint32 column, const glm::ivec4& rgba)
	{
		uint64 idx = (((uint64)row * (uint64)m_Width) + (uint64)column) * 4;

		if (idx >= (uint64)m_Width * (uint64)m_Height * 4 || idx < 0) return;

		if (Format == TextureFormat::RGBA8)
		{
			((Byte*)m_Buffer.m_Data)[idx]	  = (Byte)rgba.r;
			((Byte*)m_Buffer.m_Data)[idx + 1] = (Byte)rgba.g;
			((Byte*)m_Buffer.m_Data)[idx + 2] = (Byte)rgba.b;
			((Byte*)m_Buffer.m_Data)[idx + 3] = (Byte)rgba.a;
		}
		else
		{
			((uint16*)m_Buffer.m_Data)[idx]		= (uint16)rgba.r;
			((uint16*)m_Buffer.m_Data)[idx + 1] = (uint16)rgba.g;
			((uint16*)m_Buffer.m_Data)[idx + 2] = (uint16)rgba.b;
			((uint16*)m_Buffer.m_Data)[idx + 3] = (uint16)rgba.a;
		}
	}
	
	glm::ivec4 OpenGLTexture2D::ReadPixel(uint32 row, uint32 column)
	{
		uint64 idx = (((uint64)row * (uint64)m_Width) + (uint64)column) * 4;

		if (idx >= (uint64)m_Width * (uint64)m_Height * 4 || idx < 0) return { 0, 0, 0, 0 };

		glm::ivec4 rgba = { 0, 0, 0, 0 };

		if (Format == TextureFormat::RGBA8)
		{
			rgba.r = (uint32)(((Byte*)m_Buffer.m_Data)[idx]);
			rgba.g = (uint32)(((Byte*)m_Buffer.m_Data)[idx + 1]);
			rgba.b = (uint32)(((Byte*)m_Buffer.m_Data)[idx + 2]);
			rgba.a = (uint32)(((Byte*)m_Buffer.m_Data)[idx + 3]);
		}
		else if (Format == TextureFormat::RGBA16)
		{
			rgba.r = (uint32)(((uint16*)m_Buffer.m_Data)[idx]);
			rgba.g = (uint32)(((uint16*)m_Buffer.m_Data)[idx + 1]);
			rgba.b = (uint32)(((uint16*)m_Buffer.m_Data)[idx + 2]);
			rgba.a = (uint32)(((uint16*)m_Buffer.m_Data)[idx + 3]);
		}

		return rgba;
	}

	uint32 OpenGLTexture2D::GetMipLevelCount()
	{
		return utils::CalculateMipCount(m_Width, m_Height);
	}
	
	void OpenGLTexture2D::Bind(uint32 slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, RendererID);
	}

	// ====================================================================================================================
	//												3D Texture
	// ====================================================================================================================

	/*OpenGLTexture3D* OpenGLTexture3D::LoadFromFiles(const std::vector<HLString>& filePaths)
	{
		if (filePaths.size() != 6)
		{
			HL_CORE_ERROR("Texture3D>    [-] Failed to load cube map. {0} out of 6 texture paths were provided [-]", filePaths.size());
			return nullptr;
		}

		std::vector<unsigned char*> texture_data;

		int width = 0, height = 0, channels;
		for (auto& path : filePaths)
		{
			stbi_set_flip_vertically_on_load(0);

			stbi_uc* data = stbi_load(path.C_Str(), &width, &height, &channels, STBI_rgb_alpha);
			texture_data.push_back(data);

			if (!data)
			{
				for (auto& tex : texture_data)
					stbi_image_free(tex);

				HL_CORE_ERROR("[-] Failed to load one of the textures in texture3D: {0} [-]", path.C_Str());
				return nullptr;
			}
		}

		OpenGLTexture3D* instance = new OpenGLTexture3D(ImageFormat::RGBA, width, height, texture_data);

		HL_CORE_INFO("Texture3D>    [+] Loaded 6 textures starting with {0} [+]", filePaths[0].C_Str());
		return instance;
	}*/

	OpenGLTexture3D::OpenGLTexture3D(const std::vector<HLString>& filePaths)
	{
		m_Format = TextureFormat::RGBA;

		glGenTextures(1, &RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, RendererID);

		stbi_set_flip_vertically_on_load(1);

		int32 width, height, nrComponents;
		for (uint64 i = 0; i < filePaths.size(); i++)
		{
			unsigned char* data = stbi_load(filePaths[i].C_Str(), &width, &height, &nrComponents, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + (uint32)i, 0, GL_RGB, (GLsizei)width, (GLsizei)height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				HL_CORE_ERROR("[-] Failed to load one of the textures in texture3D: {0} [-]", filePaths[i].C_Str());
				stbi_image_free(data);
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		HL_CORE_INFO("Texture3D>    [+] Loaded 6 textures starting with {0} [+]", filePaths[0].C_Str());
	}

	OpenGLTexture3D::OpenGLTexture3D(TextureFormat format, uint32 width, uint32 height, const void* data)
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
		
		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &RendererID);
		glTextureStorage2D(RendererID, levels, utils::OpenGLImageInternalFormat(m_Format), m_Width, m_Height);

		if (m_Buffer.m_Data)
			glTextureSubImage3D(RendererID, 0, 0, 0, 0, m_Width, m_Height, 6, utils::OpenGLImageFormat(m_Format), utils::OpenGLFormatDataType(m_Format), m_Buffer.m_Data);

		glTextureParameteri(RendererID, GL_TEXTURE_MIN_FILTER, levels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTextureParameteri(RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
	}

	OpenGLTexture3D::~OpenGLTexture3D()
	{
		glDeleteTextures(1, &RendererID);
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

	Allocator OpenGLTexture3D::GetWriteableBuffer()
	{
		return m_Buffer;
	}

	void OpenGLTexture3D::Release()
	{
		if (RendererID)
		{
			glDeleteTextures(1, &RendererID);
			RendererID = 0;
		}

		if (m_Buffer)
			m_Buffer.Release();
	}

	void OpenGLTexture3D::Invalidate()
	{
		if (RendererID)
			Release();

		uint32 levels = utils::CalculateMipCount(m_Width, m_Height);

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &RendererID);
		glTextureStorage2D(RendererID, levels, utils::OpenGLImageInternalFormat(m_Format), m_Width, m_Height);

		if (m_Buffer.m_Data)
			glTextureSubImage3D(RendererID, 0, 0, 0, 0, m_Width, m_Height, 6, utils::OpenGLImageFormat(m_Format), utils::OpenGLFormatDataType(m_Format), m_Buffer.m_Data);

		glTextureParameteri(RendererID, GL_TEXTURE_MIN_FILTER, levels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTextureParameteri(RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
	}

	void OpenGLTexture3D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
	{
		// @TODO
		HL_ASSERT(false, "Not implemented yet");
	}

	glm::ivec4 OpenGLTexture3D::ReadPixel(uint32 row, uint32 column)
	{
		// @TODO
		HL_ASSERT(false, "Not implemented yet");
		return {};
	}

	void OpenGLTexture3D::UpdateResourceData(void *data)
	{
		// @TODO
		HL_ASSERT(false, "Not implemented yet");
	}

	void OpenGLTexture3D::UpdateResourceData()
	{
		// @TODO
		HL_ASSERT(false, "Not implemented yet");
	}

	uint32 OpenGLTexture3D::GetMipLevelCount()
	{
		return utils::CalculateMipCount(m_Width, m_Height);
	}

	void OpenGLTexture3D::Bind(uint32 slot) const
	{
		glBindTextureUnit(slot, RendererID);
	}
}

#endif
