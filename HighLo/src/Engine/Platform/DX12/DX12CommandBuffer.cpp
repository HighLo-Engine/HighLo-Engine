// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX12CommandBuffer.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	DX12CommandBuffer::DX12CommandBuffer(uint32 count, const HLString &debugName)
		: m_DebugName(debugName)
	{
	}

	DX12CommandBuffer::DX12CommandBuffer(const HLString &debugName, bool swapChain)
		: m_DebugName(debugName)
	{
	}

	DX12CommandBuffer::~DX12CommandBuffer()
	{
	}

	void DX12CommandBuffer::Begin()
	{
	}

	void DX12CommandBuffer::End()
	{
	}

	void DX12CommandBuffer::Submit()
	{
	}

	float DX12CommandBuffer::GetExecutionGPUTime(uint32 frameIndex, uint32 queryIndex) const
	{
		return 0.0f;
	}

	uint32 DX12CommandBuffer::BeginTimestampQuery()
	{
		return 0;
	}

	void DX12CommandBuffer::EndTimestampQuery(uint32 queryID)
	{
	}
	
	const PipelineStatistics &DX12CommandBuffer::GetPipelineStatistics(uint32 frameIndex) const
	{
		HL_ASSERT(frameIndex < m_PipelineStatisticsQueryResults.size());
		return m_PipelineStatisticsQueryResults[frameIndex];
	}
}

#endif // HIGHLO_API_DX12

