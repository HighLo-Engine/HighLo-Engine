// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanUniformBuffer.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	VulkanUniformBuffer::VulkanUniformBuffer(const HLString &name, const std::vector<UniformVariable> &layout, UniformBufferParentShader parent, uint32 slot)
		: UniformBuffer(name, layout, parent, slot)
	{
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
	}

	void VulkanUniformBuffer::UploadToShader(uint32 offset)
	{
	}
}

#endif // HIGHLO_API_VULKAN

