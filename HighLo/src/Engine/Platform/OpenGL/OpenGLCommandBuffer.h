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
		virtual void EndTimestampQuery(uint32 queryID) override;

		virtual const PipelineStatistics &GetPipelineStatistics(uint32 frameIndex) const override;

	private:

		HLString m_DebugName;
		std::vector<PipelineStatistics> m_PipelineStatisticsQueryResults;
	};
}

#endif // HIGHLO_API_OPENGL

