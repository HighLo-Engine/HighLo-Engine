// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MetalVertexBuffer.h"

#ifdef HIGHLO_API_METAL

namespace highlo
{
	MetalVertexBuffer::MetalVertexBuffer(void *data, uint32 size, VertexBufferUsage usage)
		: m_Usage(usage), m_Size(size)
	{
		m_Buffer = Allocator::Copy(data, size);
	}

	MetalVertexBuffer::MetalVertexBuffer(uint32 size, VertexBufferUsage usage)
		: m_Usage(usage), m_Size(size)
	{
	}
	
	MetalVertexBuffer::~MetalVertexBuffer()
	{
		m_Buffer.Release();
	}
	
	void MetalVertexBuffer::Bind() const
	{
	}
	
	void MetalVertexBuffer::Unbind() const
	{
	}

	void MetalVertexBuffer::UpdateContents(void *data, uint32 size, uint32 offset)
	{
		if (m_Buffer)
			m_Buffer.Release();
		
		m_Buffer = Allocator::Copy(data, size);
	}
}

#endif // HIGHLO_API_METAL

