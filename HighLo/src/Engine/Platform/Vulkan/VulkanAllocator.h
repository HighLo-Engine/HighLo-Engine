#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Vulkan.h"
#include <vk_mem_alloc.h>

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
		VmaAllocation AllocateImage(VkImageCreateInfo imageCreateInfo, VmaMemoryUsage usage, VkImage &outImage, VkDeviceSize *allocatedSize = nullptr);
		
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

		static void Init(Ref<VulkanDevice> device);
		static void Shutdown();

		static VmaAllocator &GetVMAAllocator();

	private:

		HLString m_Tag;
	};
}

#endif // HIGHLO_API_VULKAN

