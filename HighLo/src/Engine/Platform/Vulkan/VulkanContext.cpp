// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanContext.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	VulkanContext::VulkanContext(void *handle, WindowData &data)
	{
	}
	
	VulkanContext::~VulkanContext()
	{
	}
	
	void VulkanContext::Init()
	{
	}
	
	void VulkanContext::SwapBuffers()
	{
	}
	
	void VulkanContext::MakeCurrent()
	{
	}
	
	void VulkanContext::SetSwapInterval(bool bEnabled)
	{
	}
	
	void *VulkanContext::GetCurrentContext()
	{
		return nullptr;
	}
}

#endif // HIGHLO_API_VULKAN

