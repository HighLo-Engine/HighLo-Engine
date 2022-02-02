// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX12Texture3D.h"

#ifdef HIGHLO_API_DX12

#include <stb_image.h>
#include "Engine/Utils/ImageUtils.h"

namespace highlo
{
	DX12Texture3D::DX12Texture3D(const FileSystemPath &filePath, bool flipOnLoad)
	{
	}
	
	DX12Texture3D::DX12Texture3D(TextureFormat format, uint32 width, uint32 height, const void *data)
	{
	}
	
	DX12Texture3D::~DX12Texture3D()
	{
	}
	
	Allocator DX12Texture3D::GetData()
	{
		return Allocator();
	}
	
	void DX12Texture3D::Bind(uint32 slot) const
	{
	}
	
	void DX12Texture3D::Unbind(uint32 slot) const
	{
	}
	
	void DX12Texture3D::Release()
	{
	}
	
	void DX12Texture3D::Invalidate()
	{
	}
	
	void DX12Texture3D::Lock()
	{
	}
	
	void DX12Texture3D::Unlock()
	{
	}
	
	void DX12Texture3D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
	{
	}
	
	glm::ivec4 DX12Texture3D::ReadPixel(uint32 row, uint32 column)
	{
		return glm::ivec4();
	}
	
	void DX12Texture3D::UpdateResourceData(void *data)
	{
	}
	
	void DX12Texture3D::UpdateResourceData()
	{
	}
	
	uint32 DX12Texture3D::GetMipLevelCount()
	{
		return utils::CalculateMipCount(m_Specification.Width, m_Specification.Height);
	}
}

#endif // HIGHLO_API_DX12

