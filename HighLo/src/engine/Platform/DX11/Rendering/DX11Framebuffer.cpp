#include "HighLoPch.h"
#include "DX11Framebuffer.h"

#ifdef HIGHLO_API_DX11

namespace highlo
{
	DX11Framebuffer::DX11Framebuffer(const FramebufferSpecification &spec)
		: m_Specification(spec), m_Width(spec.Width), m_Height(spec.Height)
	{
	}

	DX11Framebuffer::~DX11Framebuffer()
	{
	}

	void DX11Framebuffer::Resize(uint32 width, uint32 height, bool forceRecreate)
	{
	}
	
	void DX11Framebuffer::Bind() const
	{
	}

	void DX11Framebuffer::Unbind() const
	{
	}

	void DX11Framebuffer::BindTexture(uint32 attachmentIndex, uint32 slot) const
	{
	}

	void DX11Framebuffer::ClearAttachment(uint32 attachmentIndex, int32 value)
	{
	}

	int32 DX11Framebuffer::ReadPixel(uint32 attachmentIndex, int32 x, int32 y)
	{
		return 0;
	}


}

#endif // HIGHLO_API_DX11
