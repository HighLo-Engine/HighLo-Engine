// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLTexture.h"

#ifdef HIGHLO_API_OPENGL

#include <stb_image.h>

#include "Engine/Core/Log.h"
#include "Engine/Core/File.h"
#include "Engine/Utils/ImageUtils.h"
#include "OpenGLUtils.h"

namespace highlo
{
	OpenGLTexture2D *OpenGLTexture2D::LoadFromFile(const HLString &filepath, TextureFormat format, bool flip_on_load)
	{
		int32 width, height, channels;
		stbi_set_flip_vertically_on_load(flip_on_load);

		stbi_uc *data = stbi_load(*filepath, &width, &height, &channels, STBI_rgb_alpha);
		if (!data)
		{
			stbi_image_free(data);
			HL_CORE_ERROR("Texture2D> [-] Failed to load Texture2D: " + filepath + " [-]");
			return nullptr;
		}

		OpenGLTexture2D *instance = new OpenGLTexture2D(data, width, height, format);
		instance->Name = File::GetFileName(filepath);
		HL_CORE_INFO("Texture2D>    [+] Loaded " + filepath + " [+]");

		return instance;
	}

	OpenGLTexture2D *OpenGLTexture2D::CreateFromColor(const glm::vec3 &rgb, TextureFormat format)
	{
		OpenGLTexture2D *texture = nullptr;

		if (format == TextureFormat::RGBA8)
		{
			Byte data[4];
			data[0] = (Byte)rgb.x;
			data[1] = (Byte)rgb.y;
			data[2] = (Byte)rgb.z;
			data[3] = (Byte)255; // 2^8
			texture = new OpenGLTexture2D(data, 1, 1, format);
			texture->Name = "8-Bit Texture";
		}
		else if (format == TextureFormat::RGBA16)
		{
			uint16 data[4];
			data[0] = (uint16)rgb.x;
			data[1] = (uint16)rgb.y;
			data[2] = (uint16)rgb.z;
			data[3] = (uint16)65535; // 2^16
			texture = new OpenGLTexture2D(data, 1, 1, format);
			texture->Name = "16-Bit Texture";
		}
		else if (format == TextureFormat::RGBA32)
		{
			uint32 data[4];
			data[0] = (uint32)rgb.x;
			data[1] = (uint32)rgb.y;
			data[2] = (uint32)rgb.z;
			data[3] = (uint32)4294967295; // 2^32
			texture = new OpenGLTexture2D(data, 1, 1, format);
			texture->Name = "32-Bit Texture";
		}

		return texture;
	}
	
	OpenGLTexture2D *OpenGLTexture2D::CreateFromColor(const glm::vec3 &rgb, uint32 width, uint32 height, TextureFormat format)
	{
		OpenGLTexture2D *instance = nullptr;
		uint64 row = 0, column = 0, idx = 0;

#pragma warning( push )
#pragma warning( disable : 6386)
		if (format == TextureFormat::RGBA8)
		{
			Byte *data = new Byte[(uint64)width * (uint64)height * (uint64)4];

			for (row = 0; row < (uint64)width * 4; ++row)
			{
				for (column = 0; column < (uint64)height; column += 4)
				{
					idx = row * width + column;
					data[idx] = (Byte)(uint32)rgb.r;
					data[idx + 1] = (Byte)(uint32)rgb.g;
					data[idx + 2] = (Byte)(uint32)rgb.b;
					data[idx + 3] = (Byte)255; // 2^8
				}
			}

			instance = new OpenGLTexture2D(data, width, height, format);
			instance->Name = "8-Bit Texture";
			delete[] data;
		}
		else if (format == TextureFormat::RGBA16)
		{
			uint16 *data = new uint16[(uint64)width * (uint64)height * (uint64)4];

			for (row = 0; row < (uint64)width * 4; ++row)
			{
				for (column = 0; column < (uint64)height; column += 4)
				{
					idx = row * width + column;
					data[idx] = (uint16)(uint32)rgb.r;
					data[idx + 1] = (uint16)(uint32)rgb.g;
					data[idx + 2] = (uint16)(uint32)rgb.b;
					data[idx + 3] = (uint16)65535; // 2^16
				}
			}

			instance = new OpenGLTexture2D(data, width, height, format);
			instance->Name = "16-Bit Texture";
			delete[] data;
		}
		else if (format == TextureFormat::RGBA32)
		{
			uint32 *data = new uint32[(uint64)width * (uint64)height * (uint64)4];

			for (row = 0; row < (uint64)width * 4; ++row)
				{
				for (column = 0; column < (uint64)height; column += 4)
					{
					idx = row * width + column;
					data[idx] = (uint32)rgb.r;
					data[idx + 1] = (uint32)rgb.g;
					data[idx + 2] = (uint32)rgb.b;
					data[idx + 3] = (uint32)4294967295; // 2^32
					}
				}

			instance = new OpenGLTexture2D(data, width, height, format);
			instance->Name = "32-Bit Texture";
			delete[] data;
		}
#pragma warning( pop )

		return instance;
	}
	
	OpenGLTexture2D::OpenGLTexture2D(void *img_data, uint32 width, uint32 height, TextureFormat format)
		: m_Width(width), m_Height(height)
	{
		m_Buffer = Allocator::Copy(img_data, (uint64)width * (uint64)height * (uint64)4); // 4 byte per pixel
		m_InternalFormat = utils::OpenGLTextureInternalFormat(format);
		Format = format;
		m_DataFormat = GL_RGBA;

		m_Specification.Width = width;
		m_Specification.Height = height;
		m_Specification.Format = Format;
		m_Specification.Properties = TextureProperties();
		m_Specification.Usage = TextureUsage::Texture;
		m_Specification.Mips = utils::CalculateMipCount(width, height);
		Name = "unknown";

		glGenTextures(1, &RendererID);
		glBindTexture(GL_TEXTURE_2D, RendererID);
		
		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Buffer.m_Data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	OpenGLTexture2D::OpenGLTexture2D(TextureFormat format, uint32 width, uint32 height, const void *data, TextureProperties props)
		: m_Width(width), m_Height(height)
	{
		m_InternalFormat = utils::OpenGLTextureInternalFormat(format);
		Format = format;

		m_Specification.Width = width;
		m_Specification.Height = height;
		m_Specification.Format = Format;
		m_Specification.Properties = props;
		m_Specification.Usage = TextureUsage::Texture;
		m_Specification.Mips = utils::CalculateMipCount(width, height);
		Name = "unknown";

		glGenTextures(1, &RendererID);
		glBindTexture(GL_TEXTURE_2D, RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	OpenGLTexture2D::OpenGLTexture2D(TextureFormat format, uint32 width, uint32 height)
	{
		m_InternalFormat = utils::OpenGLTextureInternalFormat(format);
		m_Buffer.Allocate(width * height * 4);
		Format = format;
		m_Width = width;
		m_Height = height;
		m_DataFormat = GL_RGBA;

		m_Specification.Width = width;
		m_Specification.Height = height;
		m_Specification.Format = Format;
		m_Specification.Usage = TextureUsage::Texture;
		m_Specification.Properties = TextureProperties();
		m_Specification.Mips = utils::CalculateMipCount(width, height);
		Name = "unknown";

		glGenTextures(1, &RendererID);
		glBindTexture(GL_TEXTURE_2D, RendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &RendererID);
		RendererID = 0;

		if (m_Buffer)
			m_Buffer.Release();
	}

	Allocator OpenGLTexture2D::GetData()
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
		Release();

		glCreateTextures(GL_TEXTURE_2D, 1, &RendererID);
		GLenum glInternalFormat = utils::OpenGLTextureInternalFormat(Format);
		uint32 mipCount = utils::CalculateMipCount(m_Width, m_Height);
		glTextureStorage2D(RendererID, mipCount, glInternalFormat, m_Width, m_Height);

		if (m_Buffer)
		{
			GLenum glFormat = utils::OpenGLTextureFormat(Format);
			GLenum glDataType = utils::OpenGLFormatDataType(Format);
			glTextureSubImage2D(RendererID, 0, 0, 0, m_Width, m_Height, glFormat, glDataType, m_Buffer.m_Data);
			glGenerateTextureMipmap(RendererID);
		}
	}

	void OpenGLTexture2D::CreatePerLayerImageViews()
	{
		HL_ASSERT(false, "Unsupported in OpenGL");
	}

	void OpenGLTexture2D::CreateSampler(TextureProperties properties)
	{
		glCreateSamplers(1, &m_SamplerRendererID);
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(properties.SamplerFilter, properties.GenerateMips));
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(properties.SamplerFilter, false));
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_WRAP_R, utils::OpenGLSamplerWrap(properties.SamplerWrap));
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(properties.SamplerWrap));
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(properties.SamplerWrap));
	}
	
	void OpenGLTexture2D::UpdateResourceData()
	{
		UpdateResourceData(m_Buffer.m_Data);
	}
	
	void OpenGLTexture2D::UpdateResourceData(void *data)
	{
		glTextureSubImage2D(RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}
	
	void OpenGLTexture2D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
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
		else if (Format == TextureFormat::RGBA16)
		{
			((uint16*)m_Buffer.m_Data)[idx]		= (uint16)rgba.r;
			((uint16*)m_Buffer.m_Data)[idx + 1] = (uint16)rgba.g;
			((uint16*)m_Buffer.m_Data)[idx + 2] = (uint16)rgba.b;
			((uint16*)m_Buffer.m_Data)[idx + 3] = (uint16)rgba.a;
		}
		else if (Format == TextureFormat::RGBA32)
		{
			((uint32*)m_Buffer.m_Data)[idx] = (uint32)rgba.r;
			((uint32*)m_Buffer.m_Data)[idx + 1] = (uint32)rgba.g;
			((uint32*)m_Buffer.m_Data)[idx + 2] = (uint32)rgba.b;
			((uint32*)m_Buffer.m_Data)[idx + 3] = (uint32)rgba.a;
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
		else if (Format == TextureFormat::RGBA32)
		{
			rgba.r = (uint32)(((uint32*)m_Buffer.m_Data)[idx]);
			rgba.g = (uint32)(((uint32*)m_Buffer.m_Data)[idx + 1]);
			rgba.b = (uint32)(((uint32*)m_Buffer.m_Data)[idx + 2]);
			rgba.a = (uint32)(((uint32*)m_Buffer.m_Data)[idx + 3]);
		}

		return rgba;
	}

	uint32 OpenGLTexture2D::GetMipLevelCount()
	{
		return utils::CalculateMipCount(m_Width, m_Height);
	}
	
	void OpenGLTexture2D::Bind(uint32 slot) const
	{
		glBindTextureUnit(slot, RendererID);
	}

	// ====================================================================================================================
	//												3D Texture
	// ====================================================================================================================

	OpenGLTexture3D::OpenGLTexture3D(const std::vector<HLString>& filePaths)
	{
		m_Format = TextureFormat::RGBA;

		glGenTextures(1, &RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, RendererID);

		stbi_set_flip_vertically_on_load(1);

		int32 width, height, nrComponents;
		for (uint64 i = 0; i < filePaths.size(); i++)
		{
			Byte *data = stbi_load(filePaths[i].C_Str(), &width, &height, &nrComponents, 0);
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
		glTextureStorage2D(RendererID, levels, utils::OpenGLTextureInternalFormat(m_Format), m_Width, m_Height);

		if (m_Buffer.m_Data)
			glTextureSubImage3D(RendererID, 0, 0, 0, 0, m_Width, m_Height, 6, utils::OpenGLTextureFormat(m_Format), utils::OpenGLFormatDataType(m_Format), m_Buffer.m_Data);

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

	Allocator OpenGLTexture3D::GetData()
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
	}

	void OpenGLTexture3D::Invalidate()
	{
		Release();

		uint32 levels = utils::CalculateMipCount(m_Width, m_Height);

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &RendererID);
		glTextureStorage2D(RendererID, levels, utils::OpenGLTextureInternalFormat(m_Format), m_Width, m_Height);

		if (m_Buffer.m_Data)
			glTextureSubImage3D(RendererID, 0, 0, 0, 0, m_Width, m_Height, 6, utils::OpenGLTextureFormat(m_Format), utils::OpenGLFormatDataType(m_Format), m_Buffer.m_Data);

		glTextureParameteri(RendererID, GL_TEXTURE_MIN_FILTER, levels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTextureParameteri(RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
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
		UpdateResourceData(m_Buffer.m_Data);
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
