// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanFrameBuffer.h"

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
}

