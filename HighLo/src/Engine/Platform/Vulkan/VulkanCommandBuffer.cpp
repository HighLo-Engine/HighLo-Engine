// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanCommandBuffer.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	VulkanCommandBuffer::VulkanCommandBuffer(const HLString &debugName, uint32 count)
	{
	}
	
	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
	}
	
	void VulkanCommandBuffer::Begin()
	{
	}
	
	void VulkanCommandBuffer::End()
	{
	}
	
	void VulkanCommandBuffer::Submit()
	{
	}
	
	float VulkanCommandBuffer::GetCPUExecutionTime(uint32 frameIndex, uint32 queryIndex) const
	{
		return 0.0f;
	}
	
	uint64 VulkanCommandBuffer::BeginTimestampQuery()
	{
		return 0;
	}
	
	void VulkanCommandBuffer::EndTimestampQuery(uint64 queryID)
	{
	}
}

#endif // HIGHLO_API_VULKAN

