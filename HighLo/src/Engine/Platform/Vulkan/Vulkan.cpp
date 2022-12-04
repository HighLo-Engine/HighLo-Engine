// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Vulkan.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanCheckpoint.h"
#include "VulkanContext.h"
#include "VulkanUtils.h"

namespace highlo
{
	void VulkanLoadDebugUtilsExtensions(VkInstance instance)
	{
		fpGetQueueCheckpointDataNV = (PFN_vkGetQueueCheckpointDataNV)(vkGetInstanceProcAddr(instance, "vkGetQueueCheckpointDataNV"));
		if (!fpGetQueueCheckpointDataNV)
		{
			fpGetQueueCheckpointDataNV = [](VkQueue queue, uint32 *retrieveCount, VkCheckpointDataNV *data) -> void
			{
			};
		}

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

		fpCmdSetCheckpointNV = (PFN_vkCmdSetCheckpointNV)(vkGetInstanceProcAddr(instance, "vkCmdSetCheckpointNV"));
		if (!fpCmdSetCheckpointNV)
		{
			fpCmdSetCheckpointNV = [](VkCommandBuffer commandBuffer, const void *marker)
			{
			};
		}
	}

	void RetrieveDiagnosticCheckpoints()
	{
		bool supported = VulkanContext::GetVulkanCurrentDevice()->GetVulkanPhysicalDevice()->IsExtensionSupported(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME);
		if (!supported)
			return;

		{
			const uint32 checkpointCount = 4;
			VkCheckpointDataNV data[checkpointCount];
			for (uint32 i = 0; i < checkpointCount; i++)
				data[i].sType = VK_STRUCTURE_TYPE_CHECKPOINT_DATA_NV;

			uint32 retrievedCount = checkpointCount;
			//vkGetQueueCheckpointDataNV(::highlo::VulkanContext::GetVulkanCurrentDevice()->GetGraphicsQueue(), &retrievedCount, data);
			fpGetQueueCheckpointDataNV(::highlo::VulkanContext::GetVulkanCurrentDevice()->GetGraphicsQueue(), &retrievedCount, data);
			HL_CORE_ERROR("RetrieveDiagnosticCheckpoints (Graphics Queue):");
			for (uint32 i = 0; i < retrievedCount; i++)
			{
				VulkanCheckpointData *checkpoint = (VulkanCheckpointData*)data[i].pCheckpointMarker;
				HL_CORE_ERROR("Checkpoint: {0} (stage: {1})", checkpoint->Data, utils::VulkanStageToString(data[i].stage));
			}
		}

		{
			const uint32 checkpointCount = 4;
			VkCheckpointDataNV data[checkpointCount];
			for (uint32 i = 0; i < checkpointCount; i++)
				data[i].sType = VK_STRUCTURE_TYPE_CHECKPOINT_DATA_NV;

			uint32 retrievedCount = checkpointCount;
			//vkGetQueueCheckpointDataNV(::highlo::VulkanContext::GetVulkanCurrentDevice()->GetComputeQueue(), &retrievedCount, data);
			fpGetQueueCheckpointDataNV(::highlo::VulkanContext::GetVulkanCurrentDevice()->GetComputeQueue(), &retrievedCount, data);
			HL_CORE_ERROR("RetrieveDiagnosticCheckpoints (Compute Queue):");
			for (uint32 i = 0; i < retrievedCount; i++)
			{
				VulkanCheckpointData *checkpoint = (VulkanCheckpointData*)data[i].pCheckpointMarker;
				HL_CORE_ERROR("Checkpoint: {0} (stage: {1})", checkpoint->Data, utils::VulkanStageToString(data[i].stage));
			}
		}
	}


}

#endif // HIGHLO_API_VULKAN

