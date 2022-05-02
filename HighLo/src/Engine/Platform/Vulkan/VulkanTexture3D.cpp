// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanTexture3D.h"

#ifdef HIGHLO_API_VULKAN

#include <stb_image.h>

#include "VulkanUtils.h"

#include "Engine/Utils/ImageUtils.h"

#define TEXTURE3D_LOG_PREFIX "Texture3D>    "

#define FACE_COUNT 6

namespace highlo
{
    VulkanTexture3D::VulkanTexture3D(const FileSystemPath &filePath, bool flipOnLoad)
        : m_FilePath(filePath)
    {
        stbi_set_flip_vertically_on_load(flipOnLoad);

        int32 width, height, nrComponents;
        m_Buffer.Data = (Byte*)stbi_loadf(*filePath.String(), &width, &height, &nrComponents, 0);
        m_Buffer.Size = width * height * 4 * FACE_COUNT; // Six layers

        if (m_Buffer.Data)
        {
            m_Loaded = true;
        }
        else
        {
            HL_CORE_ERROR(TEXTURE3D_LOG_PREFIX "[-] Failed to load Texture3D: {0} (reason: {1}) [-]", **filePath, stbi_failure_reason());
        }

        m_Specification.Format = TextureFormat::RGBA32F;
        m_Specification.Width = width;
        m_Specification.Height = height;
        m_Specification.Mips = utils::CalculateMipCount(width, height);
        m_Specification.Usage = TextureUsage::Texture;
        m_Specification.Properties = TextureProperties();
        
        Invalidate();
    }
    
    VulkanTexture3D::VulkanTexture3D(TextureFormat format, uint32 width, uint32 height, const void *data)
    {
        if (data)
        {
            m_Buffer = Allocator::Copy(data, width * height * 4 * FACE_COUNT); // Six Layers
            m_Loaded = true;
        }

        m_Specification.Format = format;
        m_Specification.Width = width;
        m_Specification.Height = height;
        m_Specification.Mips = utils::CalculateMipCount(width, height);
        m_Specification.Usage = TextureUsage::Texture;
        m_Specification.Properties = TextureProperties();

        Invalidate();
    }
    
    VulkanTexture3D::~VulkanTexture3D()
    {
        Release();
    }
    
    Allocator VulkanTexture3D::GetData()
    {
        return m_Buffer;
    }
    
    void VulkanTexture3D::Release()
    {
        if (!m_Image)
            return;

        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        vkDestroyImageView(device, m_DescriptorImageInfo.imageView, nullptr);
        vkDestroySampler(device, m_DescriptorImageInfo.sampler, nullptr);

        utils::DestroyImage(m_Image, m_MemoryAlloc);

        m_Image = nullptr;
        m_MemoryAlloc = nullptr;
        m_DescriptorImageInfo.imageView = nullptr;
        m_DescriptorImageInfo.sampler = nullptr;
    }
    
    void VulkanTexture3D::Invalidate()
    {
        auto device = VulkanContext::GetCurrentDevice();
        VkDevice vulkanDevice = device->GetNativeDevice();

        Release();

        VkFormat format = utils::VulkanTextureFormat(m_Specification.Format);
        uint32 mipCount = GetMipLevelCount();

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

        VkImageCreateInfo imageCreateInfo = {};
        imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.pNext = nullptr;
        imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
        imageCreateInfo.format = format;
        imageCreateInfo.mipLevels = mipCount;
        imageCreateInfo.arrayLayers = FACE_COUNT;
        imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageCreateInfo.extent = { m_Specification.Width, m_Specification.Height, 1 };
        imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT;
        imageCreateInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
        m_MemoryAlloc = utils::AllocateImage(imageCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, m_Image);

        m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;

        if (m_Buffer)
        {
            // Create staging buffer
            VkBufferCreateInfo bufferCreateInfo = {};
            bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferCreateInfo.pNext = nullptr;
            bufferCreateInfo.size = m_Buffer.Size;
            bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            
            VkBuffer stagingBuffer;
            VmaAllocation stagingBufferAllocation = utils::AllocateBuffer(bufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

            // copy data to staging buffer
            uint8 *dstData = utils::MapMemory<uint8>(stagingBufferAllocation);
            memcpy(dstData, m_Buffer.Data, m_Buffer.Size);
            utils::UnmapMemory(stagingBufferAllocation);

            VkCommandBuffer copyCmd = device->CreateCommandBuffer(true);

            VkImageSubresourceRange subresourceRange = {};
            subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            subresourceRange.baseMipLevel = 0;
            subresourceRange.levelCount = 1;
            subresourceRange.layerCount = FACE_COUNT;

            VkImageMemoryBarrier imageMemoryBarrier = {};
            imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            imageMemoryBarrier.pNext = nullptr;
            imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            imageMemoryBarrier.image = m_Image;
            imageMemoryBarrier.subresourceRange = subresourceRange;
            imageMemoryBarrier.srcAccessMask = 0;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

            vkCmdPipelineBarrier(copyCmd, VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

            VkBufferImageCopy bufferCopyRegion = {};
            bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            bufferCopyRegion.imageSubresource.mipLevel = 0;
            bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
            bufferCopyRegion.imageSubresource.layerCount = FACE_COUNT;
            bufferCopyRegion.imageExtent.width = m_Specification.Width;
            bufferCopyRegion.imageExtent.height = m_Specification.Height;
            bufferCopyRegion.imageExtent.depth = 1;
            bufferCopyRegion.bufferOffset = 0;

            vkCmdCopyBufferToImage(copyCmd, stagingBuffer, m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferCopyRegion);

            utils::InsertImageMemoryBarrier(copyCmd, m_Image,
                                            VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
                                            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                            subresourceRange);

            device->FlushCommandBuffer(copyCmd);

            utils::DestroyBuffer(stagingBuffer, stagingBufferAllocation);
        }

        VkCommandBuffer layoutCmd = device->CreateCommandBuffer(true);

        VkImageSubresourceRange subresourceRange = {};
        subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        subresourceRange.baseMipLevel = 0;
        subresourceRange.levelCount = mipCount;
        subresourceRange.layerCount = FACE_COUNT;

        utils::SetImageLayout(layoutCmd, m_Image, VK_IMAGE_LAYOUT_UNDEFINED, m_DescriptorImageInfo.imageLayout, subresourceRange);

        device->FlushCommandBuffer(layoutCmd);

        // Create texture sampler
        VkSamplerCreateInfo sampler = {};
        sampler.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        sampler.pNext = nullptr;
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
        sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
        sampler.maxAnisotropy = 1.0f;
        sampler.anisotropyEnable = VK_FALSE;
        VK_CHECK_RESULT(vkCreateSampler(vulkanDevice, &sampler, nullptr, &m_DescriptorImageInfo.sampler));
        utils::SetDebugUtilsObjectName(vulkanDevice, VK_OBJECT_TYPE_SAMPLER, fmt::format("Texture 3D Sampler {}", *m_Specification.DebugName), m_DescriptorImageInfo.sampler);

        // Create image view
        VkImageViewCreateInfo imageView = {};
        imageView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageView.pNext = nullptr;
        imageView.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
        imageView.format = format;
        imageView.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
        imageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageView.subresourceRange.baseMipLevel = 0;
        imageView.subresourceRange.baseArrayLayer = 0;
        imageView.subresourceRange.layerCount = FACE_COUNT;
        imageView.subresourceRange.levelCount = mipCount;
        imageView.image = m_Image;
        VK_CHECK_RESULT(vkCreateImageView(vulkanDevice, &imageView, nullptr, &m_DescriptorImageInfo.imageView));
        utils::SetDebugUtilsObjectName(vulkanDevice, VK_OBJECT_TYPE_IMAGE_VIEW, fmt::format("Texture 3D image view {}", *m_Specification.DebugName), m_DescriptorImageInfo.imageView);
    }

    void VulkanTexture3D::Resize(const glm::uvec2 &size)
    {
        Resize(size.x, size.y);
    }

    void VulkanTexture3D::Resize(const uint32 width, const uint32 height)
    {
        m_Specification.Width = width;
        m_Specification.Height = height;
        Invalidate();
    }
    
    void VulkanTexture3D::Lock()
    {
        m_Locked = true;
    }
    
    void VulkanTexture3D::Unlock()
    {
        m_Locked = false;
        Invalidate();
    }
    
    void VulkanTexture3D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
    {
        // TODO
    }
    
    glm::ivec4 VulkanTexture3D::ReadPixel(uint32 row, uint32 column)
    {
        // TODO
        return {};
    }
    
    void VulkanTexture3D::UpdateResourceData(void *data)
    {
        // TODO
    }
    
    void VulkanTexture3D::UpdateResourceData()
    {
        // TODO
    }
    
    uint32 VulkanTexture3D::GetMipLevelCount()
    {
        return utils::CalculateMipCount(m_Specification.Width, m_Specification.Height);
    }
    
    std::pair<uint32, uint32> VulkanTexture3D::GetMipSize(uint32 mip)
    {
        uint32 width = m_Specification.Width;
        uint32 height = m_Specification.Height;

        while (mip != 0)
        {
            width /= 2;
            height /= 2;
            --mip;
        }

        return { width, height };
    }
    
    void VulkanTexture3D::GenerateMips(bool readonly)
    {
        auto device = VulkanContext::GetCurrentDevice();
        VkDevice vulkanDevice = device->GetNativeDevice();
        uint32 mipLevels = GetMipLevelCount();
        VkCommandBuffer blitCmd = device->CreateCommandBuffer(true);

        for (uint32 face = 0; face < FACE_COUNT; ++face)
        {
            VkImageSubresourceRange mipSubRange = {};
            mipSubRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            mipSubRange.baseMipLevel = 0;
            mipSubRange.baseArrayLayer = face;
            mipSubRange.levelCount = 1;
            mipSubRange.layerCount = 1;

            utils::InsertImageMemoryBarrier(blitCmd, m_Image,
                                            0, VK_ACCESS_TRANSFER_WRITE_BIT,
                                            VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                            mipSubRange);
        }

        for (uint32 i = 1; i < mipLevels; ++i)
        {
            for (uint32 face = 0; face < FACE_COUNT; ++face)
            {
                VkImageBlit imageBlit = {};

                // Source
                imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                imageBlit.srcSubresource.layerCount = 1;
                imageBlit.srcSubresource.mipLevel = i - 1;
                imageBlit.srcSubresource.baseArrayLayer = face;
                imageBlit.srcOffsets[1].x = int32(m_Specification.Width >> (i - 1));
                imageBlit.srcOffsets[1].y = int32(m_Specification.Height >> (i - 1));
                imageBlit.srcOffsets[1].z = 1;

                // destionation
                imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                imageBlit.dstSubresource.layerCount = 1;
                imageBlit.dstSubresource.mipLevel = i;
                imageBlit.dstSubresource.baseArrayLayer = face;
                imageBlit.dstOffsets[1].x = int32(m_Specification.Width >> i);
                imageBlit.dstOffsets[1].y = int32(m_Specification.Height >> i);
                imageBlit.dstOffsets[1].z = 1;

                VkImageSubresourceRange mipSubRange = {};
                mipSubRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                mipSubRange.baseMipLevel = i;
                mipSubRange.baseArrayLayer = face;
                mipSubRange.levelCount = 1;
                mipSubRange.layerCount = 1;

                utils::InsertImageMemoryBarrier(blitCmd, m_Image,
                                                0, VK_ACCESS_TRANSFER_WRITE_BIT,
                                                VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                                mipSubRange);

                vkCmdBlitImage(blitCmd, m_Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                               m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageBlit, VK_FILTER_LINEAR);

                utils::InsertImageMemoryBarrier(blitCmd, m_Image, VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
                                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                                mipSubRange);
            }
        }

        VkImageSubresourceRange subresourceRange = {};
        subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        subresourceRange.layerCount = FACE_COUNT;
        subresourceRange.levelCount = mipLevels;

        utils::InsertImageMemoryBarrier(blitCmd, m_Image,
                                        VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT,
                                        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, readonly ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_GENERAL,
                                        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                                        subresourceRange);

        device->FlushCommandBuffer(blitCmd);
        m_MipsGenerated = true;
        m_DescriptorImageInfo.imageLayout = readonly ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_GENERAL;
    }
    
    VkImageView VulkanTexture3D::CreateImageViewSingleMap(uint32 mip)
    {
        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        VkFormat format = utils::VulkanTextureFormat(m_Specification.Format);

        VkImageViewCreateInfo imageView = {};
        imageView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageView.pNext = nullptr;
        imageView.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
        imageView.format = format;
        imageView.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
        imageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageView.subresourceRange.baseMipLevel = mip;
        imageView.subresourceRange.baseArrayLayer = 0;
        imageView.subresourceRange.layerCount = FACE_COUNT;
        imageView.subresourceRange.levelCount = 1;
        imageView.image = m_Image;

        VkImageView result;
        VK_CHECK_RESULT(vkCreateImageView(device, &imageView, nullptr, &result));
        utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_IMAGE_VIEW, fmt::format("Texture cube mip: {}", mip), result);
        return result;
    }
}

#endif // HIGHLO_API_VULKAN

