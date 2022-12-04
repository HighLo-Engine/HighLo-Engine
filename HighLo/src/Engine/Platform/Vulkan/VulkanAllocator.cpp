// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanAllocator.h"

#ifdef HIGHLO_API_VULKAN

#include "Engine/Utils/StringUtils.h"
#include "VulkanContext.h"

namespace highlo
{
	struct VulkanAllocatorData
	{
		VmaAllocator Allocator;
		uint64 TotalAllocatedBytes = 0;
	};

	static VulkanAllocatorData *s_VulkanAllocatorData = nullptr;

	VulkanAllocator::VulkanAllocator(const HLString &tag)
		: m_Tag(tag)
	{
	}

	VulkanAllocator::~VulkanAllocator()
	{
	}

	VmaAllocation VulkanAllocator::AllocateBuffer(VkBufferCreateInfo bufferCreateInfo, VmaMemoryUsage usage, VkBuffer &outBuffer)
	{
		VmaAllocationCreateInfo allocCreateInfo = {};
		allocCreateInfo.usage = usage;

		VmaAllocation allocation;
		vmaCreateBuffer(s_VulkanAllocatorData->Allocator, &bufferCreateInfo, &allocCreateInfo, &outBuffer, &allocation, nullptr);

		// TODO: Tracking
		VmaAllocationInfo allocInfo = {};
		vmaGetAllocationInfo(s_VulkanAllocatorData->Allocator, allocation, &allocInfo);
		HL_CORE_TRACE("VulkanAllocator ({0}): allocating buffer; size = {1}", m_Tag, utils::BytesToString(allocInfo.size));

		{
			s_VulkanAllocatorData->TotalAllocatedBytes += allocInfo.size;
			HL_CORE_TRACE("VulkanAllocator ({0}): total allocated since start is {1}", m_Tag, utils::BytesToString(s_VulkanAllocatorData->TotalAllocatedBytes));
		}

		return allocation;
	}
	
	VmaAllocation VulkanAllocator::AllocateImage(VkImageCreateInfo imageCreateInfo, VmaMemoryUsage usage, VkImage &outImage)
	{
		VmaAllocationCreateInfo allocCreateInfo = {};
		allocCreateInfo.usage = usage;

		VmaAllocation allocation;
		vmaCreateImage(s_VulkanAllocatorData->Allocator, &imageCreateInfo, &allocCreateInfo, &outImage, &allocation, nullptr);

		// TODO: Tracking
		VmaAllocationInfo allocInfo;
		vmaGetAllocationInfo(s_VulkanAllocatorData->Allocator, allocation, &allocInfo);
		HL_CORE_TRACE("VulkanAllocator ({0}): allocating image; size = {1}", *m_Tag, utils::BytesToString(allocInfo.size));

		s_VulkanAllocatorData->TotalAllocatedBytes += allocInfo.size;
		HL_CORE_TRACE("VulkanAllocator ({0}): total allocated since start is {1}", *m_Tag, utils::BytesToString(s_VulkanAllocatorData->TotalAllocatedBytes));

		return allocation;
	}
	
	void VulkanAllocator::Free(VmaAllocation allocation)
	{
		vmaFreeMemory(s_VulkanAllocatorData->Allocator, allocation);
	}
	
	void VulkanAllocator::DestroyImage(VkImage image, VmaAllocation allocation)
	{
		HL_ASSERT(image);
		HL_ASSERT(allocation);
		vmaDestroyImage(s_VulkanAllocatorData->Allocator, image, allocation);
	}
	
	void VulkanAllocator::DestroyBuffer(VkBuffer buffer, VmaAllocation allocation)
	{
		HL_ASSERT(buffer);
		HL_ASSERT(allocation);
		vmaDestroyBuffer(s_VulkanAllocatorData->Allocator, buffer, allocation);
	}
	
	void VulkanAllocator::UnmapMemory(VmaAllocation allocation)
	{
		vmaUnmapMemory(s_VulkanAllocatorData->Allocator, allocation);
	}
	
	void VulkanAllocator::DumpStats()
	{
		const auto &memoryProps = VulkanContext::GetVulkanCurrentDevice()->GetVulkanPhysicalDevice()->GetMemoryProperties();
		std::vector<VmaBudget> budgets(memoryProps.memoryHeapCount);
		vmaGetBudget(s_VulkanAllocatorData->Allocator, budgets.data());

		HL_CORE_WARN("-----------------------------------");
		for (VmaBudget &b : budgets)
		{
			HL_CORE_WARN("VmaBudget.allocationBytes = {0}", utils::BytesToString(b.allocationBytes));
			HL_CORE_WARN("VmaBudget.blockBytes = {0}", utils::BytesToString(b.blockBytes));
			HL_CORE_WARN("VmaBudget.usage = {0}", utils::BytesToString(b.usage));
			HL_CORE_WARN("VmaBudget.budget = {0}", utils::BytesToString(b.budget));
		}
		HL_CORE_WARN("-----------------------------------");
	}
	
	GPUMemoryStats VulkanAllocator::GetStats()
	{
		const auto &memoryProps = VulkanContext::GetVulkanCurrentDevice()->GetVulkanPhysicalDevice()->GetMemoryProperties();
		std::vector<VmaBudget> budgets(memoryProps.memoryHeapCount);
		vmaGetBudget(s_VulkanAllocatorData->Allocator, budgets.data());

		uint64_t usage = 0;
		uint64_t budget = 0;

		for (VmaBudget &b : budgets)
		{
			usage += b.usage;
			budget += b.budget;
		}

		// Ternary because budget can somehow be smaller than usage.
		return { usage, budget > usage ? budget - usage : 0ull };
	}
	
	void VulkanAllocator::Init(const Ref<VulkanDevice> &device)
	{
		s_VulkanAllocatorData = new VulkanAllocatorData();

		VmaAllocatorCreateInfo allocatorInfo = {};
		allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_2;
		allocatorInfo.physicalDevice = device->GetVulkanPhysicalDevice()->GetNativeDevice();
		allocatorInfo.device = device->GetNativeDevice();
		allocatorInfo.instance = VulkanContext::GetVulkanCurrentInstance();

		vmaCreateAllocator(&allocatorInfo, &s_VulkanAllocatorData->Allocator);
	}
	
	void VulkanAllocator::Shutdown()
	{
		vmaDestroyAllocator(s_VulkanAllocatorData->Allocator);

		delete s_VulkanAllocatorData;
		s_VulkanAllocatorData = nullptr;
	}
	
	VmaAllocator &VulkanAllocator::GetVMAAllocator()
	{
		return s_VulkanAllocatorData->Allocator;
	}
}

#endif // HIGHLO_API_VULKAN

