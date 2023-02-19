// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MetalUniformBuffer.h"

#ifdef HIGHLO_API_METAL

namespace highlo
{
	MetalUniformBuffer::MetalUniformBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout)
		: UniformBuffer(binding, layout), m_Size(size)
	{
	}

	MetalUniformBuffer::~MetalUniformBuffer()
	{
	}
	
	void MetalUniformBuffer::Bind() const
	{
	}
	
	void MetalUniformBuffer::Unbind() const
	{
	}
	
	void MetalUniformBuffer::UploadToShader()
	{
	}
}

#endif // HIGHLO_API_METAL

