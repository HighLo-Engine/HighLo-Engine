// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MetalStorageBuffer.h"

#ifdef HIGHLO_API_METAL

namespace highlo
{
	MetalStorageBuffer::MetalStorageBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout)
		: StorageBuffer(binding, layout), m_Size(size)
	{
	}

	MetalStorageBuffer::~MetalStorageBuffer()
	{
	}
	
	void MetalStorageBuffer::Bind() const
	{
	}
	
	void MetalStorageBuffer::Unbind() const
	{
	}
	
	void MetalStorageBuffer::UploadToShader()
	{
	}
	
	void MetalStorageBuffer::Resize(uint32 size)
	{
	}
}

#endif // HIGHLO_API_METAL

