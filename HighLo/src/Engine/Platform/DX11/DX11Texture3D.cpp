// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX11Texture3D.h"

#ifdef HIGHLO_API_DX11

#include <stb_image.h>
#include "Engine/Utils/ImageUtils.h"

namespace highlo
{
	DX11Texture3D::DX11Texture3D(TextureFormat format, uint32 width, uint32 height, const void *data)
	{
	}

	DX11Texture3D::DX11Texture3D(const FileSystemPath &filepath, bool flipOnLoad)
	{
	}

	DX11Texture3D::~DX11Texture3D()
	{
	}

	Allocator DX11Texture3D::GetData()
	{
		HL_ASSERT(m_Locked, "Texture must be locked!");
		return m_Buffer;
	}

	void DX11Texture3D::Bind(uint32 slot) const
	{
	}

	void DX11Texture3D::Unbind(uint32 slot) const
	{
	}

	void DX11Texture3D::Release()
	{
	}

	void DX11Texture3D::Invalidate()
	{
	}

	void DX11Texture3D::Lock()
	{
	}

	void DX11Texture3D::Unlock()
	{
	}

	void DX11Texture3D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
	{
	}

	glm::ivec4 DX11Texture3D::ReadPixel(uint32 row, uint32 column)
	{
		return glm::ivec4();
	}

	void DX11Texture3D::UpdateResourceData(void *data)
	{
	}

	void DX11Texture3D::UpdateResourceData()
	{
	}

	uint32 DX11Texture3D::GetMipLevelCount()
	{
		return utils::CalculateMipCount(m_Specification.Width, m_Specification.Height);
	}
	
	std::pair<uint32, uint32> DX11Texture3D::GetMipSize(uint32 mip)
	{
		return utils::GetMipSize(mip, m_Specification.Width, m_Specification.Height);
	}
	
	void DX11Texture3D::GenerateMips()
	{
	}
}

#endif // HIGHLO_API_DX11

