// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLTexture2D.h"

#ifdef HIGHLO_API_OPENGL

#include <stb_image.h>

#include "Engine/Utils/ImageUtils.h"
#include "Engine/Renderer/Renderer.h"

#include "OpenGLUtils.h"

#define TEXTURE2D_LOG_PREFIX "Texture2D>    "

namespace highlo
{	
	OpenGLTexture2D::OpenGLTexture2D(const FileSystemPath &filePath, TextureFormat format, bool flipOnLoad)
	{
		int32 width, height, channels;
		stbi_set_flip_vertically_on_load(flipOnLoad);

		if (*filePath.Absolute() == nullptr)
		{
			HL_CORE_ERROR("{0}[-] Failed to load Texture2D: {1} [-]", TEXTURE2D_LOG_PREFIX, *filePath.String());
			return;
		}

		stbi_uc *data = stbi_load(*filePath.Absolute(), &width, &height, &channels, STBI_rgb_alpha);
		if (!data)
		{
			stbi_image_free(data);
			HL_CORE_ERROR("{0}[-] Failed to load Texture2D: {1} (Error: {2}) [-]", TEXTURE2D_LOG_PREFIX, *filePath.String(), stbi_failure_reason());
			return;
		}

		Name = FileSystemPath::ExtractFileNameFromPath(filePath.String());
		m_Loaded = true;
		HL_CORE_INFO("{0}[+] Loaded {1} [+]", TEXTURE2D_LOG_PREFIX, *filePath.String());

		m_Buffer = Allocator::Copy(data, width * height * 4); // 4 byte per pixel
		m_InternalFormat = utils::OpenGLTextureInternalFormat(format);
		m_DataFormat = channels == 4 ? GL_RGBA : GL_RGB;

		// Clean up loaded data because it has been copied into our own allocator
		stbi_image_free(data);

		m_Specification.Width = width;
		m_Specification.Height = height;
		m_Specification.Format = format;
		m_Specification.Properties = TextureProperties();
		m_Specification.Usage = TextureUsage::Texture;
		m_Specification.Mips = utils::CalculateMipCount(width, height);

		glGenTextures(1, &RendererID);
		glBindTexture(GL_TEXTURE_2D, RendererID);

		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Buffer.Data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(m_Specification.Properties.SamplerWrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(m_Specification.Properties.SamplerWrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(m_Specification.Properties.SamplerFilter, false));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(m_Specification.Properties.SamplerFilter, false));

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	OpenGLTexture2D::OpenGLTexture2D(const glm::vec3 &rgb, TextureFormat format)
	{
		if (format == TextureFormat::RGBA8)
		{
			Byte data[4];
			data[0] = (Byte)rgb.x;
			data[1] = (Byte)rgb.y;
			data[2] = (Byte)rgb.z;
			data[3] = (Byte)255; // 2^8

			Name = "8-Bit Texture";
			m_Buffer = Allocator::Copy(data, 1 * 1 * 4); // 4 byte per pixel
		}
		else if (format == TextureFormat::RGBA16)
		{
			uint16 data[4];
			data[0] = (uint16)rgb.x;
			data[1] = (uint16)rgb.y;
			data[2] = (uint16)rgb.z;
			data[3] = (uint16)65535; // 2^16
			
			Name = "16-Bit Texture";
			m_Buffer = Allocator::Copy(data, 1 * 1 * 4); // 4 byte per pixel
		}
		else if (format == TextureFormat::RGBA32)
		{
			uint32 data[4];
			data[0] = (uint32)rgb.x;
			data[1] = (uint32)rgb.y;
			data[2] = (uint32)rgb.z;
			data[3] = (uint32)4294967295; // 2^32
			
			Name = "32-Bit Texture";
			m_Buffer = Allocator::Copy(data, 1 * 1 * 4); // 4 byte per pixel
		}

		m_Specification.Width = 1;
		m_Specification.Height = 1;
		m_Specification.Format = format;
		m_Specification.Properties = TextureProperties();
		m_Specification.Usage = TextureUsage::Texture;
		m_Specification.Mips = utils::CalculateMipCount(1, 1);

		m_InternalFormat = utils::OpenGLTextureInternalFormat(format);
		m_DataFormat = GL_RGBA;
		m_Loaded = true;

		glGenTextures(1, &RendererID);
		glBindTexture(GL_TEXTURE_2D, RendererID);

		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Buffer.Data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(m_Specification.Properties.SamplerWrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(m_Specification.Properties.SamplerWrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(m_Specification.Properties.SamplerFilter, false));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(m_Specification.Properties.SamplerFilter, false));

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	OpenGLTexture2D::OpenGLTexture2D(const glm::vec3 &rgb, uint32 width, uint32 height, TextureFormat format)
	{
		uint64 row = 0, column = 0, idx = 0;

	#pragma warning(push)
	#pragma warning(disable : 6386)
		if (format == TextureFormat::RGBA8)
		{
			Byte *data = new Byte[(uint64)width * (uint64)height * (uint64)4];

			for (row = 0; row < (uint64) width * 4; ++row)
			{
				for (column = 0; column < (uint64) height; column += 4)
				{
					idx = row * width + column;
					data[idx] = (Byte) (uint32) rgb.r;
					data[idx + 1] = (Byte) (uint32) rgb.g;
					data[idx + 2] = (Byte) (uint32) rgb.b;
					data[idx + 3] = (Byte) 255; // 2^8
				}
			}

			Name = "8-Bit Texture";
			m_Buffer = Allocator::Copy(data, width * height * 4); // 4 byte per pixel
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
					data[idx] = (uint16) (uint32) rgb.r;
					data[idx + 1] = (uint16) (uint32) rgb.g;
					data[idx + 2] = (uint16) (uint32) rgb.b;
					data[idx + 3] = (uint16) 65535; // 2^16
				}
			}

			Name = "16-Bit Texture";
			m_Buffer = Allocator::Copy(data, width * height * 4); // 4 byte per pixel
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
					data[idx] = (uint32) rgb.r;
					data[idx + 1] = (uint32) rgb.g;
					data[idx + 2] = (uint32) rgb.b;
					data[idx + 3] = (uint32) 4294967295; // 2^32
				}
			}

			Name = "32-Bit Texture";
			m_Buffer = Allocator::Copy(data, width * height * 4); // 4 byte per pixel
			delete[] data;
			}
	#pragma warning(pop)

		m_Specification.Width = width;
		m_Specification.Height = height;
		m_Specification.Format = format;
		m_Specification.Properties = TextureProperties();
		m_Specification.Usage = TextureUsage::Texture;
		m_Specification.Mips = utils::CalculateMipCount(width, height);

		m_InternalFormat = utils::OpenGLTextureInternalFormat(format);
		m_DataFormat = GL_RGBA;
		m_Loaded = true;

		glGenTextures(1, &RendererID);
		glBindTexture(GL_TEXTURE_2D, RendererID);

		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Buffer.Data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(m_Specification.Properties.SamplerWrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(m_Specification.Properties.SamplerWrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(m_Specification.Properties.SamplerFilter, false));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(m_Specification.Properties.SamplerFilter, false));

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	OpenGLTexture2D::OpenGLTexture2D(void *img_data, uint32 width, uint32 height, TextureFormat format)
	{
		m_Buffer = Allocator::Copy(img_data, width * height * 4); // 4 byte per pixel
		m_InternalFormat = utils::OpenGLTextureInternalFormat(format);
		m_DataFormat = GL_RGBA;

		m_Specification.Width = width;
		m_Specification.Height = height;
		m_Specification.Format = format;
		m_Specification.Properties = TextureProperties();
		m_Specification.Usage = TextureUsage::Texture;
		m_Specification.Mips = utils::CalculateMipCount(width, height);
		Name = "unknown";
		m_Loaded = true;

		glGenTextures(1, &RendererID);
		glBindTexture(GL_TEXTURE_2D, RendererID);

		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Buffer.Data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(m_Specification.Properties.SamplerWrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(m_Specification.Properties.SamplerWrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(m_Specification.Properties.SamplerFilter, false));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(m_Specification.Properties.SamplerFilter, false));

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	OpenGLTexture2D::OpenGLTexture2D(TextureFormat format, uint32 width, uint32 height, const void *data, TextureProperties props)
	{
		m_InternalFormat = utils::OpenGLTextureInternalFormat(format);
		m_Buffer = Allocator::Copy(data, width * height * 4);

		m_Specification.Width = width;
		m_Specification.Height = height;
		m_Specification.Format = format;
		m_Specification.Properties = props;
		m_Specification.Usage = TextureUsage::Texture;
		m_Specification.Mips = utils::CalculateMipCount(width, height);
		Name = "unknown";
		m_Loaded = true;

		glGenTextures(1, &RendererID);
		glBindTexture(GL_TEXTURE_2D, RendererID);

		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Buffer.Data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(m_Specification.Properties.SamplerWrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(m_Specification.Properties.SamplerWrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(m_Specification.Properties.SamplerFilter, false));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(m_Specification.Properties.SamplerFilter, false));

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	OpenGLTexture2D::OpenGLTexture2D(TextureFormat format, uint32 width, uint32 height)
	{
		m_InternalFormat = utils::OpenGLTextureInternalFormat(format);
		m_Buffer.Allocate(width * height * 4); // * 4 channels (RGBA)
		m_DataFormat = GL_RGBA;

		m_Specification.Width = width;
		m_Specification.Height = height;
		m_Specification.Format = format;
		m_Specification.Usage = TextureUsage::Texture;
		m_Specification.Properties = TextureProperties();
		m_Specification.Mips = utils::CalculateMipCount(width, height);
		Name = "unknown";
		m_Loaded = true;

		glGenTextures(1, &RendererID);
		glBindTexture(GL_TEXTURE_2D, RendererID);

		auto glFormat = utils::OpenGLTextureFormat(m_Specification.Format);
		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Specification.Width, m_Specification.Height, 0, glFormat, (glFormat == GL_DEPTH_STENCIL) ? GL_UNSIGNED_INT_24_8 : GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(m_Specification.Properties.SamplerWrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(m_Specification.Properties.SamplerWrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(m_Specification.Properties.SamplerFilter, false));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(m_Specification.Properties.SamplerFilter, false));

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification &spec)
	{
		m_Specification = spec;
		Name = "unknown";
		m_Loaded = true;
		m_InternalFormat = utils::OpenGLTextureInternalFormat(spec.Format);

		glGenTextures(1, &RendererID);
		glBindTexture(GL_TEXTURE_2D, RendererID);

		auto glFormat = utils::OpenGLTextureFormat(m_Specification.Format);
		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Specification.Width, m_Specification.Height, 0, glFormat, (glFormat == GL_DEPTH_STENCIL) ? GL_UNSIGNED_INT_24_8 : GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(m_Specification.Properties.SamplerWrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(m_Specification.Properties.SamplerWrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(m_Specification.Properties.SamplerFilter, false));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(m_Specification.Properties.SamplerFilter, false));

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		Release();

		if (m_Buffer)
			m_Buffer.Release();
	}

	Allocator OpenGLTexture2D::GetData()
	{
		HL_ASSERT(m_Locked, "Texture must be locked!");
		return m_Buffer;
	}

	void OpenGLTexture2D::Release()
	{
		if (RendererID)
		{
			glDeleteTextures(1, &RendererID);
			RendererID = 0;
			m_Loaded = false;
		}
	}

	void OpenGLTexture2D::Invalidate()
	{
		Release();

		GLenum glInternalFormat = utils::OpenGLTextureInternalFormat(m_Specification.Format);
		GLenum glFormat = utils::OpenGLTextureFormat(m_Specification.Format);
		GLenum glType = utils::OpenGLFormatDataType(m_Specification.Format);
		uint32 mipCount = utils::CalculateMipCount(m_Specification.Width, m_Specification.Height);

		glGenTextures(1, &RendererID);
		glBindTexture(GL_TEXTURE_2D, RendererID);
		glTexStorage2D(GL_TEXTURE_2D, mipCount, glInternalFormat, m_Specification.Width, m_Specification.Height);

		if (m_Buffer)
		{
			glTextureSubImage2D(RendererID, 0, 0, 0, m_Specification.Width, m_Specification.Height, glFormat, glType, m_Buffer.Data);
			glGenerateMipmap(GL_TEXTURE_2D);
			m_Loaded = true;
		}
	}

	void OpenGLTexture2D::Lock()
	{
		m_Locked = true;
	}

	void OpenGLTexture2D::Unlock()
	{
		m_Locked = false;
		glTextureSubImage2D(GetRendererID(), 0, 0, 0, m_Specification.Width, m_Specification.Height, utils::OpenGLTextureFormat(m_Specification.Format), GL_UNSIGNED_BYTE, m_Buffer.Data);
	}

	void OpenGLTexture2D::CreatePerLayerImageViews()
	{
	}

	void OpenGLTexture2D::CreateSampler(TextureProperties properties)
	{
		glGenSamplers(1, &m_SamplerRendererID);

		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(properties.SamplerFilter, false));
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(properties.SamplerFilter, false));
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_WRAP_R, utils::OpenGLSamplerWrap(properties.SamplerWrap));
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(properties.SamplerWrap));
		glSamplerParameteri(m_SamplerRendererID, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(properties.SamplerWrap));
	}
	
	void OpenGLTexture2D::UpdateResourceData()
	{
		UpdateResourceData(m_Buffer.Data);
	}
	
	void OpenGLTexture2D::UpdateResourceData(void *data)
	{
		glTextureSubImage2D(RendererID, 0, 0, 0, m_Specification.Width, m_Specification.Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}
	
	void OpenGLTexture2D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
	{
		uint64 idx = (((uint64)row * (uint64)m_Specification.Width) + (uint64)column) * 4;
		if (idx >= (uint64)m_Specification.Width * (uint64)m_Specification.Height * 4 || idx < 0) return;

		if (m_Specification.Format == TextureFormat::RGBA8)
		{
			((Byte*)m_Buffer.Data)[idx]	  = (Byte)rgba.r;
			((Byte*)m_Buffer.Data)[idx + 1] = (Byte)rgba.g;
			((Byte*)m_Buffer.Data)[idx + 2] = (Byte)rgba.b;
			((Byte*)m_Buffer.Data)[idx + 3] = (Byte)rgba.a;
		}
		else if (m_Specification.Format == TextureFormat::RGBA16)
		{
			((uint16*)m_Buffer.Data)[idx]		= (uint16)rgba.r;
			((uint16*)m_Buffer.Data)[idx + 1] = (uint16)rgba.g;
			((uint16*)m_Buffer.Data)[idx + 2] = (uint16)rgba.b;
			((uint16*)m_Buffer.Data)[idx + 3] = (uint16)rgba.a;
		}
		else if (m_Specification.Format == TextureFormat::RGBA32)
		{
			((uint32*)m_Buffer.Data)[idx] = (uint32)rgba.r;
			((uint32*)m_Buffer.Data)[idx + 1] = (uint32)rgba.g;
			((uint32*)m_Buffer.Data)[idx + 2] = (uint32)rgba.b;
			((uint32*)m_Buffer.Data)[idx + 3] = (uint32)rgba.a;
		}
	}
	
	glm::ivec4 OpenGLTexture2D::ReadPixel(uint32 row, uint32 column)
	{
		uint64 idx = (((uint64)row * (uint64)m_Specification.Width) + (uint64)column) * 4;
		if (idx >= (uint64)m_Specification.Width * (uint64)m_Specification.Height * 4 || idx < 0) return { 0, 0, 0, 0 };

		glm::ivec4 rgba = { 0, 0, 0, 0 };

		if (m_Specification.Format == TextureFormat::RGBA8)
		{
			rgba.r = (uint32)(((Byte*)m_Buffer.Data)[idx]);
			rgba.g = (uint32)(((Byte*)m_Buffer.Data)[idx + 1]);
			rgba.b = (uint32)(((Byte*)m_Buffer.Data)[idx + 2]);
			rgba.a = (uint32)(((Byte*)m_Buffer.Data)[idx + 3]);
		}
		else if (m_Specification.Format == TextureFormat::RGBA16)
		{
			rgba.r = (uint32)(((uint16*)m_Buffer.Data)[idx]);
			rgba.g = (uint32)(((uint16*)m_Buffer.Data)[idx + 1]);
			rgba.b = (uint32)(((uint16*)m_Buffer.Data)[idx + 2]);
			rgba.a = (uint32)(((uint16*)m_Buffer.Data)[idx + 3]);
		}
		else if (m_Specification.Format == TextureFormat::RGBA32)
		{
			rgba.r = (uint32)(((uint32*)m_Buffer.Data)[idx]);
			rgba.g = (uint32)(((uint32*)m_Buffer.Data)[idx + 1]);
			rgba.b = (uint32)(((uint32*)m_Buffer.Data)[idx + 2]);
			rgba.a = (uint32)(((uint32*)m_Buffer.Data)[idx + 3]);
		}

		return rgba;
	}

	uint32 OpenGLTexture2D::GetMipLevelCount()
	{
		return utils::CalculateMipCount(m_Specification.Width, m_Specification.Height);
	}

	std::pair<uint32, uint32> OpenGLTexture2D::GetMipSize(uint32 mip)
	{
		return utils::GetMipSize(mip, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLTexture2D::GenerateMips()
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	
	void OpenGLTexture2D::Bind(uint32 slot) const
	{
		glBindTextureUnit(slot, RendererID);
	}

	void OpenGLTexture2D::Unbind(uint32 slot) const
	{
		glBindTextureUnit(slot, 0);
	}
}

#endif // HIGHLO_API_OPENGL

