// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanUtils.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanContext.h"

namespace highlo::utils
{
	VkFormat VulkanImageFormat(TextureFormat format)
	{
		switch (format)
		{
			case TextureFormat::RED8UN:					return VK_FORMAT_R8_UNORM;
			case TextureFormat::RED8UI:					return VK_FORMAT_R8_UINT;
			case TextureFormat::RED16UI:				return VK_FORMAT_R16_UINT;
			case TextureFormat::RED32UI:				return VK_FORMAT_R32_UINT;
			case TextureFormat::RED32F:					return VK_FORMAT_R32_SFLOAT;
			case TextureFormat::RG8:				    return VK_FORMAT_R8G8_UNORM;
			case TextureFormat::RG16F:					return VK_FORMAT_R16G16_SFLOAT;
			case TextureFormat::RG32F:					return VK_FORMAT_R32G32_SFLOAT;
			case TextureFormat::RGBA:					return VK_FORMAT_R8G8B8A8_UNORM;
			case TextureFormat::RGBA16F:				return VK_FORMAT_R16G16B16A16_SFLOAT;
			case TextureFormat::RGBA32F:				return VK_FORMAT_R32G32B32A32_SFLOAT;
			case TextureFormat::B10R11G11UF:			return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
			case TextureFormat::DEPTH32FSTENCIL8UINT:	return VK_FORMAT_D32_SFLOAT_S8_UINT;
			case TextureFormat::DEPTH32F:				return VK_FORMAT_D32_SFLOAT;
			case TextureFormat::DEPTH24STENCIL8:		return VulkanContext::GetVulkanCurrentDevice()->GetVulkanPhysicalDevice()->GetDepthFormat();
		}

		HL_ASSERT(false);
		return VK_FORMAT_UNDEFINED;
	}
}

#endif // HIGHLO_API_VULKAN

