// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Vulkan.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo::utils
{
	void VulkanLoadDebugUtilsExtensions(VkInstance instance)
	{
		fpSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)(vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT"));
		if (fpSetDebugUtilsObjectNameEXT == nullptr)
			fpSetDebugUtilsObjectNameEXT = [](VkDevice device, const VkDebugUtilsObjectNameInfoEXT *pNameInfo) { return VK_SUCCESS; };

		fpCmdBeginDebugUtilsLabelEXT = (PFN_vkCmdBeginDebugUtilsLabelEXT)(vkGetInstanceProcAddr(instance, "vkCmdBeginDebugUtilsLabelEXT"));
		if (fpCmdBeginDebugUtilsLabelEXT == nullptr)
			fpCmdBeginDebugUtilsLabelEXT = [](VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT *pLabelInfo) {};

		fpCmdEndDebugUtilsLabelEXT = (PFN_vkCmdEndDebugUtilsLabelEXT)(vkGetInstanceProcAddr(instance, "vkCmdEndDebugUtilsLabelEXT"));
		if (fpCmdEndDebugUtilsLabelEXT == nullptr)
			fpCmdEndDebugUtilsLabelEXT = [](VkCommandBuffer commandBuffer) {};

		fpCmdInsertDebugUtilsLabelEXT = (PFN_vkCmdInsertDebugUtilsLabelEXT)(vkGetInstanceProcAddr(instance, "vkCmdInsertDebugUtilsLabelEXT"));
		if (fpCmdInsertDebugUtilsLabelEXT == nullptr)
			fpCmdInsertDebugUtilsLabelEXT = [](VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT *pLabelInfo) {};
	}
}

#endif // HIGHLO_API_VULKAN

