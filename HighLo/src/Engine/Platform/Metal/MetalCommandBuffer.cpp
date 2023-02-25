// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MetalCommandBuffer.h"

#ifdef HIGHLO_API_METAL

namespace highlo
{
	MetalCommandBuffer::MetalCommandBuffer(uint32 count, const HLString &debugName)
		: m_DebugName(debugName)
	{
	}

	MetalCommandBuffer::MetalCommandBuffer(const HLString &debugName, bool swapChain)
		: m_DebugName(debugName)
	{
	}

	MetalCommandBuffer::~MetalCommandBuffer()
	{
	}
	
	void MetalCommandBuffer::Begin()
	{
	}
	
	void MetalCommandBuffer::End()
	{
	}
	
	void MetalCommandBuffer::Submit()
	{
	}
	
	float MetalCommandBuffer::GetExecutionGPUTime(uint32 frameIndex, uint32 queryIndex) const
	{
		return 0.0f;
	}
	
	uint32 MetalCommandBuffer::BeginTimestampQuery()
	{
		return 0;
	}
	
	void MetalCommandBuffer::EndTimestampQuery(uint32 queryID)
	{
	}
	
	const PipelineStatistics &MetalCommandBuffer::GetPipelineStatistics(uint32 frameIndex) const
	{
		HL_ASSERT(frameIndex < m_PipelineStatisticsQueryResults.size());
		return m_PipelineStatisticsQueryResults[frameIndex];
	}
}

#endif // HIGHLO_API_METAL

