// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX11CommandBuffer.h"

#ifdef HIGHLO_API_DX11

namespace highlo
{
	DX11CommandBuffer::DX11CommandBuffer(uint32 count, const HLString &debugName)
		: m_DebugName(debugName)
	{
	}

	DX11CommandBuffer::DX11CommandBuffer(const HLString &debugName, bool swapChain)
		: m_DebugName(debugName)
	{
	}
	
	DX11CommandBuffer::~DX11CommandBuffer()
	{
	}
	
	void DX11CommandBuffer::Begin()
	{
	}
	
	void DX11CommandBuffer::End()
	{
	}
	
	void DX11CommandBuffer::Submit()
	{
	}
	
	float DX11CommandBuffer::GetCPUExecutionTime(uint32 frameIndex, uint32 queryIndex) const
	{
		return 0.0f;
	}
	
	uint64 DX11CommandBuffer::BeginTimestampQuery()
	{
		return uint64();
	}
	
	void DX11CommandBuffer::EndTimestampQuery(uint64 queryID)
	{
	}
}

#endif // HIGHLO_API_DX11

