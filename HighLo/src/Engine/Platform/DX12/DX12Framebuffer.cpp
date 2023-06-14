// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX12Framebuffer.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	DX12Framebuffer::DX12Framebuffer(const FramebufferSpecification &spec)
		: m_Specification(spec)
	{
	}

	DX12Framebuffer::~DX12Framebuffer()
	{
		Release();
	}

	void DX12Framebuffer::Resize(uint32 width, uint32 height, bool forceRecreate)
	{


		for (auto &callback : m_ResizeCallbacks)
			callback(this);
	}

	void DX12Framebuffer::AddResizeCallback(const std::function<void(Ref<Framebuffer>)> &func)
	{
		m_ResizeCallbacks.push_back(func);
	}

	void DX12Framebuffer::Invalidate()
	{
	}

	void DX12Framebuffer::Release()
	{
	}
	
	void DX12Framebuffer::Bind() const
	{
	}
	
	void DX12Framebuffer::Unbind() const
	{
	}
	
	void DX12Framebuffer::BindTexture(uint32 attachmentIndex, uint32 slot) const
	{
	}
	
	void DX12Framebuffer::ClearAttachment(uint32 attachmentIndex, int32 value)
	{
	}
	
	int32 DX12Framebuffer::ReadPixel(uint32 attachmentIndex, int32 x, int32 y)
	{
		return 0;
	}
}

#endif // HIGHLO_API_DX12

