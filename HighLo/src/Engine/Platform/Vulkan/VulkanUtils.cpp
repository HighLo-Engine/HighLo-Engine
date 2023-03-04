// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanUtils.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanContext.h"
#include "VulkanCheckpoint.h"

namespace highlo::utils
{
	struct VulkanAllocationData
	{
		VmaAllocator Allocator;
		uint64 TotalAllocatedBytes = 0;
	};

	static VulkanAllocationData *s_VulkanAllocationData;

	void RetrieveDiagnosticCheckpoints()
	{
		bool supported = VulkanContext::GetCurrentDevice()->GetPhysicalDevice()->IsExtensionSupported(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME);
		if (!supported)
			return;

		{
			const uint32 checkpointCount = 4;
			VkCheckpointDataNV data[checkpointCount];
			for (uint32 i = 0; i < checkpointCount; ++i)
				data[i].sType = VK_STRUCTURE_TYPE_CHECKPOINT_DATA_NV;

			uint32 retrievedCount = checkpointCount;
			fpGetQueueCheckpointDataNV(::highlo::VulkanContext::GetCurrentDevice()->GetGraphicsQueue(), &retrievedCount, data);
			HL_CORE_ERROR("RetrieveDiagnosticCheckpoints (Graphics Queue):");
			for (uint32 i = 0; i < retrievedCount; ++i)
			{
				VulkanCheckpointData *checkpoint = (VulkanCheckpointData *)data[i].pCheckpointMarker;
				HL_CORE_ERROR("Checkpoint: {0} (stage: {1})", checkpoint->Data, VulkanStageToString(data[i].stage));
			}
		}
		{
			const uint32 checkpointCount = 4;
			VkCheckpointDataNV data[checkpointCount];
			for (uint32 i = 0; i < checkpointCount; ++i)
				data[i].sType = VK_STRUCTURE_TYPE_CHECKPOINT_DATA_NV;

			uint32 retrievedCount = checkpointCount;
			fpGetQueueCheckpointDataNV(::highlo::VulkanContext::GetCurrentDevice()->GetComputeQueue(), &retrievedCount, data);
			HL_CORE_ERROR("RetrieveDiagnosticCheckpoints (Compute Queue):");
			for (uint32 i = 0; i < retrievedCount; ++i)
			{
				VulkanCheckpointData *checkpoint = (VulkanCheckpointData *)data[i].pCheckpointMarker;
				HL_CORE_ERROR("Checkpoint: {0} (stage: {1})", checkpoint->Data, VulkanStageToString(data[i].stage));
			}
		}
	}

	void InitAllocator(const Ref<VulkanDevice> &device)
	{
		s_VulkanAllocationData = new VulkanAllocationData();

		VmaAllocatorCreateInfo allocCreateInfo = {};
		allocCreateInfo.vulkanApiVersion = VK_API_VERSION_1_2;
		allocCreateInfo.device = device->GetNativeDevice();
		allocCreateInfo.physicalDevice = device->GetPhysicalDevice()->GetNativeDevice();
		allocCreateInfo.instance = VulkanContext::GetInstance();

		vmaCreateAllocator(&allocCreateInfo, &s_VulkanAllocationData->Allocator);
	}

	void ShutdownAllocator()
	{
		vmaDestroyAllocator(s_VulkanAllocationData->Allocator);

		delete s_VulkanAllocationData;
		s_VulkanAllocationData = nullptr;
	}

	void FreeAllocation(VmaAllocation allocation)
	{
		vmaFreeMemory(s_VulkanAllocationData->Allocator, allocation);
	}

	VmaAllocation AllocateBuffer(VkBufferCreateInfo createInfo, VmaMemoryUsage usage, VkBuffer &outBuffer)
	{
		VmaAllocationCreateInfo allocCreateInfo = {};
		allocCreateInfo.usage = usage;

		HL_ASSERT(s_VulkanAllocationData);

		VmaAllocation allocation;
		vmaCreateBuffer(s_VulkanAllocationData->Allocator, &createInfo, &allocCreateInfo, &outBuffer, &allocation, nullptr);

		VmaAllocationInfo allocInfo = {};
		vmaGetAllocationInfo(s_VulkanAllocationData->Allocator, allocation, &allocInfo);

		// Track the memory usage
		s_VulkanAllocationData->TotalAllocatedBytes += allocInfo.size;

		return allocation;
	}

	void DestroyBuffer(VkBuffer buffer, VmaAllocation allocation)
	{
		HL_ASSERT(buffer);
		HL_ASSERT(allocation);
		vmaDestroyBuffer(s_VulkanAllocationData->Allocator, buffer, allocation);
	}

	VmaAllocation AllocateImage(VkImageCreateInfo createInfo, VmaMemoryUsage usage, VkImage &outImage)
	{
		VmaAllocationCreateInfo allocCreateInfo = {};
		allocCreateInfo.usage = usage;

		HL_ASSERT(s_VulkanAllocationData);

		VmaAllocation allocation;
		vmaCreateImage(s_VulkanAllocationData->Allocator, &createInfo, &allocCreateInfo, &outImage, &allocation, nullptr);

		VmaAllocationInfo allocInfo = {};
		vmaGetAllocationInfo(s_VulkanAllocationData->Allocator, allocation, &allocInfo);

		// Track the memory usage
		s_VulkanAllocationData->TotalAllocatedBytes += allocInfo.size;

		return allocation;
	}

	void DestroyImage(VkImage image, VmaAllocation allocation)
	{
		HL_ASSERT(image);
		HL_ASSERT(allocation);
		vmaDestroyImage(s_VulkanAllocationData->Allocator, image, allocation);
	}

	VmaAllocator &GetVMAAllocator()
	{
		return s_VulkanAllocationData->Allocator;
	}

	void UnmapMemory(VmaAllocation allocation)
	{
		vmaUnmapMemory(s_VulkanAllocationData->Allocator, allocation);
	}
}

#endif // HIGHLO_API_VULKAN

