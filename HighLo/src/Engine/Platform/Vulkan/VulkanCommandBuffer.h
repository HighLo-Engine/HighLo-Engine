// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-23) initial release
//

#pragma once

#include "Vulkan.h"
#include "Engine/Renderer/CommandBuffer.h"
#include "Engine/Renderer/PipelineStatistics.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	class VulkanCommandBuffer : public CommandBuffer
	{
	public:

		VulkanCommandBuffer(const HLString &debugName = "", uint32 count = 0);
		virtual ~VulkanCommandBuffer();

		virtual void Begin() override;
		virtual void End() override;
		virtual void Submit() override;

		virtual float GetCPUExecutionTime(uint32 frameIndex, uint32 queryIndex = 0) const override;

		virtual uint64 BeginTimestampQuery() override;
		virtual void EndTimestampQuery(uint64 queryID) override;

	private:

		HLString m_DebugName;
		VkCommandPool m_CommandPool = nullptr;

		std::vector<VkCommandBuffer> m_CommandBuffers;
		std::vector<VkFence> m_Fences;
		std::vector<VkQueryPool> m_TimestampQueryPools;
		std::vector<VkQueryPool> m_PipelineStatisticsQueryPools;
		std::vector<std::vector<uint64>> m_TimestampQueryResults;
		std::vector<std::vector<float>> m_ExecutionGPUTimes;
		std::vector<PipelineStatistics> m_PipelineStatisticsQueryResults;

		uint32 m_TimestampQueryCount = 0;
		uint32 m_TimestampNextAvailQuery = 0;
		uint32 m_PipelineQueryCount = 0;
	};
}

#endif // HIGHLO_API_VULKAN

