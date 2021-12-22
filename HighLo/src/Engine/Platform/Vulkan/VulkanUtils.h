// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-23) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include <vulkan/vulkan.h>

#include "Engine/Renderer/TextureFormat.h"

namespace highlo::utils
{
	VkFormat VulkanImageFormat(TextureFormat format);
}

#endif // HIGHLO_API_VULKAN