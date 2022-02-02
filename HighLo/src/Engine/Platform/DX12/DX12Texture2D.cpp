// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX12Texture2D.h"

#ifdef HIGHLO_API_DX12

#include <stb_image.h>
#include "Engine/Utils/ImageUtils.h"

namespace highlo
{
	DX12Texture2D::DX12Texture2D(const FileSystemPath &filePath, TextureFormat format, bool flipOnLoad)
	{
	}
	
	DX12Texture2D::DX12Texture2D(const glm::vec3 &rgb, TextureFormat format)
	{
	}
	
	DX12Texture2D::DX12Texture2D(const glm::vec3 &rgb, uint32 width, uint32 height, TextureFormat format)
	{
	}
	
	DX12Texture2D::DX12Texture2D(void *imgData, uint32 width, uint32 height, TextureFormat format)
	{
	}
	
	DX12Texture2D::DX12Texture2D(TextureFormat format, uint32 width, uint32 height, const void *data, TextureProperties props)
	{
	}
	
	DX12Texture2D::DX12Texture2D(TextureFormat format, uint32 width, uint32 height)
	{
	}
	
	DX12Texture2D::DX12Texture2D(const TextureSpecification &spec)
	{
	}
	
	DX12Texture2D::~DX12Texture2D()
	{
	}
	
	Allocator DX12Texture2D::GetData()
	{
		return Allocator();
	}
	
	void DX12Texture2D::Bind(uint32 slot) const
	{
	}
	
	void DX12Texture2D::Unbind(uint32 slot) const
	{
	}
	
	void DX12Texture2D::Release()
	{
	}
	
	void DX12Texture2D::Invalidate()
	{
	}
	
	void DX12Texture2D::Lock()
	{
	}
	
	void DX12Texture2D::Unlock()
	{
	}
	
	void DX12Texture2D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
	{
	}
	
	glm::ivec4 DX12Texture2D::ReadPixel(uint32 row, uint32 column)
	{
		return glm::ivec4();
	}
	
	void DX12Texture2D::UpdateResourceData(void *data)
	{
	}
	
	void DX12Texture2D::UpdateResourceData()
	{
	}
	
	uint32 DX12Texture2D::GetMipLevelCount()
	{
		return utils::CalculateMipCount(m_Specification.Width, m_Specification.Height);
	}
	
	void DX12Texture2D::CreatePerLayerImageViews()
	{
	}
	
	void DX12Texture2D::CreateSampler(TextureProperties properties)
	{
	}
}

#endif // HIGHLO_API_DX12

