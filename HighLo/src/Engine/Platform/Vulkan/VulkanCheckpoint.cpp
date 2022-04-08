#include "HighLoPch.h"
#include "VulkanCheckpoint.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanContext.h"
#include "VulkanUtils.h"

namespace highlo
{
	static std::vector<VulkanCheckpointData> s_CheckpointStorage(1024);
	static uint32 s_CeckpointStorageIndex = 0;

	void SetVulkanCheckpoint(VkCommandBuffer commandBuffer, const HLString &data)
	{
		const bool supported = VulkanContext::GetCurrentDevice()->GetPhysicalDevice()->IsExtensionSupported(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME);
		if (!supported)
			return;

		s_CeckpointStorageIndex = (s_CeckpointStorageIndex + 1) % 1024;
		VulkanCheckpointData &checkpoint = s_CheckpointStorage[s_CeckpointStorageIndex];
		memset(checkpoint.Data, 0, sizeof(checkpoint.Data));
		strcpy_s(checkpoint.Data, *data);
		
		fpCmdSetCheckpointNV(commandBuffer, &checkpoint);
		//vkCmdSetCheckpointNV(commandBuffer, &checkpoint);
	}
}

#endif // HIGHLO_API_VULKAN

