// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-11-19) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Vulkan.h"
#include "Engine/Graphics/TextureFormat.h"

namespace highlo::utils
{
	inline static void SetDebugUtilsObjectName(const VkDevice device, const VkObjectType objectType, const HLString &name, const void *handle)
	{
		VkDebugUtilsObjectNameInfoEXT nameInfo;
		nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
		nameInfo.objectType = objectType;
		nameInfo.pObjectName = name.C_Str();
		nameInfo.objectHandle = (uint64)handle;
		nameInfo.pNext = VK_NULL_HANDLE;

		VK_CHECK_RESULT(fpSetDebugUtilsObjectNameEXT(device, &nameInfo));
	}

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

	VkFormat VulkanImageFormat(TextureFormat format);
}

#endif // HIGHLO_API_VULKAN

