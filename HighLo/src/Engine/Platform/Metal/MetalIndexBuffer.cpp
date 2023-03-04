// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MetalIndexBuffer.h"

#ifdef HIGHLO_API_METAL

namespace highlo
{
	MetalIndexBuffer::MetalIndexBuffer(std::vector<int32> &indices)
	{
		m_Count = (uint32)indices.size();
		m_Size = m_Count * sizeof(int32);
		m_Buffer = Allocator::Copy(&indices[0], m_Size);
	}

	MetalIndexBuffer::MetalIndexBuffer(void *data, uint32 size)
	{
		m_Size = size;
		m_Count = size / sizeof(int32);
		m_Buffer = Allocator::Copy(data, size);
	}
	
	MetalIndexBuffer::MetalIndexBuffer(uint32 size)
	{
		m_Size = size;
		m_Count = size / sizeof(int32);
	}
	
	MetalIndexBuffer::~MetalIndexBuffer()
	{
		m_Buffer.Release();
	}
	
	void MetalIndexBuffer::Bind() const
	{
	}
	
	void MetalIndexBuffer::Unbind() const
	{
	}
	
	void MetalIndexBuffer::UpdateContents(void *data, uint32 size, uint32 offset)
	{
		if (m_Buffer)
			m_Buffer.Release();

		m_Size = size;
		m_Count = size / sizeof(int32);
		m_Buffer = Allocator::Copy(data, size);
	}
	
	void MetalIndexBuffer::UpdateContents(std::vector<int32> &indices, uint32 offset)
	{
		if (m_Buffer)
			m_Buffer.Release();

		m_Count = (uint32)indices.size();
		m_Size = m_Count * sizeof(int32);
		m_Buffer = Allocator::Copy(&indices[0], m_Size);
	}
}

#endif // HIGHLO_API_METAL

