// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX12UniformBuffer.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	DX12UniformBuffer::DX12UniformBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout)
		: UniformBuffer(binding, layout), m_Size(size)
	{
	}

	DX12UniformBuffer::~DX12UniformBuffer()
	{
	}
	
	void DX12UniformBuffer::Bind() const
	{
	}
	
	void DX12UniformBuffer::Unbind() const
	{
	}
	
	void DX12UniformBuffer::UploadToShader()
	{
	}
}

#endif // HIGHLO_API_DX12

