#pragma once

#include "Engine/Renderer/Renderer.h"

#ifdef HIGHLO_API_VULKAN

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include "VulkanDevice.h"
#include "VulkanContext.h"

inline PFN_vkSetDebugUtilsObjectNameEXT fpSetDebugUtilsObjectNameEXT;
inline PFN_vkCmdBeginDebugUtilsLabelEXT fpCmdBeginDebugUtilsLabelEXT;
inline PFN_vkCmdEndDebugUtilsLabelEXT fpCmdEndDebugUtilsLabelEXT;
inline PFN_vkCmdInsertDebugUtilsLabelEXT fpCmdInsertDebugUtilsLabelEXT;

namespace highlo::utils
{
	inline void LoadDebugExtensions(VkInstance instance)
	{
		fpSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)(vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT"));
		if (!fpSetDebugUtilsObjectNameEXT)
		{
			fpSetDebugUtilsObjectNameEXT = [](VkDevice device, const VkDebugUtilsObjectNameInfoEXT *pNameInfo) -> VkResult
			{
				return VK_SUCCESS;
			};
		}

		fpCmdBeginDebugUtilsLabelEXT = (PFN_vkCmdBeginDebugUtilsLabelEXT)(vkGetInstanceProcAddr(instance, "vkCmdBeginDebugUtilsLabelEXT"));
		if (!fpCmdBeginDebugUtilsLabelEXT)
		{
			fpCmdBeginDebugUtilsLabelEXT = [](VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT *pLabelInfo)
			{
			};
		}

		fpCmdEndDebugUtilsLabelEXT = (PFN_vkCmdEndDebugUtilsLabelEXT)(vkGetInstanceProcAddr(instance, "vkCmdEndDebugUtilsLabelEXT"));
		if (!fpCmdEndDebugUtilsLabelEXT)
		{
			fpCmdEndDebugUtilsLabelEXT = [](VkCommandBuffer commandBuffer)
			{
			};
		}

		fpCmdInsertDebugUtilsLabelEXT = (PFN_vkCmdInsertDebugUtilsLabelEXT)(vkGetInstanceProcAddr(instance, "vkCmdInsertDebugUtilsLabelEXT"));
		if (!fpCmdInsertDebugUtilsLabelEXT)
		{
			fpCmdInsertDebugUtilsLabelEXT = [](VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT *pLabelInfo)
			{
			};
		}
	}

	void RetrieveDiagnosticCheckpoints();

	inline const char *VulkanStageToString(VkPipelineStageFlagBits flags)
	{
		switch (flags)
		{
			case VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT: return "VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT";
			case VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT: return "VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT";
		}

		HL_ASSERT(false);
		return nullptr;
	}

	inline const char *VulkanResultToString(VkResult result)
	{
		switch (result)
		{
			case VK_SUCCESS: return "VK_SUCCESS";
			case VK_NOT_READY: return "VK_NOT_READY";
			case VK_TIMEOUT: return "VK_TIMEOUT";
			case VK_EVENT_SET: return "VK_EVENT_SET";
			case VK_EVENT_RESET: return "VK_EVENT_RESET";
			case VK_INCOMPLETE: return "VK_INCOMPLETE";
			case VK_ERROR_OUT_OF_HOST_MEMORY: return "VK_ERROR_OUT_OF_HOST_MEMORY";
			case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
			case VK_ERROR_INITIALIZATION_FAILED: return "VK_ERROR_INITIALIZATION_FAILED";
			case VK_ERROR_DEVICE_LOST: return "VK_ERROR_DEVICE_LOST";
			case VK_ERROR_MEMORY_MAP_FAILED: return "VK_ERROR_MEMORY_MAP_FAILED";
			case VK_ERROR_LAYER_NOT_PRESENT: return "VK_ERROR_LAYER_NOT_PRESENT";
			case VK_ERROR_EXTENSION_NOT_PRESENT: return "VK_ERROR_EXTENSION_NOT_PRESENT";
			case VK_ERROR_FEATURE_NOT_PRESENT: return "VK_ERROR_FEATURE_NOT_PRESENT";
			case VK_ERROR_INCOMPATIBLE_DRIVER: return "VK_ERROR_INCOMPATIBLE_DRIVER";
			case VK_ERROR_TOO_MANY_OBJECTS: return "VK_ERROR_TOO_MANY_OBJECTS";
			case VK_ERROR_FORMAT_NOT_SUPPORTED: return "VK_ERROR_FORMAT_NOT_SUPPORTED";
			case VK_ERROR_FRAGMENTED_POOL: return "VK_ERROR_FRAGMENTED_POOL";
			case VK_ERROR_UNKNOWN: return "VK_ERROR_UNKNOWN";
			case VK_ERROR_OUT_OF_POOL_MEMORY: return "VK_ERROR_OUT_OF_POOL_MEMORY";
			case VK_ERROR_INVALID_EXTERNAL_HANDLE: return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
			case VK_ERROR_FRAGMENTATION: return "VK_ERROR_FRAGMENTATION";
			case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
			case VK_ERROR_SURFACE_LOST_KHR: return "VK_ERROR_SURFACE_LOST_KHR";
			case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
			case VK_SUBOPTIMAL_KHR: return "VK_SUBOPTIMAL_KHR";
			case VK_ERROR_OUT_OF_DATE_KHR: return "VK_ERROR_OUT_OF_DATE_KHR";
			case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
			case VK_ERROR_VALIDATION_FAILED_EXT: return "VK_ERROR_VALIDATION_FAILED_EXT";
			case VK_ERROR_INVALID_SHADER_NV: return "VK_ERROR_INVALID_SHADER_NV";
			case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
			case VK_ERROR_NOT_PERMITTED_EXT: return "VK_ERROR_NOT_PERMITTED_EXT";
			case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT: return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
			case VK_THREAD_IDLE_KHR: return "VK_THREAD_IDLE_KHR";
			case VK_THREAD_DONE_KHR: return "VK_THREAD_DONE_KHR";
			case VK_OPERATION_DEFERRED_KHR: return "VK_OPERATION_DEFERRED_KHR";
			case VK_OPERATION_NOT_DEFERRED_KHR: return "VK_OPERATION_NOT_DEFERRED_KHR";
			case VK_PIPELINE_COMPILE_REQUIRED_EXT: return "VK_PIPELINE_COMPILE_REQUIRED_EXT";
		}

		HL_ASSERT(false);
		return nullptr;
	}

	inline const char *VulkanObjectTypeToString(VkObjectType obj)
	{
		switch (obj)
		{
			case VK_OBJECT_TYPE_COMMAND_BUFFER: return "VK_OBJECT_TYPE_COMMAND_BUFFER";
			case VK_OBJECT_TYPE_PIPELINE: return "VK_OBJECT_TYPE_PIPELINE";
			case VK_OBJECT_TYPE_FRAMEBUFFER: return "VK_OBJECT_TYPE_FRAMEBUFFER";
			case VK_OBJECT_TYPE_IMAGE: return "VK_OBJECT_TYPE_IMAGE";
			case VK_OBJECT_TYPE_QUERY_POOL: return "VK_OBJECT_TYPE_QUERY_POOL";
			case VK_OBJECT_TYPE_RENDER_PASS: return "VK_OBJECT_TYPE_RENDER_PASS";
			case VK_OBJECT_TYPE_COMMAND_POOL: return "VK_OBJECT_TYPE_COMMAND_POOL";
			case VK_OBJECT_TYPE_PIPELINE_CACHE: return "VK_OBJECT_TYPE_PIPELINE_CACHE";
			case VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR: return "VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR";
			case VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV: return "VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV";
			case VK_OBJECT_TYPE_BUFFER: return "VK_OBJECT_TYPE_BUFFER";
			case VK_OBJECT_TYPE_BUFFER_VIEW: return "VK_OBJECT_TYPE_BUFFER_VIEW";
			case VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT: return "VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT";
			case VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT: return "VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT";
			case VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR: return "VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR";
			case VK_OBJECT_TYPE_DESCRIPTOR_POOL: return "VK_OBJECT_TYPE_DESCRIPTOR_POOL";
			case VK_OBJECT_TYPE_DESCRIPTOR_SET: return "VK_OBJECT_TYPE_DESCRIPTOR_SET";
			case VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT: return "VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT";
			case VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE: return "VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE";
			case VK_OBJECT_TYPE_PRIVATE_DATA_SLOT_EXT: return "VK_OBJECT_TYPE_PRIVATE_DATA_SLOT_EXT";
			case VK_OBJECT_TYPE_DEVICE: return "VK_OBJECT_TYPE_DEVICE";
			case VK_OBJECT_TYPE_DEVICE_MEMORY: return "VK_OBJECT_TYPE_DEVICE_MEMORY";
			case VK_OBJECT_TYPE_PIPELINE_LAYOUT: return "VK_OBJECT_TYPE_PIPELINE_LAYOUT";
			case VK_OBJECT_TYPE_DISPLAY_KHR: return "VK_OBJECT_TYPE_DISPLAY_KHR";
			case VK_OBJECT_TYPE_DISPLAY_MODE_KHR: return "VK_OBJECT_TYPE_DISPLAY_MODE_KHR";
			case VK_OBJECT_TYPE_PHYSICAL_DEVICE: return "VK_OBJECT_TYPE_PHYSICAL_DEVICE";
			case VK_OBJECT_TYPE_EVENT: return "VK_OBJECT_TYPE_EVENT";
			case VK_OBJECT_TYPE_FENCE: return "VK_OBJECT_TYPE_FENCE";
			case VK_OBJECT_TYPE_IMAGE_VIEW: return "VK_OBJECT_TYPE_IMAGE_VIEW";
			case VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NV: return "VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NV";
			case VK_OBJECT_TYPE_INSTANCE: return "VK_OBJECT_TYPE_INSTANCE";
			case VK_OBJECT_TYPE_PERFORMANCE_CONFIGURATION_INTEL: return "VK_OBJECT_TYPE_PERFORMANCE_CONFIGURATION_INTEL";
			case VK_OBJECT_TYPE_QUEUE: return "VK_OBJECT_TYPE_QUEUE";
			case VK_OBJECT_TYPE_SAMPLER: return "VK_OBJECT_TYPE_SAMPLER";
			case VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION: return "VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION";
			case VK_OBJECT_TYPE_SEMAPHORE: return "VK_OBJECT_TYPE_SEMAPHORE";
			case VK_OBJECT_TYPE_SHADER_MODULE: return "VK_OBJECT_TYPE_SHADER_MODULE";
			case VK_OBJECT_TYPE_SURFACE_KHR: return "VK_OBJECT_TYPE_SURFACE_KHR";
			case VK_OBJECT_TYPE_SWAPCHAIN_KHR: return "VK_OBJECT_TYPE_SWAPCHAIN_KHR";
			case VK_OBJECT_TYPE_VALIDATION_CACHE_EXT: return "VK_OBJECT_TYPE_VALIDATION_CACHE_EXT";
			case VK_OBJECT_TYPE_UNKNOWN: return "VK_OBJECT_TYPE_UNKNOWN";
			case VK_OBJECT_TYPE_MAX_ENUM: return "VK_OBJECT_TYPE_MAX_ENUM";
		}

		HL_ASSERT(false);
		return nullptr;
	}

	inline static void VulkanCheckResult(VkResult result)
	{
		if (result != VK_SUCCESS)
		{
			HL_CORE_ERROR("VkResult is '{0}' in {1}:{2}", ::highlo::utils::VulkanResultToString(result), __FILE__, __LINE__);
			if (result == VK_ERROR_DEVICE_LOST)
			{
				using namespace std::chrono_literals;
				std::this_thread::sleep_for(3s);
				::highlo::utils::RetrieveDiagnosticCheckpoints();
				::highlo::utils::DumpGPUInfos();
			}
			HL_ASSERT(result == VK_SUCCESS);
		}
	}

	inline static void SetDebugUtilsObjectName(const VkDevice &device, const VkObjectType obj, const HLString &name, const void *handle)
	{
		VkDebugUtilsObjectNameInfoEXT nameInfo;
		nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
		nameInfo.objectType = obj;
		nameInfo.pObjectName = *name;
		nameInfo.objectHandle = (uint64)handle;
		nameInfo.pNext = VK_NULL_HANDLE;

		VkResult result = fpSetDebugUtilsObjectNameEXT(device, &nameInfo);
		VulkanCheckResult(result);
	}

	// Allocation helpers

	struct VulkanAllocationData
	{
		VmaAllocator Allocator;
		uint64 TotalAllocatedBytes = 0;
	};

	static VulkanAllocationData *s_VulkanAllocationData = nullptr;

	static void InitAllocator(const Ref<VulkanDevice> &device)
	{
		s_VulkanAllocationData = new VulkanAllocationData();
		
		VmaAllocatorCreateInfo allocCreateInfo = {};
		allocCreateInfo.vulkanApiVersion = VK_API_VERSION_1_2;
		allocCreateInfo.device = device->GetNativeDevice();
		allocCreateInfo.physicalDevice = device->GetPhysicalDevice()->GetNativeDevice();
		allocCreateInfo.instance = VulkanContext::GetInstance();

		vmaCreateAllocator(&allocCreateInfo, &s_VulkanAllocationData->Allocator);
	}

	static void ShutdownAllocator()
	{
		vmaDestroyAllocator(s_VulkanAllocationData->Allocator);

		delete s_VulkanAllocationData;
		s_VulkanAllocationData = nullptr;
	}

	static VmaAllocator GetVMAAllocator()
	{
		return s_VulkanAllocationData->Allocator;
	}

	static void FreeAllocation(VmaAllocation allocation)
	{
		vmaFreeMemory(s_VulkanAllocationData->Allocator, allocation);
	}

	static VmaAllocation AllocateBuffer(VkBufferCreateInfo createInfo, VmaMemoryUsage usage, VkBuffer &outBuffer)
	{
		VmaAllocationCreateInfo allocCreateInfo = {};
		allocCreateInfo.usage = usage;

		VmaAllocation allocation;
		vmaCreateBuffer(s_VulkanAllocationData->Allocator, &createInfo, &allocCreateInfo, &outBuffer, &allocation, nullptr);

		VmaAllocationInfo allocInfo = {};
		vmaGetAllocationInfo(s_VulkanAllocationData->Allocator, allocation, &allocInfo);

		// Track the memory usage
		s_VulkanAllocationData->TotalAllocatedBytes += allocInfo.size;

		return allocation;
	}

	static void DestroyBuffer(VkBuffer buffer, VmaAllocation allocation)
	{
		HL_ASSERT(buffer);
		HL_ASSERT(allocation);
		vmaDestroyBuffer(s_VulkanAllocationData->Allocator, buffer, allocation);
	}

	static VmaAllocation AllocateImage(VkImageCreateInfo createInfo, VmaMemoryUsage usage, VkImage &outImage)
	{
		VmaAllocationCreateInfo allocCreateInfo = {};
		allocCreateInfo.usage = usage;

		VmaAllocation allocation;
		vmaCreateImage(s_VulkanAllocationData->Allocator, &createInfo, &allocCreateInfo, &outImage, &allocation, nullptr);

		VmaAllocationInfo allocInfo = {};
		vmaGetAllocationInfo(s_VulkanAllocationData->Allocator, allocation, &allocInfo);

		// Track the memory usage
		s_VulkanAllocationData->TotalAllocatedBytes += allocInfo.size;

		return allocation;
	}

	static void DestroyImage(VkImage image, VmaAllocation allocation)
	{
		HL_ASSERT(image);
		HL_ASSERT(allocation);
		vmaDestroyImage(s_VulkanAllocationData->Allocator, image, allocation);
	}

	template<typename T>
	static T *MapMemory(VmaAllocation allocation)
	{
		T *mappedMemory;
		vmaMapMemory(s_VulkanAllocationData->Allocator, allocation, (void**)&mappedMemory);
		return mappedMemory;
	}

	static void UnmapMemory(VmaAllocation allocation)
	{
		vmaUnmapMemory(s_VulkanAllocationData->Allocator, allocation);
	}
}

#define VK_CHECK_RESULT(f)\
{\
	VkResult res = (f);\
	::highlo::utils::VulkanCheckResult(res);\
}

#endif // HIGHLO_API_VULKAN

