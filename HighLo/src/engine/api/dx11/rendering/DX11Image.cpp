#include "HighLoPch.h"
#include "DX11Image.h"

#ifdef HIGHLO_API_DX11

#include "engine/utils/ImageUtils.h"

namespace highlo
{
	DX11Image2D::DX11Image2D(ImageFormat format, uint32 width, uint32 height, Allocator buffer)
		: m_Format(format), m_Width(width), m_Height(height), m_ImageData(buffer) {}

	DX11Image2D::DX11Image2D(ImageFormat format, uint32 width, uint32 height, const void *data)
		: m_Format(format), m_Width(width), m_Height(height)
	{
		if (data)
			m_ImageData = Allocator::Copy(data, utils::GetImageMemorySize(format, width, height));
	}

	DX11Image2D::~DX11Image2D()
	{
		m_ImageData.Release();
		if (m_RendererID)
		{
			// Free Textures
		}
	}

	void DX11Image2D::Invalidate()
	{
		if (m_RendererID)
			Release();

		// Create Textures
	}

	void DX11Image2D::Release()
	{
		if (m_RendererID)
		{
			// Free Textures
			m_RendererID = 0;
		}

		m_ImageData.Release();
	}
}

#endif // HIGHLO_API_DX11