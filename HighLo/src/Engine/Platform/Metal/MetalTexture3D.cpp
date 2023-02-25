// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MetalTexture3D.h"

#ifdef HIGHLO_API_METAL

namespace highlo
{
	MetalTexture3D::MetalTexture3D(const FileSystemPath &filePath, bool flipOnLoad)
	{
	}

	MetalTexture3D::MetalTexture3D(TextureFormat format, uint32 width, uint32 height, const void *data)
	{
	}
	
	MetalTexture3D::~MetalTexture3D()
	{
		Release();
	}
	
	Allocator MetalTexture3D::GetData()
	{
		HL_ASSERT(m_Locked);
		return m_Buffer;
	}
	
	void MetalTexture3D::Release()
	{
	}

	void MetalTexture3D::Invalidate()
	{
	}
	
	void MetalTexture3D::Resize(const glm::uvec2 &size)
	{
	}
	
	void MetalTexture3D::Resize(const uint32 width, const uint32 height)
	{
	}
	
	void MetalTexture3D::Lock()
	{
	}
	
	void MetalTexture3D::Unlock()
	{
	}
	
	void MetalTexture3D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
	{
	}
	
	glm::ivec4 MetalTexture3D::ReadPixel(uint32 row, uint32 column)
	{
		return glm::ivec4();
	}
	
	void MetalTexture3D::UpdateResourceData(void *data)
	{
	}
	
	void MetalTexture3D::UpdateResourceData()
	{
	}
	
	uint32 MetalTexture3D::GetMipLevelCount()
	{
		return 0;
	}
	
	std::pair<uint32, uint32> MetalTexture3D::GetMipSize(uint32 mip)
	{
		return { 0, 0 };
	}
	
	void MetalTexture3D::GenerateMips(bool readonly)
	{
	}
	
	void MetalTexture3D::Bind(uint32 slot) const
	{
	}
	
	void MetalTexture3D::Unbind(uint32 slot) const
	{
	}
}

#endif // HIGHLO_API_METAL

