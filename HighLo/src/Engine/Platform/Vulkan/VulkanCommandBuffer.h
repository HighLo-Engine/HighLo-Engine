#pragma once

#include "Engine/Graphics/CommandBuffer.h"

#ifdef HIGHLO_API_VULKAN

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

		virtual float GetCPUExecutionTime(uint32 frameIndex, uint32 queryIndex = 0) const override;

		virtual uint64 BeginTimestampQuery() override;
		virtual void EndTimestampQuery(uint64 queryID) override;

	private:


	};
}

#endif // HIGHLO_API_VULKAN

