// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

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

	float DX12CommandBuffer::GetCPUExecutionTime(uint32 frameIndex, uint32 queryIndex) const
	{
		return 0.0f;
	}

	uint64 DX12CommandBuffer::BeginTimestampQuery()
	{
		return uint64();
	}

	void DX12CommandBuffer::EndTimestampQuery(uint64 queryID)
	{
	}
}

#endif // HIGHLO_API_DX12

