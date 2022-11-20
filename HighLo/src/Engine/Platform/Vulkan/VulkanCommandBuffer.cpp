// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanCommandBuffer.h"

namespace highlo
{
	VulkanCommandBuffer::VulkanCommandBuffer(uint32 count, const HLString &debugName)
		: m_DebugName(debugName), m_Count(count)
	{
	}

	VulkanCommandBuffer::VulkanCommandBuffer(const HLString &debugName, bool swapChain)
		: m_DebugName(debugName), m_SwapChain(swapChain)
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

	float VulkanCommandBuffer::GetExecutionGPUTime(uint32 frameIndex, uint32 queryIndex) const
	{
		return 0.0f;
	}

	uint32 VulkanCommandBuffer::BeginTimestampQuery()
	{
		return 0;
	}

	void VulkanCommandBuffer::EndTimestampQuery(uint32 queryID)
	{
	}

	const PipelineStatistics &VulkanCommandBuffer::GetPipelineStatistics(uint32 frameIndex) const
	{
		return m_PipelineStatisticsQueryResults[frameIndex];
	}
}

