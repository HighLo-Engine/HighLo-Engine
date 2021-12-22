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
	struct VulkanCheckpointData
	{
		char Data[64];
	};

	VkFormat VulkanImageFormat(TextureFormat format);
	VkSamplerAddressMode VulkanSamplerWrap(TextureWrap wrap);
	VkFilter VulkanSamplerFilter(TextureFilter filter);

	void InsertTextureMemoryBarrier(VkCommandBuffer commandBuffer,
									VkImage image, 
									VkAccessFlags srcAccessMask, 
									VkAccessFlags dstAccessMask, 
									VkImageLayout oldImageLayout, 
									VkImageLayout imageLayout, 
									VkPipelineStageFlags srcStageMask, 
									VkPipelineStageFlags dstStageMask, 
									VkImageSubresourceRange subResourceRange);

	void SetTextureLayout(VkCommandBuffer commandBuffer, 
						  VkImage image, 
						  VkImageLayout oldImageLayout, 
						  VkImageLayout imageLayout, 
						  VkImageSubresourceRange subresourceRange, 
						  VkPipelineStageFlagBits srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 
						  VkPipelineStageFlagBits dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);

	void SetTextureLayout(VkCommandBuffer commandBuffer, 
						  VkImage image, 
						  VkImageAspectFlags aspectMask, 
						  VkImageLayout oldImageLayout, 
						  VkImageLayout imageLayout, 
						  VkPipelineStageFlagBits srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 
						  VkPipelineStageFlagBits dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);

	void SetVulkanCheckpoint(VkCommandBuffer commandBuffer, const HLString &data);
}

#endif // HIGHLO_API_VULKAN