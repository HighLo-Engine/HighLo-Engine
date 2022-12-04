// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanComputePipeline.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	VulkanComputePipeline::VulkanComputePipeline(const Ref<Shader> &computeShader)
		: m_Shader(computeShader)
	{
	}

	VulkanComputePipeline::~VulkanComputePipeline()
	{
	}

	void VulkanComputePipeline::Begin(const Ref<CommandBuffer> &renderCommandBuffer)
	{
	}

	void VulkanComputePipeline::End()
	{
	}

	void VulkanComputePipeline::Invalidate()
	{
	}
}

#endif // HIGHLO_API_VULKAN

