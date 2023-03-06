// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MetalTexture3D.h"

#ifdef HIGHLO_API_METAL

#include "Engine/Utils/ImageUtils.h"

namespace highlo
{
	MetalTexture3D::MetalTexture3D(const FileSystemPath &filePath, bool flipOnLoad)
		: m_FilePath(filePath)
	{
	}

	MetalTexture3D::MetalTexture3D(TextureFormat format, uint32 width, uint32 height, const void *data)
	{
	}

	MetalTexture3D::MetalTexture3D(const TextureSpecification &spec)
		: m_Specification(spec)
	{
	}
	
	MetalTexture3D::~MetalTexture3D()
	{
		Release();

		if (m_Buffer)
			m_Buffer.Release();
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
		Resize(size.x, size.y);
	}
	
	void MetalTexture3D::Resize(const uint32 width, const uint32 height)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
	}
	
	void MetalTexture3D::Lock()
	{
		HL_ASSERT(!m_Locked);
		m_Locked = true;
	}
	
	void MetalTexture3D::Unlock()
	{
		HL_ASSERT(m_Locked);
		m_Locked = false;
		Invalidate();
	}
	
	void MetalTexture3D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
	{
		// TODO
	}
	
	glm::ivec4 MetalTexture3D::ReadPixel(uint32 row, uint32 column)
	{
		// TODO
		return glm::ivec4();
	}
	
	void MetalTexture3D::UpdateResourceData(void *data)
	{
		if (!data)
			return;

		// TODO
	}
	
	void MetalTexture3D::UpdateResourceData()
	{
		if (!m_Buffer)
			return;

		UpdateResourceData(m_Buffer.Data);
	}
	
	uint32 MetalTexture3D::GetMipLevelCount()
	{
		return utils::CalculateMipCount(m_Specification.Width, m_Specification.Height);
	}
	
	std::pair<uint32, uint32> MetalTexture3D::GetMipSize(uint32 mip)
	{
		return utils::GetMipSize(mip, m_Specification.Width, m_Specification.Height);
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

