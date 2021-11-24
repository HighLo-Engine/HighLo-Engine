// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanFramebuffer.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	VulkanFramebuffer::VulkanFramebuffer(const FramebufferSpecification &spec)
	{
	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
	}
	
	void VulkanFramebuffer::Bind() const
	{
	}
	
	void VulkanFramebuffer::Unbind() const
	{
	}
	
	void VulkanFramebuffer::Resize(uint32 width, uint32 height, bool forceRecreate)
	{
	}
	
	void VulkanFramebuffer::AddResizeCallback(const std::function<void(Ref<Framebuffer>)> &func)
	{
	}
	
	void VulkanFramebuffer::BindTexture(uint32 attachmentIndex, uint32 slot) const
	{
	}
	
	void VulkanFramebuffer::ClearAttachment(uint32 attachmentIndex, int32 value)
	{
	}
	
	int32 VulkanFramebuffer::ReadPixel(uint32 attachmentIndex, int32 x, int32 y)
	{
		return 0;
	}
	
	Ref<Texture> VulkanFramebuffer::GetImage(uint32 attachmentIndex) const
	{
		HL_ASSERT(attachmentIndex < m_AttachmentTextures.size());
		return m_AttachmentTextures[attachmentIndex];
	}
	
	Ref<Texture> VulkanFramebuffer::GetDepthImage() const
	{
		return m_DepthTexture;
	}
}

#endif // HIGHLO_API_VULKAN

