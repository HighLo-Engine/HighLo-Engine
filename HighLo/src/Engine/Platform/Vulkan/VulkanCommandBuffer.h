// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-04-22) initial release
//

#pragma once

#include "Engine/Graphics/CommandBuffer.h"

#ifdef HIGHLO_API_VULKAN

#include <vulkan/vulkan.h>

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

		virtual float GetExecutionGPUTime(uint32 frameIndex, uint32 queryIndex = 0) const override;

		virtual uint32 BeginTimestampQuery() override;
		virtual void EndTimestampQuery(uint32 queryID) override;

		virtual const PipelineStatistics &GetPipelineStatistics(uint32 frameIndex) const override;

		// Vulkan-specific
		VkCommandBuffer GetCommandBuffer(uint32 frameIndex) const
		{
			HL_ASSERT(frameIndex < m_CommandBuffers.size());
			return m_CommandBuffers[frameIndex];
		}

	private:

		HLString m_DebugName;
		bool m_OwnedBySwapchain = false;
		uint32 m_PipelineQueryCount = 0;
		std::vector<PipelineStatistics> m_PipelineStatisticsQueryResults;

		VkCommandPool m_CommandPool = nullptr;
		std::vector<VkCommandBuffer> m_CommandBuffers;
		std::vector<VkFence> m_WaitFences;

		uint32 m_TimestampQueryCount = 0;
		uint32 m_TimestampNextAvailQuery = 0;
		std::vector<VkQueryPool> m_TimestampQueryPools;
		std::vector<VkQueryPool> m_PipelineStatisticsQueryPools;
		std::vector<std::vector<uint32>> m_TimestampQueryResults;
		std::vector<std::vector<float>> m_ExecutionGPUTimes;
	};
}

#endif // HIGHLO_API_VULKAN

