// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MetalFramebuffer.h"

#ifdef HIGHLO_API_METAL

namespace highlo
{
	MetalFramebuffer::MetalFramebuffer(const FramebufferSpecification &spec)
		: m_Specification(spec)
	{
	}
	
	MetalFramebuffer::~MetalFramebuffer()
	{
		Release();
	}
	
	void MetalFramebuffer::Invalidate()
	{
	}
	
	void MetalFramebuffer::Release()
	{
	}

	void MetalFramebuffer::Resize(uint32 width, uint32 height, bool forceRecreate)
	{
	}
	
	void MetalFramebuffer::AddResizeCallback(const std::function<void(Ref<Framebuffer>)> &func)
	{
		m_ResizeCallbacks.push_back(func);
	}
	
	void MetalFramebuffer::Bind() const
	{
	}
	
	void MetalFramebuffer::Unbind() const
	{
	}
	
	void MetalFramebuffer::BindTexture(uint32 attachmentIndex, uint32 slot) const
	{
	}
	
	void MetalFramebuffer::ClearAttachment(uint32 attachmentIndex, int32 value)
	{
	}
	
	int32 MetalFramebuffer::ReadPixel(uint32 attachmentIndex, int32 x, int32 y)
	{
		return -1;
	}
}

#endif // HIGHLO_API_METAL

