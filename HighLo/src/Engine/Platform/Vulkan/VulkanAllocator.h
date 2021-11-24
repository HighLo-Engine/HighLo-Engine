// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-24) initial release
//

#pragma once

#include "Vulkan.h"
#include "VulkanDevice.h"

#include <../vendor/VulkanMemoryAllocator/vk_mem_alloc.h>

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	struct GPUMemoryStats
	{
		uint64 Used  = 0;
		uint64 Freed = 0;
	};

	class VulkanAllocator
	{
	public:

		VulkanAllocator() = default;
		VulkanAllocator(const HLString &tag);
		~VulkanAllocator();

		static void Init(Ref<VulkanDevice> &device);
		static void Shutdown();

		static void DumpStats();
		static GPUMemoryStats GetStats();
		static VmaAllocator &GetVMAAllocator();

		VmaAllocation AllocateBuffer(VkBufferCreateInfo bufferCreateInfo, VmaMemoryUsage usage, VkBuffer &outBuffer);
		VmaAllocation AllocateImage(VkImageCreateInfo imageCreateInfo, VmaMemoryUsage usage, VkImage &outImage);
		void Free(VmaAllocation allocation);
		void DestroyImage(VkImage image, VmaAllocation allocation);
		void DestroyBuffer(VkBuffer buffer, VmaAllocation allocation);

		template<typename T>
		T *MapMemory(VmaAllocation allocation)
		{
			T *mappedMemory;
			vmaMapMemory(VulkanAllocator::GetVMAAllocator(), allocation, (void**)&mappedMemory);
			return mappedMemory;
		}

		void UnmapMemory(VmaAllocation allocation);

	private:

		HLString m_Tag;
	};
}

#endif // HIGHLO_API_VULKAN

