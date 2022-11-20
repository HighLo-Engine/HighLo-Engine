// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanCheckpoint.h"

namespace highlo
{
	static std::vector<VulkanCheckpointData> s_CheckpointStorage(1024);
	static uint32 s_CheckpointStorageIndex = 0;

	void SetVulkanCheckpoint(VkCommandBuffer commandBuffer, const HLString &data)
	{

	}
}

