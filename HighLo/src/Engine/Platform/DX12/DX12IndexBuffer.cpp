// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX12IndexBuffer.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	DX12IndexBuffer::DX12IndexBuffer(std::vector<int32> &indices)
	{
		m_Count = (uint32)indices.size();
		m_Size = ((uint32)indices.size()) * sizeof(int32);
		m_Buffer = Allocator::Copy(&indices[0], m_Size);

	}

	DX12IndexBuffer::DX12IndexBuffer(void *data, uint32 size)
	{
		m_Count = size / sizeof(int32);
		m_Size = size;
		m_Buffer = Allocator::Copy(data, size);

	}
	
	DX12IndexBuffer::DX12IndexBuffer(uint32 size)
	{
		m_Count = size / sizeof(int32);
		m_Size = size;

	}
	
	DX12IndexBuffer::~DX12IndexBuffer()
	{
		m_Buffer.Release();
	}
	
	void DX12IndexBuffer::Bind() const
	{
	}
	
	void DX12IndexBuffer::Unbind() const
	{
	}
	
	void DX12IndexBuffer::UpdateContents(void *data, uint32 size, uint32 offset)
	{
		HL_ASSERT(offset < size);
		m_Count = size / sizeof(int32);
		m_Size = size;

		if (m_Buffer)
			m_Buffer.Release();

		m_Buffer = Allocator::Copy(data, size);
	}
	
	void DX12IndexBuffer::UpdateContents(std::vector<int32> &indices, uint32 offset)
	{
		m_Count = (uint32)indices.size();
		m_Size = ((uint32)indices.size()) * sizeof(int32);

		if (m_Buffer)
			m_Buffer.Release();

		m_Buffer = Allocator::Copy(&indices[0], m_Size);
	}
}

#endif // HIGHLO_API_DX12

