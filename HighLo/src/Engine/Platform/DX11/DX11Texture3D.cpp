// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX11Texture3D.h"

#ifdef HIGHLO_API_DX11

#include <stb_image.h>
#include "Engine/Utils/ImageUtils.h"

namespace highlo
{
	DX11Texture3D::DX11Texture3D(const FileSystemPath &filepath, bool flipOnLoad)
		: m_FilePath(filepath)
	{
	}

	DX11Texture3D::DX11Texture3D(TextureFormat format, uint32 width, uint32 height, const void *data)
	{
	}

	DX11Texture3D::DX11Texture3D(const TextureSpecification &spec)
		: m_Specification(spec)
	{
	}

	DX11Texture3D::~DX11Texture3D()
	{
		Release();

		if (m_Buffer)
			m_Buffer.Release();
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

	void DX11Texture3D::Resize(const glm::uvec2 &size)
	{
		Resize(size.x, size.y);
	}

	void DX11Texture3D::Resize(const uint32 width, const uint32 height)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
	}

	void DX11Texture3D::Lock()
	{
		HL_ASSERT(!m_Locked);
		m_Locked = true;
	}

	void DX11Texture3D::Unlock()
	{
		HL_ASSERT(m_Locked);
		m_Locked = false;
		Invalidate();
	}

	void DX11Texture3D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
	{
		// TODO
	}

	glm::ivec4 DX11Texture3D::ReadPixel(uint32 row, uint32 column)
	{
		// TODO
		return glm::ivec4();
	}

	void DX11Texture3D::UpdateResourceData(void *data)
	{
		if (!data)
			return;

		// TODO
	}

	void DX11Texture3D::UpdateResourceData()
	{
		if (!m_Buffer)
			return;

		UpdateResourceData(m_Buffer.Data);
	}

	uint32 DX11Texture3D::GetMipLevelCount()
	{
		return utils::CalculateMipCount(m_Specification.Width, m_Specification.Height);
	}
	
	std::pair<uint32, uint32> DX11Texture3D::GetMipSize(uint32 mip)
	{
		return utils::GetMipSize(mip, m_Specification.Width, m_Specification.Height);
	}
	
	void DX11Texture3D::GenerateMips(bool readonly)
	{
	}
}

#endif // HIGHLO_API_DX11

