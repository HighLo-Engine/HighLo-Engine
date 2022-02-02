// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX11Texture2D.h"

#ifdef HIGHLO_API_DX11

#include <stb_image.h>
#include "Engine/Utils/ImageUtils.h"

namespace highlo
{
	DX11Texture2D::DX11Texture2D(const FileSystemPath &filePath, TextureFormat format, bool flipOnLoad)
	{
	}

	DX11Texture2D::DX11Texture2D(const glm::vec3 &rgb, TextureFormat format)
	{
	}
	
	DX11Texture2D::DX11Texture2D(const glm::vec3 &rgb, uint32 width, uint32 height, TextureFormat format)
	{
	}
	
	DX11Texture2D::DX11Texture2D(TextureFormat format, uint32 width, uint32 height)
	{
	}
	
	DX11Texture2D::DX11Texture2D(TextureFormat format, uint32 width, uint32 height, const void *data, TextureProperties props)
	{
	}

	DX11Texture2D::DX11Texture2D(const TextureSpecification &specification)
	{
	}
	
	DX11Texture2D::~DX11Texture2D()
	{
	}
	
	Allocator DX11Texture2D::GetData()
	{
		HL_ASSERT(m_Locked, "Texture must be locked!");
		return m_Buffer;
	}
	
	void DX11Texture2D::Bind(uint32 slot) const
	{
	}
	
	void DX11Texture2D::Unbind(uint32 slot) const
	{
	}
	
	void DX11Texture2D::Release()
	{
	}
	
	void DX11Texture2D::Invalidate()
	{
	}
	
	void DX11Texture2D::Lock()
	{
	}
	
	void DX11Texture2D::Unlock()
	{
	}
	
	void DX11Texture2D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
	{
	}
	
	glm::ivec4 DX11Texture2D::ReadPixel(uint32 row, uint32 column)
	{
		return glm::ivec4();
	}
	
	void DX11Texture2D::UpdateResourceData(void *data)
	{
	}
	
	void DX11Texture2D::UpdateResourceData()
	{
	}
	
	uint32 DX11Texture2D::GetMipLevelCount()
	{
		return utils::CalculateMipCount(m_Specification.Width, m_Specification.Height);
	}
	
	void DX11Texture2D::CreatePerLayerImageViews()
	{
	}
	
	void DX11Texture2D::CreateSampler(TextureProperties properties)
	{
	}
}

#endif
