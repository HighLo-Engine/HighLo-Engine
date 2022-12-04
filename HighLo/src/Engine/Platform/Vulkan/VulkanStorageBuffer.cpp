// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanStorageBuffer.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	VulkanStorageBuffer::VulkanStorageBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout)
		: StorageBuffer(binding, layout)
	{
	}
	
	VulkanStorageBuffer::~VulkanStorageBuffer()
	{
	}

	void VulkanStorageBuffer::UploadToShader()
	{
	}

	void VulkanStorageBuffer::Resize(uint32 size)
	{
	}
}

#endif // HIGHLO_API_VULKAN

