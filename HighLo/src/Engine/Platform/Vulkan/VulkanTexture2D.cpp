// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanTexture2D.h"

#ifdef HIGHLO_API_VULKAN

#include "Engine/Core/FileSystem.h"
#include "Engine/Utils/ImageUtils.h"
#include "VulkanUtils.h"
#include "VulkanAllocator.h"

#include <stb_image.h>

namespace highlo
{
	static std::map<VkImage, WeakRef<VulkanTexture2D>> s_ImageReferences;

	VulkanTexture2D::VulkanTexture2D(const FileSystemPath &filePath, bool flipOnLoad)
		: m_FilePath(filePath)
	{
		int32 width, height, channels;
		stbi_set_flip_vertically_on_load(flipOnLoad);

		if (!FileSystem::Get()->FileExists(filePath))
		{
			HL_CORE_ERROR("[-] Failed to load Texture2D: {0} [-]", *filePath.String());
			return;
		}

		stbi_uc *data = stbi_load(*filePath.Absolute(), &width, &height, &channels, STBI_rgb_alpha);
		if (!data)
		{
			stbi_image_free(data);
			HL_CORE_ERROR("[-] Failed to load Texture2D: {0} (Error: {1}) [-]", *filePath.String(), stbi_failure_reason());
			return;
		}

		Name = FileSystemPath::ExtractFileNameFromPath(m_FilePath.String());
		m_Loaded = true;
		HL_CORE_INFO("[+] Loaded {0} [+]", *filePath.String());

		if (channels == 4)
		{
			m_Buffer = Allocator::Copy(data, width * height * 4); // 4 byte per pixel
			m_Specification.Format = TextureFormat::RGBA;
		}
		else if (channels == 3)
		{
			m_Buffer = Allocator::Copy(data, width * height * 3); // 3 byte per pixel
			m_Specification.Format = TextureFormat::RG8;
		}

		// Clean up loaded data because it has been copied into our own allocator
		stbi_image_free(data);

		m_Specification.Width = width;
		m_Specification.Height = height;
		m_Specification.Properties = TextureProperties();
		m_Specification.Usage = TextureUsage::Texture;
		m_Specification.Mips = utils::CalculateMipCount(width, height);

		Invalidate();
	}

	VulkanTexture2D::VulkanTexture2D(const glm::vec3 &rgb, TextureFormat format)
	{
		if (format == TextureFormat::RGBA8 || format == TextureFormat::RGBA)
		{
			Byte data[4];
			data[0] = (Byte)rgb.x;
			data[1] = (Byte)rgb.y;
			data[2] = (Byte)rgb.z;
			data[3] = (Byte)255; // 2^8

			Name = "8-Bit Texture";
			m_Buffer = Allocator::Copy(data, 1 * 1 * 4); // 4 byte per pixel
		}
		else if (format == TextureFormat::RGBA16)
		{
			uint16 data[4];
			data[0] = (uint16)rgb.x;
			data[1] = (uint16)rgb.y;
			data[2] = (uint16)rgb.z;
			data[3] = (uint16)65535; // 2^16

			Name = "16-Bit Texture";
			m_Buffer = Allocator::Copy(data, 1 * 1 * 4); // 4 byte per pixel
		}
		else if (format == TextureFormat::RGBA32)
		{
			uint32 data[4];
			data[0] = (uint32)rgb.x;
			data[1] = (uint32)rgb.y;
			data[2] = (uint32)rgb.z;
			data[3] = (uint32)4294967295; // 2^32

			Name = "32-Bit Texture";
			m_Buffer = Allocator::Copy(data, 1 * 1 * 4); // 4 byte per pixel
		}

		m_Specification.Width = 1;
		m_Specification.Height = 1;
		m_Specification.Format = format;
		m_Specification.Properties = TextureProperties();
		m_Specification.Usage = TextureUsage::Texture;
		m_Specification.Mips = utils::CalculateMipCount(1, 1);
		m_Loaded = true;

		Invalidate();
	}
	
	VulkanTexture2D::VulkanTexture2D(const glm::vec3 &rgb, uint32 width, uint32 height, TextureFormat format)
	{
		uint64 row = 0, column = 0, idx = 0;

#pragma warning(push)
#pragma warning(disable : 6386)
		if (format == TextureFormat::RGBA8)
		{
			Byte *data = new Byte[(uint64)width * (uint64)height * (uint64)4];

			for (row = 0; row < (uint64)width * 4; ++row)
			{
				for (column = 0; column < (uint64)height; column += 4)
				{
					idx = row * width + column;
					data[idx] = (Byte)(uint32)rgb.r;
					data[idx + 1] = (Byte)(uint32)rgb.g;
					data[idx + 2] = (Byte)(uint32)rgb.b;
					data[idx + 3] = (Byte)255; // 2^8
				}
			}

			Name = "8-Bit Texture";
			m_Buffer = Allocator::Copy(data, width * height * 4); // 4 byte per pixel
			delete[] data;
		}
		else if (format == TextureFormat::RGBA16)
		{
			uint16 *data = new uint16[(uint64)width * (uint64)height * (uint64)4];

			for (row = 0; row < (uint64)width * 4; ++row)
			{
				for (column = 0; column < (uint64)height; column += 4)
				{
					idx = row * width + column;
					data[idx] = (uint16)(uint32)rgb.r;
					data[idx + 1] = (uint16)(uint32)rgb.g;
					data[idx + 2] = (uint16)(uint32)rgb.b;
					data[idx + 3] = (uint16)65535; // 2^16
				}
			}

			Name = "16-Bit Texture";
			m_Buffer = Allocator::Copy(data, width * height * 4); // 4 byte per pixel
			delete[] data;
		}
		else if (format == TextureFormat::RGBA32)
		{
			uint32 *data = new uint32[(uint64)width * (uint64)height * (uint64)4];

			for (row = 0; row < (uint64)width * 4; ++row)
			{
				for (column = 0; column < (uint64)height; column += 4)
				{
					idx = row * width + column;
					data[idx] = (uint32)rgb.r;
					data[idx + 1] = (uint32)rgb.g;
					data[idx + 2] = (uint32)rgb.b;
					data[idx + 3] = (uint32)4294967295; // 2^32
				}
			}

			Name = "32-Bit Texture";
			m_Buffer = Allocator::Copy(data, width * height * 4); // 4 byte per pixel
			delete[] data;
		}
#pragma warning(pop)

		m_Specification.Width = width;
		m_Specification.Height = height;
		m_Specification.Format = format;
		m_Specification.Properties = TextureProperties();
		m_Specification.Usage = TextureUsage::Texture;
		m_Specification.Mips = utils::CalculateMipCount(width, height);

		m_Loaded = true;

		Invalidate();
	}
	
	VulkanTexture2D::VulkanTexture2D(void *imgData, uint32 width, uint32 height, TextureFormat format)
	{
		m_Buffer = Allocator::Copy(imgData, width * height * 4); // 4 byte per pixel

		m_Specification.Width = width;
		m_Specification.Height = height;
		m_Specification.Format = format;
		m_Specification.Properties = TextureProperties();
		m_Specification.Usage = TextureUsage::Texture;
		m_Specification.Mips = utils::CalculateMipCount(width, height);
		Name = "unknown";
		m_Loaded = true;

		Invalidate();
	}
	
	VulkanTexture2D::VulkanTexture2D(TextureFormat format, uint32 width, uint32 height, const void *data, TextureProperties props)
	{
		m_Buffer = Allocator::Copy(data, width * height * 4);

		m_Specification.Width = width;
		m_Specification.Height = height;
		m_Specification.Format = format;
		m_Specification.Properties = props;
		m_Specification.Usage = TextureUsage::Texture;
		m_Specification.Mips = utils::CalculateMipCount(width, height);
		Name = "unknown";
		m_Loaded = true;

		Invalidate();
	}
	
	VulkanTexture2D::VulkanTexture2D(TextureFormat format, uint32 width, uint32 height)
	{
		m_Buffer.Allocate(width * height * 4); // * 4 channels (RGBA)

		m_Specification.Width = width;
		m_Specification.Height = height;
		m_Specification.Format = format;
		m_Specification.Usage = TextureUsage::Texture;
		m_Specification.Properties = TextureProperties();
		m_Specification.Mips = utils::CalculateMipCount(width, height);
		Name = "unknown";
		m_Loaded = true;

		Invalidate();
	}
	
	VulkanTexture2D::VulkanTexture2D(const TextureSpecification &spec)
		: m_Specification(spec)
	{
		Name = "unknown";
		m_Loaded = true;

		Invalidate();
	}
	
	VulkanTexture2D::~VulkanTexture2D()
	{
		Release();

		if (m_Buffer)
			m_Buffer.Release();
	}
	
	Allocator VulkanTexture2D::GetData()
	{
		HL_ASSERT(m_Locked);
		return m_Buffer;
	}
	
	void VulkanTexture2D::Release()
	{
		ReleaseVulkanData();
	}
	
	void VulkanTexture2D::Invalidate()
	{
		Release();

		auto device = VulkanContext::GetCurrentDevice();
		auto vulkanDevice = device->GetVulkanDevice();

		uint32 mipCount = m_Specification.Properties.GenerateMips ? GetMipLevelCount() : 1;

		if (!m_Buffer)
			m_Specification.Usage = TextureUsage::Storage;

		InvalidateVulkanData();
		auto &info = GetTextureInfo();

		if (m_Buffer)
		{
			VkDeviceSize size = m_Buffer.Size;

			VkMemoryAllocateInfo memAllocInfo{};
			memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

			VulkanAllocator allocator("Texture2D");

			// Create staging buffer
			VkBufferCreateInfo bufferCreateInfo{};
			bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferCreateInfo.size = size;
			bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			VkBuffer stagingBuffer;
			VmaAllocation stagingBufferAllocation = allocator.AllocateBuffer(bufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

			// Copy data to staging buffer
			uint8 *destData = allocator.MapMemory<uint8>(stagingBufferAllocation);
			HL_ASSERT(m_Buffer.Data);
			memcpy(destData, m_Buffer.Data, size);
			allocator.UnmapMemory(stagingBufferAllocation);

			VkCommandBuffer copyCmd = device->GetCommandBuffer(true);

			// Image memory barriers for the texture image

			// The sub resource range describes the regions of the image that will be transitioned using the memory barriers below
			VkImageSubresourceRange subresourceRange = {};
			// Image only contains color data
			subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			// Start at first mip level
			subresourceRange.baseMipLevel = 0;
			subresourceRange.levelCount = 1;
			subresourceRange.layerCount = 1;

			// Transition the texture image layout to transfer target, so we can safely copy our buffer data to it.
			VkImageMemoryBarrier imageMemoryBarrier{};
			imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageMemoryBarrier.image = info.Image;
			imageMemoryBarrier.subresourceRange = subresourceRange;
			imageMemoryBarrier.srcAccessMask = 0;
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

			// Insert a memory dependency at the proper pipeline stages that will execute the image layout transition 
			// Source pipeline stage is host write/read exection (VK_PIPELINE_STAGE_HOST_BIT)
			// Destination pipeline stage is copy command exection (VK_PIPELINE_STAGE_TRANSFER_BIT)
			vkCmdPipelineBarrier(
				copyCmd,
				VK_PIPELINE_STAGE_HOST_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				0,
				0, nullptr,
				0, nullptr,
				1, &imageMemoryBarrier);

			VkBufferImageCopy bufferCopyRegion = {};
			bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			bufferCopyRegion.imageSubresource.mipLevel = 0;
			bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
			bufferCopyRegion.imageSubresource.layerCount = 1;
			bufferCopyRegion.imageExtent.width = m_Specification.Width;
			bufferCopyRegion.imageExtent.height = m_Specification.Height;
			bufferCopyRegion.imageExtent.depth = 1;
			bufferCopyRegion.bufferOffset = 0;

			// Copy mip levels from staging buffer
			vkCmdCopyBufferToImage(
				copyCmd,
				stagingBuffer,
				info.Image,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1,
				&bufferCopyRegion);

			if (mipCount > 1) // Mips to generate
			{
				utils::InsertImageMemoryBarrier(copyCmd, info.Image,
												VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
												VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
												VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
												subresourceRange);
			}
			else
			{
				utils::InsertImageMemoryBarrier(copyCmd, info.Image,
												VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT,
												VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, GetDescriptorInfo().imageLayout,
												VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
												subresourceRange);
			}


			device->FlushCommandBuffer(copyCmd);

			// Clean up staging resources
			allocator.DestroyBuffer(stagingBuffer, stagingBufferAllocation);
		}
		else
		{
			VkCommandBuffer transitionCommandBuffer = device->GetCommandBuffer(true);
			VkImageSubresourceRange subresourceRange = {};
			subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subresourceRange.layerCount = 1;
			subresourceRange.levelCount = GetMipLevelCount();
			utils::SetImageLayout(transitionCommandBuffer, info.Image, VK_IMAGE_LAYOUT_UNDEFINED, GetDescriptorInfo().imageLayout, subresourceRange);
			device->FlushCommandBuffer(transitionCommandBuffer);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// CREATE TEXTURE SAMPLER
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Create a texture sampler
		VkSamplerCreateInfo sampler{};
		sampler.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		sampler.maxAnisotropy = 1.0f;
		sampler.magFilter = utils::VulkanSamplerFilter(m_Specification.Properties.SamplerFilter);
		sampler.minFilter = utils::VulkanSamplerFilter(m_Specification.Properties.SamplerFilter);
		sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sampler.addressModeU = utils::VulkanSamplerWrap(m_Specification.Properties.SamplerWrap);
		sampler.addressModeV = utils::VulkanSamplerWrap(m_Specification.Properties.SamplerWrap);
		sampler.addressModeW = utils::VulkanSamplerWrap(m_Specification.Properties.SamplerWrap);
		sampler.mipLodBias = 0.0f;
		sampler.compareOp = VK_COMPARE_OP_NEVER;
		sampler.minLod = 0.0f;
		sampler.maxLod = (float)mipCount;

		sampler.anisotropyEnable = VK_FALSE;
		sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

		VK_CHECK_RESULT(vkCreateSampler(vulkanDevice, &sampler, nullptr, &info.Sampler));

		if (!m_Specification.Properties.Storage)
		{
			VkImageViewCreateInfo view{};
			view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			view.viewType = VK_IMAGE_VIEW_TYPE_2D;
			view.format = utils::VulkanTextureFormat(m_Specification.Format);
			view.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
			// The subresource range describes the set of mip levels (and array layers) that can be accessed through this image view
			// It's possible to create multiple image views for a single image referring to different (and/or overlapping) ranges of the image
			view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			view.subresourceRange.baseMipLevel = 0;
			view.subresourceRange.baseArrayLayer = 0;
			view.subresourceRange.layerCount = 1;
			view.subresourceRange.levelCount = mipCount;
			view.image = info.Image;
			VK_CHECK_RESULT(vkCreateImageView(vulkanDevice, &view, nullptr, &info.ImageView));

			utils::SetDebugUtilsObjectName(vulkanDevice, VK_OBJECT_TYPE_IMAGE_VIEW, fmt::format("Texture view: {}", m_Specification.DebugName), info.ImageView);
			UpdateDescriptor();
		}

		if (m_Buffer && m_Specification.Properties.GenerateMips && mipCount > 1)
			GenerateMips();
	}
	
	void VulkanTexture2D::Resize(const glm::uvec2 &size)
	{
		Resize(size.x, size.y);
	}
	
	void VulkanTexture2D::Resize(const uint32 width, const uint32 height)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}
	
	void VulkanTexture2D::Lock()
	{
		HL_ASSERT(!m_Locked);
		m_Locked = true;
	}
	
	void VulkanTexture2D::Unlock()
	{
		HL_ASSERT(m_Locked);
		m_Locked = false;
		Invalidate();
	}
	
	void VulkanTexture2D::CreatePerLayerImageViews()
	{
		HL_ASSERT(m_Specification.Layers > 1);

		VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();

		VkImageAspectFlags aspectMask = utils::IsDepthFormat(m_Specification.Format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
		if (m_Specification.Format == TextureFormat::DEPTH24STENCIL8)
			aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

		const VkFormat vulkanFormat = utils::VulkanTextureFormat(m_Specification.Format);

		m_PerLayerImageViews.resize(m_Specification.Layers);
		for (uint32 layer = 0; layer < m_Specification.Layers; layer++)
		{
			VkImageViewCreateInfo imageViewCreateInfo = {};
			imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewCreateInfo.format = vulkanFormat;
			imageViewCreateInfo.flags = 0;
			imageViewCreateInfo.subresourceRange = {};
			imageViewCreateInfo.subresourceRange.aspectMask = aspectMask;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = m_Specification.Mips;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = layer;
			imageViewCreateInfo.subresourceRange.layerCount = 1;
			imageViewCreateInfo.image = m_Info.Image;
			VK_CHECK_RESULT(vkCreateImageView(device, &imageViewCreateInfo, nullptr, &m_PerLayerImageViews[layer]));
			utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_IMAGE_VIEW, fmt::format("{} image view layer: {}", m_Specification.DebugName, layer), m_PerLayerImageViews[layer]);
		}
	}
	
	void VulkanTexture2D::CreateSampler(TextureProperties properties)
	{
		m_Specification.Properties = properties;

		VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();
		VkSamplerCreateInfo samplerCreateInfo = {};
		samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCreateInfo.maxAnisotropy = 1.0f;

		if (utils::IsIntegerBased(m_Specification.Format))
		{
			samplerCreateInfo.magFilter = VK_FILTER_NEAREST;
			samplerCreateInfo.minFilter = VK_FILTER_NEAREST;
			samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
		}
		else
		{
			samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
			samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
			samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		}

		samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerCreateInfo.addressModeV = samplerCreateInfo.addressModeU;
		samplerCreateInfo.addressModeW = samplerCreateInfo.addressModeU;
		samplerCreateInfo.mipLodBias = 0.0f;
		samplerCreateInfo.minLod = 0.0f;
		samplerCreateInfo.maxLod = 100.0f;
		samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		VK_CHECK_RESULT(vkCreateSampler(device, &samplerCreateInfo, nullptr, &m_Info.Sampler));
		utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_SAMPLER, fmt::format("{} default sampler", m_Specification.DebugName), m_Info.Sampler);

	}
	
	void VulkanTexture2D::CreatePerSpecificLayerImageViews(const std::vector<uint32> &layerIndices)
	{
		HL_ASSERT(m_Specification.Layers > 1);

		VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();

		VkImageAspectFlags aspectMask = utils::IsDepthFormat(m_Specification.Format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
		if (m_Specification.Format == TextureFormat::DEPTH24STENCIL8)
			aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

		const VkFormat vulkanFormat = utils::VulkanTextureFormat(m_Specification.Format);

		if (m_PerLayerImageViews.empty())
			m_PerLayerImageViews.resize(m_Specification.Layers);

		for (uint32 layer : layerIndices)
		{
			VkImageViewCreateInfo imageViewCreateInfo = {};
			imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewCreateInfo.format = vulkanFormat;
			imageViewCreateInfo.flags = 0;
			imageViewCreateInfo.subresourceRange = {};
			imageViewCreateInfo.subresourceRange.aspectMask = aspectMask;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = m_Specification.Mips;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = layer;
			imageViewCreateInfo.subresourceRange.layerCount = 1;
			imageViewCreateInfo.image = m_Info.Image;
			VK_CHECK_RESULT(vkCreateImageView(device, &imageViewCreateInfo, nullptr, &m_PerLayerImageViews[layer]));
			utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_IMAGE_VIEW, fmt::format("{} image view layer: {}", m_Specification.DebugName, layer), m_PerLayerImageViews[layer]);
		}
	}
	
	void VulkanTexture2D::UpdateResourceData()
	{
		if (!m_Buffer)
			return;

		UpdateResourceData(m_Buffer.Data);
	}
	
	void VulkanTexture2D::UpdateResourceData(void *data)
	{
		if (!data)
			return;

		// TODO
	}
	
	void VulkanTexture2D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
	{
		// TODO
	}
	
	glm::ivec4 VulkanTexture2D::ReadPixel(uint32 row, uint32 column)
	{
		// TODO
		return glm::ivec4();
	}
	
	uint32 VulkanTexture2D::GetMipLevelCount()
	{
		return utils::CalculateMipCount(m_Specification.Width, m_Specification.Height);
	}
	
	std::pair<uint32, uint32> VulkanTexture2D::GetMipSize(uint32 mip)
	{
		return utils::GetMipSize(mip, m_Specification.Width, m_Specification.Height);
	}
	
	void VulkanTexture2D::GenerateMips(bool readonly)
	{
		auto device = VulkanContext::GetCurrentDevice();
		auto vulkanDevice = device->GetVulkanDevice();
		const auto &info = GetTextureInfo();
		const VkCommandBuffer blitCmd = VulkanContext::GetCurrentDevice()->GetCommandBuffer(true);

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = info.Image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		const auto mipLevels = GetMipLevelCount();
		for (uint32 i = 1; i < mipLevels; i++)
		{
			VkImageBlit imageBlit{};

			// Source
			imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBlit.srcSubresource.layerCount = 1;
			imageBlit.srcSubresource.mipLevel = i - 1;
			imageBlit.srcOffsets[1].x = int32(m_Specification.Width >> (i - 1));
			imageBlit.srcOffsets[1].y = int32(m_Specification.Height >> (i - 1));
			imageBlit.srcOffsets[1].z = 1;

			// Destination
			imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBlit.dstSubresource.layerCount = 1;
			imageBlit.dstSubresource.mipLevel = i;
			imageBlit.dstOffsets[1].x = int32(m_Specification.Width >> i);
			imageBlit.dstOffsets[1].y = int32(m_Specification.Height >> i);
			imageBlit.dstOffsets[1].z = 1;

			VkImageSubresourceRange mipSubRange = {};
			mipSubRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			mipSubRange.baseMipLevel = i;
			mipSubRange.levelCount = 1;
			mipSubRange.layerCount = 1;

			// Prepare current mip level as image blit destination
			utils::InsertImageMemoryBarrier(blitCmd, info.Image,
											0, VK_ACCESS_TRANSFER_WRITE_BIT,
											VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
											VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
											mipSubRange);

			// Blit from previous level
			vkCmdBlitImage(
				blitCmd,
				info.Image,
				VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				info.Image,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1,
				&imageBlit,
				utils::VulkanSamplerFilter(m_Specification.Properties.SamplerFilter));

			// Prepare current mip level as image blit source for next level
			utils::InsertImageMemoryBarrier(blitCmd, info.Image,
											VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
											VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
											VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
											mipSubRange);
		}

		// After the loop, all mip layers are in TRANSFER_SRC layout, so transition all to SHADER_READ
		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.layerCount = 1;
		subresourceRange.levelCount = mipLevels;

		utils::InsertImageMemoryBarrier(blitCmd, info.Image,
										VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT,
										VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
										VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
										subresourceRange);

		VulkanContext::GetCurrentDevice()->FlushCommandBuffer(blitCmd);
	}
	
	void VulkanTexture2D::SetData(void *data, uint32 data_size)
	{
		if (m_Buffer)
			m_Buffer.Release();

		m_Buffer = Allocator::Copy(data, data_size);
		Invalidate();
	}
	
	void VulkanTexture2D::CopyToHostBuffer(Allocator &buffer)
	{
		auto device = VulkanContext::GetCurrentDevice();
		auto vulkanDevice = device->GetVulkanDevice();
		VulkanAllocator allocator("Texture2D");

		uint64 bufferSize = m_Specification.Width * m_Specification.Height * utils::GetImageFormatBPP(m_Specification.Format);

		// Create staging buffer
		VkBufferCreateInfo bufferCreateInfo{};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = bufferSize;
		bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkBuffer stagingBuffer;
		VmaAllocation stagingBufferAllocation = allocator.AllocateBuffer(bufferCreateInfo, VMA_MEMORY_USAGE_GPU_TO_CPU, stagingBuffer);

		uint32 mipCount = 1;
		uint32 mipWidth = m_Specification.Width, mipHeight = m_Specification.Height;

		VkCommandBuffer copyCmd = device->GetCommandBuffer(true);

		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = mipCount;
		subresourceRange.layerCount = 1;

		utils::InsertImageMemoryBarrier(copyCmd, m_Info.Image,
										VK_ACCESS_TRANSFER_READ_BIT, 0,
										m_DescriptorImageInfo.imageLayout, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
										VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
										subresourceRange);

		uint64 mipDataOffset = 0;
		for (uint32 mip = 0; mip < mipCount; mip++)
		{
			VkBufferImageCopy bufferCopyRegion = {};
			bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			bufferCopyRegion.imageSubresource.mipLevel = mip;
			bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
			bufferCopyRegion.imageSubresource.layerCount = 1;
			bufferCopyRegion.imageExtent.width = mipWidth;
			bufferCopyRegion.imageExtent.height = mipHeight;
			bufferCopyRegion.imageExtent.depth = 1;
			bufferCopyRegion.bufferOffset = mipDataOffset;

			vkCmdCopyImageToBuffer(
				copyCmd,
				m_Info.Image,
				VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				stagingBuffer,
				1,
				&bufferCopyRegion);

			uint64 mipDataSize = mipWidth * mipHeight * sizeof(float) * 4 * 6;
			mipDataOffset += mipDataSize;
			mipWidth /= 2;
			mipHeight /= 2;
		}

		utils::InsertImageMemoryBarrier(copyCmd, m_Info.Image,
										VK_ACCESS_TRANSFER_READ_BIT, 0,
										VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_DescriptorImageInfo.imageLayout,
										VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
										subresourceRange);

		device->FlushCommandBuffer(copyCmd);

		// Copy data from staging buffer
		uint8 *srcData = allocator.MapMemory<uint8>(stagingBufferAllocation);
		buffer.Allocate((uint32)bufferSize);
		memcpy(buffer.Data, srcData, bufferSize);
		allocator.UnmapMemory(stagingBufferAllocation);

		allocator.DestroyBuffer(stagingBuffer, stagingBufferAllocation);
	}
	
	void VulkanTexture2D::UpdateDescriptor()
	{
		if (m_Specification.Format == TextureFormat::DEPTH24STENCIL8 || m_Specification.Format == TextureFormat::DEPTH32F || m_Specification.Format == TextureFormat::DEPTH32FSTENCIL8UINT)
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
		else if (m_Specification.Usage == TextureUsage::Storage)
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
		else
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		if (m_Specification.Usage == TextureUsage::Storage)
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
		else if (m_Specification.Usage == TextureUsage::HostRead)
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

		m_DescriptorImageInfo.imageView = m_Info.ImageView;
		m_DescriptorImageInfo.sampler = m_Info.Sampler;
	}
	
	VkImageView VulkanTexture2D::GetMipImageView(uint32 mip)
	{
		if (m_PerMipImageViews.find(mip) == m_PerMipImageViews.end())
		{
			VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();

			VkImageAspectFlags aspectMask = utils::IsDepthFormat(m_Specification.Format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
			if (m_Specification.Format == TextureFormat::DEPTH24STENCIL8)
				aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

			const VkFormat vulkanFormat = utils::VulkanTextureFormat(m_Specification.Format);

			VkImageViewCreateInfo imageViewCreateInfo = {};
			imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewCreateInfo.format = vulkanFormat;
			imageViewCreateInfo.flags = 0;
			imageViewCreateInfo.subresourceRange = {};
			imageViewCreateInfo.subresourceRange.aspectMask = aspectMask;
			imageViewCreateInfo.subresourceRange.baseMipLevel = mip;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;
			imageViewCreateInfo.image = m_Info.Image;

			VK_CHECK_RESULT(vkCreateImageView(device, &imageViewCreateInfo, nullptr, &m_PerMipImageViews[mip]));
			utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_IMAGE_VIEW, fmt::format("{} image view mip: {}", m_Specification.DebugName, mip), m_PerMipImageViews[mip]);
		}

		return m_PerMipImageViews.at(mip);
	}
	
	const std::map<VkImage, WeakRef<VulkanTexture2D>> &VulkanTexture2D::GetImageRefs()
	{
		return s_ImageReferences;
	}

	void VulkanTexture2D::InvalidateVulkanData()
	{
		HL_ASSERT(m_Specification.Width > 0 && m_Specification.Height > 0);

		// Try release first if necessary
		ReleaseVulkanData();

		VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();
		VulkanAllocator allocator("Texture2DData");

		VkImageUsageFlags usage = VK_IMAGE_USAGE_SAMPLED_BIT; // TODO: this (probably) shouldn't be implied
		if (m_Specification.Usage == TextureUsage::Attachment)
		{
			if (utils::IsDepthFormat(m_Specification.Format))
				usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			else
				usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		}

		if (m_Specification.Transfer || m_Specification.Usage == TextureUsage::Texture)
		{
			usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		}
		
		if (m_Specification.Usage == TextureUsage::Storage)
		{
			usage |= VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		}

		VkImageAspectFlags aspectMask = utils::IsDepthFormat(m_Specification.Format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
		if (m_Specification.Format == TextureFormat::DEPTH24STENCIL8)
			aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

		VkFormat vulkanFormat = utils::VulkanTextureFormat(m_Specification.Format);

		VmaMemoryUsage memoryUsage = m_Specification.Usage == TextureUsage::HostRead ? VMA_MEMORY_USAGE_GPU_TO_CPU : VMA_MEMORY_USAGE_GPU_ONLY;

		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.format = vulkanFormat;
		imageCreateInfo.extent.width = m_Specification.Width;
		imageCreateInfo.extent.height = m_Specification.Height;
		imageCreateInfo.extent.depth = 1;
		imageCreateInfo.mipLevels = m_Specification.Mips;
		imageCreateInfo.arrayLayers = m_Specification.Layers;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = m_Specification.Usage == TextureUsage::HostRead ? VK_IMAGE_TILING_LINEAR : VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.usage = usage;
		m_Info.MemoryAlloc = allocator.AllocateImage(imageCreateInfo, memoryUsage, m_Info.Image, &m_GPUAllocationSize);
		s_ImageReferences[m_Info.Image] = this;
		utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_IMAGE, m_Specification.DebugName, m_Info.Image);

		// Create a default image view
		VkImageViewCreateInfo imageViewCreateInfo = {};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.viewType = m_Specification.Layers > 1 ? VK_IMAGE_VIEW_TYPE_2D_ARRAY : VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = vulkanFormat;
		imageViewCreateInfo.flags = 0;
		imageViewCreateInfo.subresourceRange = {};
		imageViewCreateInfo.subresourceRange.aspectMask = aspectMask;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = m_Specification.Mips;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = m_Specification.Layers;
		imageViewCreateInfo.image = m_Info.Image;
		VK_CHECK_RESULT(vkCreateImageView(device, &imageViewCreateInfo, nullptr, &m_Info.ImageView));
		utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_IMAGE_VIEW, fmt::format("{} default image view", m_Specification.DebugName), m_Info.ImageView);

		// TODO: Renderer should contain some kind of sampler cache
		VkSamplerCreateInfo samplerCreateInfo = {};
		samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCreateInfo.maxAnisotropy = 1.0f;
		if (utils::IsIntegerBased(m_Specification.Format))
		{
			samplerCreateInfo.magFilter = VK_FILTER_NEAREST;
			samplerCreateInfo.minFilter = VK_FILTER_NEAREST;
			samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
		}
		else
		{
			samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
			samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
			samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		}

		samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerCreateInfo.addressModeV = samplerCreateInfo.addressModeU;
		samplerCreateInfo.addressModeW = samplerCreateInfo.addressModeU;
		samplerCreateInfo.mipLodBias = 0.0f;
		samplerCreateInfo.maxAnisotropy = 1.0f;
		samplerCreateInfo.minLod = 0.0f;
		samplerCreateInfo.maxLod = 100.0f;
		samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		VK_CHECK_RESULT(vkCreateSampler(device, &samplerCreateInfo, nullptr, &m_Info.Sampler));
		utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_SAMPLER, fmt::format("{} default sampler", m_Specification.DebugName), m_Info.Sampler);

		if (m_Specification.Usage == TextureUsage::Storage)
		{
			// Transition image to GENERAL layout
			VkCommandBuffer commandBuffer = VulkanContext::GetCurrentDevice()->GetCommandBuffer(true);

			VkImageSubresourceRange subresourceRange = {};
			subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subresourceRange.baseMipLevel = 0;
			subresourceRange.levelCount = m_Specification.Mips;
			subresourceRange.layerCount = m_Specification.Layers;

			utils::InsertImageMemoryBarrier(commandBuffer, m_Info.Image,
											0, 0,
											VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL,
											VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
											subresourceRange);

			VulkanContext::GetCurrentDevice()->FlushCommandBuffer(commandBuffer);
		}
		else if (m_Specification.Usage == TextureUsage::HostRead)
		{
			// Transition image to TRANSFER_DST layout
			VkCommandBuffer commandBuffer = VulkanContext::GetCurrentDevice()->GetCommandBuffer(true);

			VkImageSubresourceRange subresourceRange = {};
			subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subresourceRange.baseMipLevel = 0;
			subresourceRange.levelCount = m_Specification.Mips;
			subresourceRange.layerCount = m_Specification.Layers;

			utils::InsertImageMemoryBarrier(commandBuffer, m_Info.Image,
											0, 0,
											VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
											VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
											subresourceRange);

			VulkanContext::GetCurrentDevice()->FlushCommandBuffer(commandBuffer);
		}

		UpdateDescriptor();
	}
	
	void VulkanTexture2D::ReleaseVulkanData()
	{
		if (m_Info.Image == nullptr)
			return;

		const VulkanTextureInfo &info = m_Info;
		Renderer::SubmitWithoutResources([info, mipViews = m_PerMipImageViews, layerViews = m_PerLayerImageViews]() mutable
		{
			const auto vulkanDevice = VulkanContext::GetCurrentDevice()->GetVulkanDevice();
			vkDestroyImageView(vulkanDevice, info.ImageView, nullptr);
			vkDestroySampler(vulkanDevice, info.Sampler, nullptr);

			for (auto &view : mipViews)
			{
				if (view.second)
					vkDestroyImageView(vulkanDevice, view.second, nullptr);
			}
			
			for (auto &view : layerViews)
			{
				if (view)
					vkDestroyImageView(vulkanDevice, view, nullptr);
			}

			VulkanAllocator allocator("VulkanImage2D");
			allocator.DestroyImage(info.Image, info.MemoryAlloc);
			s_ImageReferences.erase(info.Image);
		});

		m_Info.Image = nullptr;
		m_Info.ImageView = nullptr;
		m_Info.Sampler = nullptr;
		m_PerLayerImageViews.clear();
		m_PerMipImageViews.clear();
	}
}

#endif // HIGHLO_API_VULKAN

