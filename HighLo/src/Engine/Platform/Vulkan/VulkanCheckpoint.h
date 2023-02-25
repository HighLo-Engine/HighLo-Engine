// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-04-22) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include <vulkan/vulkan.h>

namespace highlo
{
	struct VulkanCheckpointData
	{
		char Data[64];
	};

	void SetVulkanCheckpoint(VkCommandBuffer commandBuffer, const HLString &data);
}

#endif // HIGHLO_API_VULKAN

