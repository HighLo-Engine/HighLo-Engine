#pragma once

#include "Engine/Graphics/CommandBuffer.h"

#ifdef HIGHLO_API_OPENGL

namespace highlo
{
	class OpenGLCommandBuffer : public CommandBuffer
	{
	public:

		OpenGLCommandBuffer(uint32 count = 0, const HLString &debugName = "");
		OpenGLCommandBuffer(const HLString &debugName = "", bool swapChain = false);
		virtual ~OpenGLCommandBuffer();

		virtual void Begin() override;
		virtual void End() override;
		virtual void Submit() override;

		virtual float GetExecutionGPUTime(uint32 frameIndex, uint32 queryIndex = 0) const override;

		virtual uint32 BeginTimestampQuery() override;
		virtual void EndTimestampQuery(uint32 queryIndex) override;

		virtual const PipelineStatistics &GetPipelineStatistics(uint32 frameIndex) const override;

	private:

		HLString m_DebugName;
		std::vector<PipelineStatistics> m_PipelineStatisticsQueryResults;

		// GPU time measuring index
		uint32 m_TimestampQueryCount = 0;
		uint32 m_TimestampNextAvailQuery = 0;
		
		// GPU time measuring
		std::vector<uint32> m_TimestampQueryPool;
		std::vector<std::vector<float>> m_ExecutionGPUTimes;
	};
}

#endif // HIGHLO_API_OPENGL

