// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanRenderPass.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	VulkanRenderPass::VulkanRenderPass(const RenderPassSpecification &spec)
		: m_Specification(spec)
	{
	}

	VulkanRenderPass::~VulkanRenderPass()
	{
	}
}

#endif // HIGHLO_API_VULKAN

