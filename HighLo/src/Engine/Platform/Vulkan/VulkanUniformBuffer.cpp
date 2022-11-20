// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanUniformBuffer.h"

namespace highlo
{
	VulkanUniformBuffer::VulkanUniformBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout)
		: UniformBuffer(binding, layout)
	{
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
	}

	void VulkanUniformBuffer::UploadToShader()
	{
	}
}

