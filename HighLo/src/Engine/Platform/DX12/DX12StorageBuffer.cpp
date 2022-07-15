// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX12StorageBuffer.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	DX12StorageBuffer::DX12StorageBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout)
		: StorageBuffer(binding, layout), m_Size(size)
	{
	}
	
	DX12StorageBuffer::~DX12StorageBuffer()
	{
	}
	
	void DX12StorageBuffer::Bind() const
	{
	}
	
	void DX12StorageBuffer::Unbind() const
	{
	}
	
	void DX12StorageBuffer::UploadToShader()
	{
	}

	void DX12StorageBuffer::Resize(uint32 size)
	{
	}
}

#endif // HIGHLO_API_DX12

