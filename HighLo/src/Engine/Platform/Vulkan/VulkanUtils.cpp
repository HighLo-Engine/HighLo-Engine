#include "HighLoPch.h"
#include "VulkanUtils.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanContext.h"

namespace highlo::utils
{
	static std::vector<VulkanCheckpointData> s_CheckpointStorage(1024);
	static uint32 s_CheckpointStorageIndex = 0;

	VkFormat VulkanImageFormat(TextureFormat format)
	{
		switch (format)
		{
			case TextureFormat::RED32F:				return VK_FORMAT_R32_SFLOAT;
			case TextureFormat::RG16F:				return VK_FORMAT_R16G16_SFLOAT;
			case TextureFormat::RG32F:				return VK_FORMAT_R32G32_SFLOAT;
			case TextureFormat::RGBA:				return VK_FORMAT_R8G8B8A8_UNORM;
			case TextureFormat::RGBA8:				return VK_FORMAT_R8G8B8A8_UNORM;
			case TextureFormat::RGBA16F:			return VK_FORMAT_R16G16B16A16_SFLOAT;
			case TextureFormat::RGBA32F:			return VK_FORMAT_R32G32B32A32_SFLOAT;
			case TextureFormat::DEPTH32F:			return VK_FORMAT_D32_SFLOAT;
			case TextureFormat::DEPTH24STENCIL8:	return VulkanContext::GetCurrentDevice()->GetPhysicalDevice()->GetDepthFormat();
		}

		HL_ASSERT(false);
		return VK_FORMAT_UNDEFINED;
	}

	VkSamplerAddressMode VulkanSamplerWrap(TextureWrap wrap)
	{
		switch (wrap)
		{
			case TextureWrap::Clamp:        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			case TextureWrap::Repeat:       return VK_SAMPLER_ADDRESS_MODE_REPEAT;
		}

		HL_ASSERT(false);
		return (VkSamplerAddressMode)0;
	}
	
	VkFilter VulkanSamplerFilter(TextureFilter filter)
	{
		switch (filter)
		{
			case TextureFilter::Linear:     return VK_FILTER_LINEAR;
			case TextureFilter::Nearest:    return VK_FILTER_NEAREST;
		}

		HL_ASSERT(false);
		return (VkFilter)0;
	}

	void InsertTextureMemoryBarrier(VkCommandBuffer commandBuffer, VkImage image, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkImageLayout oldImageLayout, VkImageLayout imageLayout, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkImageSubresourceRange subResourceRange)
	{
		VkImageMemoryBarrier imageMemoryBarrier{};
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		imageMemoryBarrier.srcAccessMask = srcAccessMask;
		imageMemoryBarrier.dstAccessMask = dstAccessMask;
		imageMemoryBarrier.oldLayout = oldImageLayout;
		imageMemoryBarrier.newLayout = imageLayout;
		imageMemoryBarrier.image = image;
		imageMemoryBarrier.subresourceRange = subResourceRange;

		vkCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
	}

	void SetTextureLayout(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout oldImageLayout, VkImageLayout imageLayout, VkImageSubresourceRange subresourceRange, VkPipelineStageFlagBits srcStageMask, VkPipelineStageFlagBits dstStageMask)
	{
		// First Create an image barrier
		VkImageMemoryBarrier imageMemoryBarrier = {};
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		imageMemoryBarrier.oldLayout = oldImageLayout;
		imageMemoryBarrier.newLayout = imageLayout;
		imageMemoryBarrier.image = image;
		imageMemoryBarrier.subresourceRange = subresourceRange;

		// Create source layouts
		// These flags are used to make sure that any writes or reads of any types have been finished their writing or reading processs,
		// before the oldImageLayout gets transitioned into the new imageLayout
		switch (oldImageLayout)
		{
			/// Image layout is not defined, only valid for initial layouts
			case VK_IMAGE_LAYOUT_UNDEFINED:
				imageMemoryBarrier.srcAccessMask = 0;
				break;

			/// Image was preinitialized and is only valid as initial layouts for linear images, preserves the memory content
			case VK_IMAGE_LAYOUT_PREINITIALIZED:
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
				break;

			/// Image is color attachment
			case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				break;

			/// Image is depth/stencil attachment
			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				break;

			/// Image is a transfer source
			case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				break;

			/// Image is a transfer destination
			case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				break;

			/// Image is read by a shader (either as a sampler or a input attachment)
			case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
				break;

			default:
				break;
		}

		// Create destination layouts
		// The destination access mask controls the dependencies for new image layouts
		switch (imageLayout)
		{
			/// Image will be used as a transfer destination
			case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				break;

			/// Image will be used as a transfer source
			case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				break;

			/// Image will be used as a color attachment
			case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				break;

			/// Image will be used as a depth/stencil attachment
			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
				imageMemoryBarrier.dstAccessMask = imageMemoryBarrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				break;

			/// Image will be read into a shader (either as a sampler or as a input attachment)
			case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
				if (imageMemoryBarrier.srcAccessMask == 0)
					imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;

				imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				break;

			default:
				break;
		}

		vkCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
	}

	void SetTextureLayout(VkCommandBuffer commandBuffer, VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout imageLayout, VkPipelineStageFlagBits srcStageMask, VkPipelineStageFlagBits dstStageMask)
	{
		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = aspectMask;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.layerCount = 1;
		SetTextureLayout(commandBuffer, image, oldImageLayout, imageLayout, subresourceRange, srcStageMask, dstStageMask);
	}

	void SetVulkanCheckpoint(VkCommandBuffer commandBuffer, const HLString &data)
	{
		const bool supported = VulkanContext::GetCurrentDevice()->GetPhysicalDevice()->IsExtensionSupported(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME);
		if (!supported)
			return;

		s_CheckpointStorageIndex = (s_CheckpointStorageIndex + 1) % 1024;
		VulkanCheckpointData &checkpoint = s_CheckpointStorage[s_CheckpointStorageIndex];
		memset(checkpoint.Data, 0, sizeof(checkpoint.Data));
		strcpy_s(checkpoint.Data, *data);
		vkCmdSetCheckpointNV(commandBuffer, &checkpoint);
	}


}

#endif // HIGHLO_API_VULKAN