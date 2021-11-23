// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLTexture.h"

#ifdef HIGHLO_API_OPENGL

#include <stb_image.h>

#include "Engine/Utils/ImageUtils.h"
#include "Engine/Renderer/Renderer.h"

#include "OpenGLUtils.h"

#define TEXTURE2D_LOG_PREFIX "Texture2D>    "
#define TEXTURE3D_LOG_PREFIX "Texture3D>    "

namespace highlo
{	
	OpenGLTexture2D::OpenGLTexture2D(const FileSystemPath &filePath, TextureFormat format, bool flipOnLoad)
	{
		int32 width, height, channels;
		stbi_set_flip_vertically_on_load(flipOnLoad);

		stbi_uc *data = stbi_load(*filePath.Absolute(), &width, &height, &channels, STBI_rgb_alpha);
		if (!data)
		{
			stbi_image_free(data);
			HL_CORE_ERROR("{0}[-] Failed to load Texture2D: {1} [-]", TEXTURE2D_LOG_PREFIX, *filePath.String());
		}

		Name = FileSystemPath::ExtractFileNameFromPath(filePath.String());
		m_Loaded = true;
		HL_CORE_INFO("{0}[+] Loaded {1} [+]", TEXTURE2D_LOG_PREFIX, *filePath.String());

		m_Buffer = Allocator::Copy(data, width * height * 4); // 4 byte per pixel
		m_InternalFormat = utils::OpenGLTextureInternalFormat(format);
		m_DataFormat = channels == 4 ? GL_RGBA : GL_RGB;

		m_Specification.Width = width;
		m_Specification.Height = height;
		m_Specification.Format = format;
		m_Specification.Properties = TextureProperties();
		m_Specification.Usage = TextureUsage::Texture;
		m_Specification.Mips = utils::CalculateMipCount(width, height);

		Ref<OpenGLTexture2D> instance = this;
		Renderer::Submit([instance]() mutable
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &instance->RendererID);
			glBindTexture(GL_TEXTURE_2D, instance->RendererID);

			glTexImage2D(GL_TEXTURE_2D, 0, instance->m_InternalFormat, instance->m_Specification.Width, instance->m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, instance->m_Buffer.m_Data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(instance->m_Specification.Properties.SamplerFilter, false));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(instance->m_Specification.Properties.SamplerFilter, false));

			glBindTexture(GL_TEXTURE_2D, 0);
		});
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

		Ref<OpenGLTexture2D> instance = this;
		Renderer::Submit([instance]() mutable
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &instance->RendererID);
			glBindTexture(GL_TEXTURE_2D, instance->RendererID);

			glTexImage2D(GL_TEXTURE_2D, 0, instance->m_InternalFormat, instance->m_Specification.Width, instance->m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, instance->m_Buffer.m_Data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(instance->m_Specification.Properties.SamplerFilter, false));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(instance->m_Specification.Properties.SamplerFilter, false));

			glBindTexture(GL_TEXTURE_2D, 0);
		});
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

		Ref<OpenGLTexture2D> instance = this;
		Renderer::Submit([instance]() mutable
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &instance->RendererID);
			glBindTexture(GL_TEXTURE_2D, instance->RendererID);

			glTexImage2D(GL_TEXTURE_2D, 0, instance->m_InternalFormat, instance->m_Specification.Width, instance->m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, instance->m_Buffer.m_Data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(instance->m_Specification.Properties.SamplerFilter, false));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(instance->m_Specification.Properties.SamplerFilter, false));

			glBindTexture(GL_TEXTURE_2D, 0);
		});
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

		Ref<OpenGLTexture2D> instance = this;
		Renderer::Submit([instance]() mutable
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &instance->RendererID);
			glBindTexture(GL_TEXTURE_2D, instance->RendererID);

			glTexImage2D(GL_TEXTURE_2D, 0, instance->m_InternalFormat, instance->m_Specification.Width, instance->m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, instance->m_Buffer.m_Data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(instance->m_Specification.Properties.SamplerFilter, false));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(instance->m_Specification.Properties.SamplerFilter, false));

			glBindTexture(GL_TEXTURE_2D, 0);
		});
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

		Ref<OpenGLTexture2D> instance = this;
		Renderer::Submit([instance]() mutable
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &instance->RendererID);
			glBindTexture(GL_TEXTURE_2D, instance->RendererID);

			glTexImage2D(GL_TEXTURE_2D, 0, instance->m_InternalFormat, instance->m_Specification.Width, instance->m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, instance->m_Buffer.m_Data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(instance->m_Specification.Properties.SamplerFilter, false));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(instance->m_Specification.Properties.SamplerFilter, false));

			glBindTexture(GL_TEXTURE_2D, 0);
		});
	}

	OpenGLTexture2D::OpenGLTexture2D(TextureFormat format, uint32 width, uint32 height)
	{
		m_InternalFormat = utils::OpenGLTextureInternalFormat(format);
		m_Buffer.Allocate(width * height * 4);
		m_DataFormat = GL_RGBA;

		m_Specification.Width = width;
		m_Specification.Height = height;
		m_Specification.Format = format;
		m_Specification.Usage = TextureUsage::Texture;
		m_Specification.Properties = TextureProperties();
		m_Specification.Mips = utils::CalculateMipCount(width, height);
		Name = "unknown";
		m_Loaded = true;

		Ref<OpenGLTexture2D> instance = this;
		Renderer::Submit([instance]() mutable
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &instance->RendererID);
			glBindTexture(GL_TEXTURE_2D, instance->RendererID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(instance->m_Specification.Properties.SamplerFilter, false));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(instance->m_Specification.Properties.SamplerFilter, false));

			glBindTexture(GL_TEXTURE_2D, 0);
		});
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
			GLuint rendererID = RendererID;
			Renderer::Submit([rendererID]()
			{
				glDeleteTextures(1, &rendererID);
			});
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

		Ref<OpenGLTexture2D> instance = this;
		Renderer::Submit([instance, mipCount, glInternalFormat, glFormat, glType]() mutable
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &instance->RendererID);
			glTexStorage2D(instance->RendererID, mipCount, glInternalFormat, instance->m_Specification.Width, instance->m_Specification.Height);

			if (instance->m_Buffer)
			{
				glTextureSubImage2D(instance->RendererID, 0, 0, 0, instance->m_Specification.Width, instance->m_Specification.Height, glFormat, glType, instance->m_Buffer.m_Data);
				glGenerateTextureMipmap(instance->RendererID);
				instance->m_Loaded = true;
			}
		});
	}

	void OpenGLTexture2D::Lock()
	{
		m_Locked = true;
	}

	void OpenGLTexture2D::Unlock()
	{
		m_Locked = false;
		Ref<OpenGLTexture2D> instance = this;
		Renderer::Submit([instance]()
		{
			glTextureSubImage2D(instance->GetRendererID(), 0, 0, 0, instance->m_Specification.Width, instance->m_Specification.Height, utils::OpenGLTextureFormat(instance->m_Specification.Format), GL_UNSIGNED_BYTE, instance->m_Buffer.m_Data);
		});

	}

	void OpenGLTexture2D::CreatePerLayerImageViews()
	{
		HL_ASSERT(false, "Unsupported in OpenGL");
	}

	void OpenGLTexture2D::CreateSampler(TextureProperties properties)
	{
		Ref<OpenGLTexture2D> instance = this;
		Renderer::Submit([instance, properties]() mutable
		{
			 glCreateSamplers(1, &instance->m_SamplerRendererID);

			 glSamplerParameteri(instance->m_SamplerRendererID, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(properties.SamplerFilter, properties.GenerateMips));
			 glSamplerParameteri(instance->m_SamplerRendererID, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(properties.SamplerFilter, false));
			 glSamplerParameteri(instance->m_SamplerRendererID, GL_TEXTURE_WRAP_R, utils::OpenGLSamplerWrap(properties.SamplerWrap));
			 glSamplerParameteri(instance->m_SamplerRendererID, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(properties.SamplerWrap));
			 glSamplerParameteri(instance->m_SamplerRendererID, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(properties.SamplerWrap));
		});
	}
	
	void OpenGLTexture2D::UpdateResourceData()
	{
		UpdateResourceData(m_Buffer.m_Data);
	}
	
	void OpenGLTexture2D::UpdateResourceData(void *data)
	{
		Ref<OpenGLTexture2D> instance = this;
		Renderer::Submit([instance, data]()
		{
			glTextureSubImage2D(instance->RendererID, 0, 0, 0, instance->m_Specification.Width, instance->m_Specification.Height, instance->m_DataFormat, GL_UNSIGNED_BYTE, data);
		});
	}
	
	void OpenGLTexture2D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
	{
		uint64 idx = (((uint64)row * (uint64)m_Specification.Width) + (uint64)column) * 4;
		if (idx >= (uint64)m_Specification.Width * (uint64)m_Specification.Height * 4 || idx < 0) return;

		if (m_Specification.Format == TextureFormat::RGBA8)
		{
			((Byte*)m_Buffer.m_Data)[idx]	  = (Byte)rgba.r;
			((Byte*)m_Buffer.m_Data)[idx + 1] = (Byte)rgba.g;
			((Byte*)m_Buffer.m_Data)[idx + 2] = (Byte)rgba.b;
			((Byte*)m_Buffer.m_Data)[idx + 3] = (Byte)rgba.a;
		}
		else if (m_Specification.Format == TextureFormat::RGBA16)
		{
			((uint16*)m_Buffer.m_Data)[idx]		= (uint16)rgba.r;
			((uint16*)m_Buffer.m_Data)[idx + 1] = (uint16)rgba.g;
			((uint16*)m_Buffer.m_Data)[idx + 2] = (uint16)rgba.b;
			((uint16*)m_Buffer.m_Data)[idx + 3] = (uint16)rgba.a;
		}
		else if (m_Specification.Format == TextureFormat::RGBA32)
		{
			((uint32*)m_Buffer.m_Data)[idx] = (uint32)rgba.r;
			((uint32*)m_Buffer.m_Data)[idx + 1] = (uint32)rgba.g;
			((uint32*)m_Buffer.m_Data)[idx + 2] = (uint32)rgba.b;
			((uint32*)m_Buffer.m_Data)[idx + 3] = (uint32)rgba.a;
		}
	}
	
	glm::ivec4 OpenGLTexture2D::ReadPixel(uint32 row, uint32 column)
	{
		uint64 idx = (((uint64)row * (uint64)m_Specification.Width) + (uint64)column) * 4;
		if (idx >= (uint64)m_Specification.Width * (uint64)m_Specification.Height * 4 || idx < 0) return { 0, 0, 0, 0 };

		glm::ivec4 rgba = { 0, 0, 0, 0 };

		if (m_Specification.Format == TextureFormat::RGBA8)
		{
			rgba.r = (uint32)(((Byte*)m_Buffer.m_Data)[idx]);
			rgba.g = (uint32)(((Byte*)m_Buffer.m_Data)[idx + 1]);
			rgba.b = (uint32)(((Byte*)m_Buffer.m_Data)[idx + 2]);
			rgba.a = (uint32)(((Byte*)m_Buffer.m_Data)[idx + 3]);
		}
		else if (m_Specification.Format == TextureFormat::RGBA16)
		{
			rgba.r = (uint32)(((uint16*)m_Buffer.m_Data)[idx]);
			rgba.g = (uint32)(((uint16*)m_Buffer.m_Data)[idx + 1]);
			rgba.b = (uint32)(((uint16*)m_Buffer.m_Data)[idx + 2]);
			rgba.a = (uint32)(((uint16*)m_Buffer.m_Data)[idx + 3]);
		}
		else if (m_Specification.Format == TextureFormat::RGBA32)
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
		return utils::CalculateMipCount(m_Specification.Width, m_Specification.Height);
	}
	
	void OpenGLTexture2D::Bind(uint32 slot) const
	{
		Ref<const OpenGLTexture2D> instance = this;
		Renderer::Submit([instance, slot]()
		{
			glBindTextureUnit(slot, instance->RendererID);
		});
	}

	void OpenGLTexture2D::Unbind(uint32 slot) const
	{
		Renderer::Submit([slot]()
		{
			glBindTextureUnit(slot, 0);
		});
	}

	// ====================================================================================================================
	//												3D Texture
	// ====================================================================================================================

	OpenGLTexture3D::OpenGLTexture3D(const std::vector<HLString> &filePaths, bool flipOnLoad)
	{
		m_Specification.Format = TextureFormat::RGBA;

		Ref<OpenGLTexture3D> instance = this;
		Renderer::Submit([instance, filePaths, flipOnLoad]() mutable
		{
			glGenTextures(1, &instance->RendererID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, instance->RendererID);

			stbi_set_flip_vertically_on_load(flipOnLoad);

			int32 width, height, nrComponents;
			for (uint64 i = 0; i < filePaths.size(); i++)
			{
				Byte *data = stbi_load(filePaths[i].C_Str(), &width, &height, &nrComponents, 0);
				if (data)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + (uint32)i, 0, GL_RGB, (GLsizei)width, (GLsizei)height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
					instance->m_Buffer = Allocator::Copy(data, width * height * 4 * 6);
					instance->m_Loaded = true;
					stbi_image_free(data);
				}
				else
				{
					HL_CORE_ERROR("{0}[-] Failed to load one of the textures in texture3D: {1} [-]", TEXTURE3D_LOG_PREFIX, filePaths[i].C_Str());
					stbi_image_free(data);
				}
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(instance->m_Specification.Properties.SamplerFilter, false));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(instance->m_Specification.Properties.SamplerFilter, false));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));

			HL_CORE_INFO("{0}[+] Loaded 6 textures starting with {1} [+]", TEXTURE3D_LOG_PREFIX, filePaths[0].C_Str());
		});
	}

	OpenGLTexture3D::OpenGLTexture3D(TextureFormat format, uint32 width, uint32 height, const void *data)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;
		m_Specification.Format = format;
		m_Loaded = true;

		if (data)
			m_Buffer = Allocator::Copy(data, width * height * 4 * 6); // Six Layers

		uint32 levels = utils::CalculateMipCount(width, height);
		
		Ref<OpenGLTexture3D> instance = this;
		Renderer::Submit([instance, levels]() mutable
		{
			glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &instance->RendererID);
			glTextureStorage2D(instance->RendererID, levels, utils::OpenGLTextureInternalFormat(instance->m_Specification.Format), instance->m_Specification.Width, instance->m_Specification.Height);

			if (instance->m_Buffer.m_Data)
				glTextureSubImage3D(instance->RendererID, 0, 0, 0, 0, instance->m_Specification.Width, instance->m_Specification.Height, 6, utils::OpenGLTextureFormat(instance->m_Specification.Format), utils::OpenGLFormatDataType(instance->m_Specification.Format), instance->m_Buffer.m_Data);

			glTextureParameteri(instance->RendererID, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(instance->m_Specification.Properties.SamplerFilter, levels > 1));
			glTextureParameteri(instance->RendererID, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(instance->m_Specification.Properties.SamplerFilter, false));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
		});
	}

	OpenGLTexture3D::~OpenGLTexture3D()
	{
		Release();
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
			GLuint rendererID = RendererID;
			Renderer::Submit([rendererID]()
			{
				glDeleteTextures(1, &rendererID);
			});
			RendererID = 0;
			m_Loaded = false;
		}
	}

	void OpenGLTexture3D::Invalidate()
	{
		Release();

		uint32 levels = utils::CalculateMipCount(m_Specification.Width, m_Specification.Height);

		Ref<OpenGLTexture3D> instance = this;
		Renderer::Submit([instance, levels]() mutable
		{
			glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &instance->RendererID);
			glTextureStorage2D(instance->RendererID, levels, utils::OpenGLTextureInternalFormat(instance->m_Specification.Format), instance->m_Specification.Width, instance->m_Specification.Height);

			if (instance->m_Buffer.m_Data)
			{
				glTextureSubImage3D(instance->RendererID, 0, 0, 0, 0, instance->m_Specification.Width, instance->m_Specification.Height, 6, utils::OpenGLTextureFormat(instance->m_Specification.Format), utils::OpenGLFormatDataType(instance->m_Specification.Format), instance->m_Buffer.m_Data);
				instance->m_Loaded = true;
			}

			glTextureParameteri(instance->RendererID, GL_TEXTURE_MIN_FILTER, utils::OpenGLSamplerFilter(instance->m_Specification.Properties.SamplerFilter, levels > 1));
			glTextureParameteri(instance->RendererID, GL_TEXTURE_MAG_FILTER, utils::OpenGLSamplerFilter(instance->m_Specification.Properties.SamplerFilter, false));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, utils::OpenGLSamplerWrap(instance->m_Specification.Properties.SamplerWrap));
		});
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
			glTextureSubImage3D(instance->RendererID, 0, 0, 0, 0, instance->m_Specification.Width, instance->m_Specification.Height, 6, utils::OpenGLTextureFormat(instance->m_Specification.Format), utils::OpenGLFormatDataType(instance->m_Specification.Format), instance->m_Buffer.m_Data);
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
		UpdateResourceData(m_Buffer.m_Data);
	}

	uint32 OpenGLTexture3D::GetMipLevelCount()
	{
		return utils::CalculateMipCount(m_Specification.Width, m_Specification.Height);
	}

	void OpenGLTexture3D::Bind(uint32 slot) const
	{
		Ref<const OpenGLTexture3D> instance = this;
		Renderer::Submit([instance, slot]()
		{
			glBindTextureUnit(slot, instance->RendererID);
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

#endif
