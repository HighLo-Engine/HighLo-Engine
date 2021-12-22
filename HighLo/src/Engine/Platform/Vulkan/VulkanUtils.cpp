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
			case TextureFormat::RED32F:				return VK_FORMAT_R32_SFLOAT;
			case TextureFormat::RG16F:				return VK_FORMAT_R16G16_SFLOAT;
			case TextureFormat::RG32F:				return VK_FORMAT_R32G32_SFLOAT;
			case TextureFormat::RGBA:				return VK_FORMAT_R8G8B8A8_UNORM;
			case TextureFormat::RGBA16F:			return VK_FORMAT_R16G16B16A16_SFLOAT;
			case TextureFormat::RGBA32F:			return VK_FORMAT_R32G32B32A32_SFLOAT;
			case TextureFormat::DEPTH32F:			return VK_FORMAT_D32_SFLOAT;
			case TextureFormat::DEPTH24STENCIL8:	return VulkanContext::GetCurrentDevice()->GetPhysicalDevice()->GetDepthFormat();
		}

		HL_ASSERT(false);
		return VK_FORMAT_UNDEFINED;
	}
}

#endif // HIGHLO_API_VULKAN