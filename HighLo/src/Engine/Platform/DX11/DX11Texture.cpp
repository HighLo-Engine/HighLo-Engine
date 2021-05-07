#include "HighLoPch.h"
#include "DX11Texture.h"

#ifdef HIGHLO_API_DX11

#include "stb_image.h"
#include "Engine/Core/HLLog.h"
#include "Engine/Utils/ImageUtils.h"

namespace highlo
{
	DX11Texture2D* DX11Texture2D::LoadFromFile(const HLString& filepath, TextureFormat format, bool flip_on_load)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(flip_on_load);

		stbi_uc* data = stbi_load(filepath.C_Str(), &width, &height, &channels, STBI_rgb_alpha);
		if (!data)
		{
			stbi_image_free(data);
			HL_CORE_ERROR("Texture2D> [-] Failed to load texture2D: " + filepath + " [-]");
			return nullptr;
		}

		DX11Texture2D* instance = new DX11Texture2D(data, width, height, format);
		instance->m_ImageData = data;

		HL_CORE_INFO("Texture2D>    [+] Loaded " + filepath + " [+]");

		return instance;
	}

	DX11Texture2D* DX11Texture2D::CreateFromColor(const glm::vec3& rgb, TextureFormat format)
	{
		DX11Texture2D* instance;

		if (format == TextureFormat::RGBA8)
		{
			unsigned char data[4] = { (unsigned char)rgb.x, (unsigned char)rgb.y, (unsigned char)rgb.z, (unsigned char)255 };
			instance = new DX11Texture2D(data, 1, 1, format);

			instance->m_ImageData = malloc(4);
			memcpy_s(instance->m_ImageData, 4, data, 4);
		}
		else
		{
			uint16 data[4] = { (uint16)rgb.x, (uint16)rgb.y, (uint16)rgb.z, (uint16)65535 };
			instance = new DX11Texture2D(data, 1, 1, format);

			instance->m_ImageData = malloc(8);
			memcpy_s(instance->m_ImageData, 8, data, 8);
		}

		return instance;
	}

	DX11Texture2D* DX11Texture2D::CreateFromColor(const glm::vec3& rgb, uint32 width, uint32 height, TextureFormat format)
	{
		DX11Texture2D* instance;

#pragma warning( push )
#pragma warning( disable : 6386)
		if (format == TextureFormat::RGBA8)
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

			instance = new DX11Texture2D(data, width, height, format);

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

			instance = new DX11Texture2D(data, width, height, format);

			instance->m_ImageData = new uint16[(uint64)width * (uint64)height * (uint64)4];
			memcpy_s(instance->m_ImageData, ((uint64)width * (uint64)height * (uint64)8), data, ((uint64)width * (uint64)height * (uint64)8));

			delete[] data;
		}
#pragma warning( pop )

		return instance;
	}

	DX11Texture2D::DX11Texture2D(void* img_data, uint32 width, uint32 height, TextureFormat format)
	{
		m_Width = width;
		m_Height = height;
		Format = format;

		DXGI_FORMAT dxgi_format;
		uint32 byte_stride = (format == TextureFormat::RGBA16) ? 8 : 4;

		switch (format)
		{
		case TextureFormat::RGBA8:
		{
			dxgi_format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		}
		case TextureFormat::RGBA16:
		{
			dxgi_format = DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_UNORM;
			break;
		}
		default:
		{
			dxgi_format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		}
		}

		D3D11_SUBRESOURCE_DATA resource_data;
		ZeroMemory(&resource_data, sizeof(D3D11_SUBRESOURCE_DATA));
		resource_data.pSysMem = img_data;
		resource_data.SysMemPitch = byte_stride * width;
		resource_data.SysMemSlicePitch = 0;

		D3D11_TEXTURE2D_DESC texture_desc;
		ZeroMemory(&texture_desc, sizeof(D3D11_TEXTURE2D_DESC));
		texture_desc.Width = m_Width;
		texture_desc.Height = m_Height;
		texture_desc.ArraySize = 1;
		texture_desc.MipLevels = 1;
		texture_desc.Format = dxgi_format;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage = D3D11_USAGE_DYNAMIC;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texture_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HRESULT result = DX11Resources::s_Device->CreateTexture2D(&texture_desc, &resource_data, m_TextureBuffer.GetAddressOf());
		if (FAILED(result))
		{
			HL_CORE_ERROR("Texture2D> Failed to create ID3D11Texture2D | HRESULT: " + std::to_string(result));
			return;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		SRVDesc.Format = dxgi_format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = 1;
		SRVDesc.Texture2D.MostDetailedMip = 0;

		result = DX11Resources::s_Device->CreateShaderResourceView(m_TextureBuffer.Get(),
			&SRVDesc, m_ShaderResource.GetAddressOf());

		if (FAILED(result))
			HL_CORE_ERROR("Texture2D> Failed to create shader resource view");
	}

	DX11Texture2D::~DX11Texture2D()
	{
		stbi_image_free(m_ImageData);
	}

	void* DX11Texture2D::GetData() const
	{
		return m_ImageData;
	}

	void DX11Texture2D::Bind(uint32 slot) const
	{
		if (ShaderLocation == TextureShaderLocation::PIXEL_SHADER) DX11Resources::s_DeviceContext->PSSetShaderResources(slot, 1, m_ShaderResource.GetAddressOf());
		if (ShaderLocation == TextureShaderLocation::DOMAIN_SHADER) DX11Resources::s_DeviceContext->DSSetShaderResources(slot, 1, m_ShaderResource.GetAddressOf());
	}

	DX11Texture2D::DX11Texture2D(uint32 width, uint32 height, const ComPtr<ID3D11ShaderResourceView>& SRV, TextureFormat format)
	{
		m_Width = width;
		m_Height = height;
		m_ShaderResource = SRV;
		Format = format;
	}

	void DX11Texture2D::UpdateResourceData()
	{
		UpdateResourceData(m_ImageData);
	}

	void DX11Texture2D::UpdateResourceData(void* data)
	{
		uint64 byte_offset = (Format == TextureFormat::RGBA16) ? 8 : 4;

		D3D11_MAPPED_SUBRESOURCE resource;
		DX11Resources::s_DeviceContext->Map(m_TextureBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, data, (uint64)m_Width * (uint64)m_Height * byte_offset);
		DX11Resources::s_DeviceContext->Unmap(m_TextureBuffer.Get(), 0);
	}

	void DX11Texture2D::WritePixel(uint32 row, uint32 column, const glm::ivec4& rgba)
	{
		uint64 idx = (((uint64)row * (uint64)m_Width) + (uint64)column) * 4;

		if (idx >= (uint64)m_Width * (uint64)m_Height * 4 || idx < 0) return;

		if (Format == TextureFormat::RGBA8)
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

	glm::ivec4 DX11Texture2D::ReadPixel(uint32 row, uint32 column)
	{
		uint64 idx = (((uint64)row * (uint64)m_Width) + (uint64)column) * 4;

		if (idx >= (uint64)m_Width * (uint64)m_Height * 4 || idx < 0) return { 0, 0, 0, 0 };

		glm::ivec4 rgba;

		if (Format == TextureFormat::RGBA8)
		{
			rgba.r = (uint32)(((unsigned char*)m_ImageData)[idx]);
			rgba.g = (uint32)(((unsigned char*)m_ImageData)[idx + 1]);
			rgba.b = (uint32)(((unsigned char*)m_ImageData)[idx + 2]);
			rgba.a = (uint32)(((unsigned char*)m_ImageData)[idx + 3]);
		}
		else
		{
			rgba.r = (uint32)(((uint16*)m_ImageData)[idx]);
			rgba.g = (uint32)(((uint16*)m_ImageData)[idx + 1]);
			rgba.b = (uint32)(((uint16*)m_ImageData)[idx + 2]);
			rgba.a = (uint32)(((uint16*)m_ImageData)[idx + 3]);
		}

		return rgba;
	}

	uint32 DX11Texture2D::GetMipLevelCount()
	{
		return utils::CalculateMipCount(m_Width, m_Height);
	}

	HLRendererID DX11Texture2D::GetRendererID()
	{
		// TODO
		return 0;
	}
}

#endif
