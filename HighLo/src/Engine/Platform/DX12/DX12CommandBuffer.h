// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-02-02) initial release
//

#pragma once

#include "Engine/Graphics/CommandBuffer.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	class DX12CommandBuffer : public CommandBuffer
	{
	public:

		DX12CommandBuffer(uint32 count = 0, const HLString &debugName = "");
		DX12CommandBuffer(const HLString &debugName = "", bool swapChain = false);
		virtual ~DX12CommandBuffer();

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

#endif // HIGHLO_API_DX12

