// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

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

	DX12Texture3D::DX12Texture3D(const TextureSpecification &spec)
		: m_Specification(spec)
	{
	}
	
	DX12Texture3D::~DX12Texture3D()
	{
		Release();

		if (m_Buffer)
			m_Buffer.Release();
	}
	
	Allocator DX12Texture3D::GetData()
	{
		HL_ASSERT(m_Locked);
		return m_Buffer;
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

	void DX12Texture3D::Resize(const glm::uvec2 &size)
	{
		Resize(size.x, size.y);
	}

	void DX12Texture3D::Resize(const uint32 width, const uint32 height)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
	}
	
	void DX12Texture3D::Lock()
	{
		HL_ASSERT(!m_Locked);
		m_Locked = true;
	}
	
	void DX12Texture3D::Unlock()
	{
		HL_ASSERT(m_Locked);
		m_Locked = false;
		Invalidate();
	}
	
	void DX12Texture3D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
	{
		// TODO
	}
	
	glm::ivec4 DX12Texture3D::ReadPixel(uint32 row, uint32 column)
	{
		// TODO
		return glm::ivec4();
	}
	
	void DX12Texture3D::UpdateResourceData(void *data)
	{
		if (!data)
			return;

		// TODO
	}
	
	void DX12Texture3D::UpdateResourceData()
	{
		if (!m_Buffer)
			return;

		UpdateResourceData(m_Buffer.Data);
	}
	
	uint32 DX12Texture3D::GetMipLevelCount()
	{
		return utils::CalculateMipCount(m_Specification.Width, m_Specification.Height);
	}

	std::pair<uint32, uint32> DX12Texture3D::GetMipSize(uint32 mip)
	{
		return utils::GetMipSize(mip, m_Specification.Width, m_Specification.Height);
	}

	void DX12Texture3D::GenerateMips(bool readonly)
	{
	}
}

#endif // HIGHLO_API_DX12

