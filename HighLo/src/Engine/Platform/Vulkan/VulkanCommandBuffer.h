// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-23) initial release
//

#pragma once

#include "Engine/Renderer/CommandBuffer.h"
#include "Engine/Renderer/PipelineStatistics.h"

#ifdef HIGHLO_API_VULKAN

#include "Vulkan.h"

namespace highlo
{
	class VulkanCommandBuffer : public CommandBuffer
	{
	public:

		VulkanCommandBuffer(uint32 count = 0, const HLString &debugName = "");
		VulkanCommandBuffer(const HLString &debugName = "", bool swapChain = false);
		virtual ~VulkanCommandBuffer();

		virtual void Begin() override;
		virtual void End() override;
		virtual void Submit() override;

		virtual float GetCPUExecutionTime(uint32 frameIndex, uint32 queryIndex = 0) const override
		{
			if (queryIndex / 2 >= m_TimestampNextAvailQuery / 2)
			{
				return 0.0f;
			}

			return m_ExecutionGPUTimes[frameIndex][queryIndex / 2];
		}

		virtual uint64 BeginTimestampQuery() override;
		virtual void EndTimestampQuery(uint64 queryID) override;

		// Vulkan-specific

		VkCommandBuffer GetCommandBuffer(uint32 frameIndex) const
		{
			HL_ASSERT(frameIndex < m_CommandBuffers.size());
			return m_CommandBuffers[frameIndex];
		}

	private:

		HLString m_DebugName;
		VkCommandPool m_CommandPool = nullptr;
		std::vector<VkFence> m_WaitFences;
		bool m_OwnedFromSwapChain = false;

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

