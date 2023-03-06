// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MetalTexture2D.h"

#ifdef HIGHLO_API_METAL

#include "Engine/Utils/ImageUtils.h"

namespace highlo
{
	MetalTexture2D::MetalTexture2D(const FileSystemPath &filePath, bool flipOnLoad)
		: m_FilePath(filePath)
	{
	}

	MetalTexture2D::MetalTexture2D(const glm::vec3 &rgb, TextureFormat format)
	{
	}
	
	MetalTexture2D::MetalTexture2D(const glm::vec3 &rgb, uint32 width, uint32 height, TextureFormat format)
	{
	}
	
	MetalTexture2D::MetalTexture2D(void *imgData, uint32 width, uint32 height, TextureFormat format)
	{
	}
	
	MetalTexture2D::MetalTexture2D(TextureFormat format, uint32 width, uint32 height, const void *data, TextureProperties props)
	{
	}
	
	MetalTexture2D::MetalTexture2D(TextureFormat format, uint32 width, uint32 height)
	{
	}
	
	MetalTexture2D::MetalTexture2D(const TextureSpecification &spec)
		: m_Specification(spec)
	{
	}
	
	MetalTexture2D::~MetalTexture2D()
	{
		Release();

		if (m_Buffer)
			m_Buffer.Release();
	}
	
	Allocator MetalTexture2D::GetData()
	{
		HL_ASSERT(m_Locked);
		return m_Buffer;
	}
	
	void MetalTexture2D::Release()
	{
	}
	
	void MetalTexture2D::Invalidate()
	{
	}
	
	void MetalTexture2D::Resize(const glm::uvec2 &size)
	{
		Resize(size.x, size.y);
	}
	
	void MetalTexture2D::Resize(const uint32 width, const uint32 height)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
	}
	
	void MetalTexture2D::Lock()
	{
		HL_ASSERT(!m_Locked);
		m_Locked = true;
	}
	
	void MetalTexture2D::Unlock()
	{
		HL_ASSERT(m_Locked);
		m_Locked = false;
		Invalidate();
	}
	
	void MetalTexture2D::CreatePerLayerImageViews()
	{
	}
	
	void MetalTexture2D::CreateSampler(TextureProperties properties)
	{
	}
	
	void MetalTexture2D::CreatePerSpecificLayerImageViews(const std::vector<uint32> &layerIndices)
	{
	}
	
	void MetalTexture2D::UpdateResourceData()
	{
		if (!m_Buffer)
			return;

		UpdateResourceData(m_Buffer.Data);
	}
	
	void MetalTexture2D::UpdateResourceData(void *data)
	{
		if (!data)
			return;

		// TODO
	}
	
	void MetalTexture2D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
	{
	}
	
	glm::ivec4 MetalTexture2D::ReadPixel(uint32 row, uint32 column)
	{
		return glm::ivec4();
	}
	
	uint32 MetalTexture2D::GetMipLevelCount()
	{
		return utils::CalculateMipCount(m_Specification.Width, m_Specification.Height);
	}
	
	std::pair<uint32, uint32> MetalTexture2D::GetMipSize(uint32 mip)
	{
		return utils::GetMipSize(mip, m_Specification.Width, m_Specification.Height);
	}
	
	void MetalTexture2D::GenerateMips(bool readonly)
	{
	}
	
	void MetalTexture2D::SetData(void *data, uint32 data_size)
	{
		if (m_Buffer)
			m_Buffer.Release();

		m_Buffer = Allocator::Copy(data, data_size);
		Invalidate();
	}
	
	void MetalTexture2D::Bind(uint32 slot) const
	{
	}
	
	void MetalTexture2D::Unbind(uint32 slot) const
	{
	}
}

#endif // HIGHLO_API_METAL

