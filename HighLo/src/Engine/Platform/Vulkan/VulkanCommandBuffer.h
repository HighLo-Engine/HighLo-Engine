// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-11-19) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Engine/Graphics/CommandBuffer.h"

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

		virtual float GetExecutionGPUTime(uint32 frameIndex, uint32 queryIndex = 0) const override;

		virtual uint32 BeginTimestampQuery() override;
		virtual void EndTimestampQuery(uint32 queryID) override;

		virtual const PipelineStatistics &GetPipelineStatistics(uint32 frameIndex) const override;

	private:

		HLString m_DebugName;
		bool m_SwapChain = false;
		uint32 m_Count = 0;
		std::vector<PipelineStatistics> m_PipelineStatisticsQueryResults;
	};
}

#endif // HIGHLO_API_VULKAN

