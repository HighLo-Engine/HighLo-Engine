#include "HighLoPch.h"
#include "VulkanUtils.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanContext.h"
#include "VulkanCheckpoint.h"

namespace highlo::utils
{
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
			vkGetQueueCheckpointDataNV(::highlo::VulkanContext::GetCurrentDevice()->GetGraphicsQueue(), &retrievedCount, data);
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
			vkGetQueueCheckpointDataNV(::highlo::VulkanContext::GetCurrentDevice()->GetComputeQueue(), &retrievedCount, data);
			HL_CORE_ERROR("RetrieveDiagnosticCheckpoints (Compute Queue):");
			for (uint32 i = 0; i < retrievedCount; ++i)
			{
				VulkanCheckpointData *checkpoint = (VulkanCheckpointData *)data[i].pCheckpointMarker;
				HL_CORE_ERROR("Checkpoint: {0} (stage: {1})", checkpoint->Data, VulkanStageToString(data[i].stage));
			}
		}
	}
}

#endif // HIGHLO_API_VULKAN

