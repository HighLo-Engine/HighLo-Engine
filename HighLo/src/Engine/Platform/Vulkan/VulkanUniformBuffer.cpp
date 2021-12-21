// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanUniformBuffer.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	VulkanUniformBuffer::VulkanUniformBuffer(uint32 size, uint32 binding)
		: m_Size(size), m_Binding(binding)
	{
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
	}
	
	void VulkanUniformBuffer::SetData(const void *data, uint32 size, uint32 offset)
	{
	}
}

#endif // HIGHLO_API_VULKAN

