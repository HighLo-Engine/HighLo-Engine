// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX12VertexBuffer.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	DX12VertexBuffer::DX12VertexBuffer(void *data, uint32 size, VertexBufferUsage usage)
		: m_Usage(usage), m_Size(size)
	{
		m_Buffer = Allocator::Copy(data, size);
	}

	DX12VertexBuffer::DX12VertexBuffer(uint32 size, VertexBufferUsage usage)
		: m_Usage(usage), m_Size(size)
	{
	}
	
	DX12VertexBuffer::~DX12VertexBuffer()
	{
		m_Buffer.Release();
	}
	
	void DX12VertexBuffer::Bind() const
	{
	}
	
	void DX12VertexBuffer::Unbind() const
	{
	}
	
	void DX12VertexBuffer::UpdateContents(void *data, uint32 size, uint32 offset)
	{
		m_Size = size;

		if (m_Buffer)
			m_Buffer.Release();

		m_Buffer = Allocator::Copy(data, size);
	}
}

#endif // HIGHLO_API_DX12

