// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanFramebuffer.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	VulkanFramebuffer::VulkanFramebuffer(const FramebufferSpecification &spec)
		: m_Specification(spec)
	{

	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
	}
	
	void VulkanFramebuffer::Invalidate()
	{
	}
	
	void VulkanFramebuffer::Release()
	{
	}
	
	void VulkanFramebuffer::Resize(uint32 width, uint32 height, bool forceRecreate)
	{
	}
	
	void VulkanFramebuffer::AddResizeCallback(const std::function<void(Ref<Framebuffer>)> &func)
	{
	}
	
	void VulkanFramebuffer::ClearAttachment(uint32 attachmentIndex, int32 value)
	{
	}
	
	int32 VulkanFramebuffer::ReadPixel(uint32 attachmentIndex, int32 x, int32 y)
	{
		// TODO
		return 0;
	}
}

#endif // HIGHLO_API_VULKAN

