// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-12-04) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Vulkan.h"
#include "VulkanDevice.h"

namespace highlo
{
	struct GPUMemoryStats
	{
		uint64 Used = 0;
		uint64 Free = 0;
	};

	class VulkanAllocator
	{
	public:

		VulkanAllocator() = default;
		VulkanAllocator(const HLString &tag);
		~VulkanAllocator();

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

		static void DumpStats();
		static GPUMemoryStats GetStats();

		static void Init(const Ref<VulkanDevice> &device);
		static void Shutdown();

		static VmaAllocator &GetVMAAllocator();

	private:

		HLString m_Tag = "";
	};
}

#endif // HIGHLO_API_VULKAN

