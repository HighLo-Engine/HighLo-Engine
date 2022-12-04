// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanCheckpoint.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanContext.h"

namespace highlo
{
	static std::vector<VulkanCheckpointData> s_CheckpointStorage(1024);
	static uint32 s_CheckpointStorageIndex = 0;

	void SetVulkanCheckpoint(VkCommandBuffer commandBuffer, const HLString &data)
	{
		const bool supported = VulkanContext::GetVulkanCurrentDevice()->GetVulkanPhysicalDevice()->IsExtensionSupported(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME);
		if (!supported)
			return;

		s_CheckpointStorageIndex = (s_CheckpointStorageIndex + 1) % 1024;
		VulkanCheckpointData &checkpoint = s_CheckpointStorage[s_CheckpointStorageIndex];
		memset(checkpoint.Data, 0, sizeof(checkpoint.Data));
		strcpy_s(checkpoint.Data, *data);

		fpCmdSetCheckpointNV(commandBuffer, &checkpoint);
		//vkCmdSetCheckpointNV(commandBuffer, &checkpoint);
	}
}

#endif // HIGHLO_API_VULKAN

