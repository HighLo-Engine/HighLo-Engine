#include "HighLoPch.h"
#include "VulkanTexture3D.h"

#ifdef HIGHLO_API_VULKAN

#include <stb_image.h>
#include "VulkanContext.h"

#include "Engine/Utils/ImageUtils.h"

namespace highlo
{
    VulkanTexture3D::VulkanTexture3D(const FileSystemPath &filePath, bool flipOnLoad)
    {
        int32 width, height, channels;
        HL_ASSERT(stbi_is_hdr(*filePath.String()));

        m_Buffer.m_Data = (Byte*)stbi_loadf(*filePath.String(), &width, &height, &channels, 4);
        m_Buffer.m_Size = width * height * 4 * sizeof(float);
        m_Specification.Format = TextureFormat::RGBA32F;

        HL_ASSERT(m_Buffer.m_Data, "Failed to load image!");

        m_Specification.Width = width;
        m_Specification.Height = height;
        m_Specification.Mips = GetMipLevelCount();
        m_Specification.Properties = TextureProperties();

        Invalidate();
    }

    VulkanTexture3D::VulkanTexture3D(TextureFormat format, uint32 width, uint32 height, const void *data)
    {
        m_Specification.Format = format;
        m_Specification.Width = width;
        m_Specification.Height = height;
        m_Specification.Properties = TextureProperties();
        m_Specification.Mips = GetMipLevelCount();

        if (data)
        {
            uint32 size = width * height * 4 * 4;
            m_Buffer = Allocator::Copy(data, size);
        }

        Invalidate();
    }

    VulkanTexture3D::~VulkanTexture3D()
    {
        Release();
    }

    Allocator VulkanTexture3D::GetData()
    {
        HL_ASSERT(m_Locked, "Texture must be locked!");
        return m_Buffer;
    }

    void VulkanTexture3D::Bind(uint32 slot) const
    {
    }

    void VulkanTexture3D::Unbind(uint32 slot) const
    {
    }

    void VulkanTexture3D::Release()
    {
        HL_CORE_TRACE("Destroying VulkanTexture3D");

        auto device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        vkDestroyImageView(device, m_DescriptorImageInfo.imageView, nullptr);
        vkDestroySampler(device, m_DescriptorImageInfo.sampler, nullptr);

        VulkanAllocator allocator("VulkanTexture3D");
        allocator.DestroyImage(m_Image, m_MemoryAlloc);
    }

    void VulkanTexture3D::Invalidate()
    {
        auto &device = VulkanContext::GetCurrentDevice();
        auto nativeDevice = device->GetNativeDevice();
        VulkanAllocator allocator("VulkanTexture3D");

        VkFormat format = utils::VulkanImageFormat(m_Specification.Format);
        uint32 mipCount = GetMipLevelCount();

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

        // Create optimal tiled target image
        VkImageCreateInfo imageCreateInfo{};
        imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
        imageCreateInfo.format = format;
        imageCreateInfo.mipLevels = mipCount;
        imageCreateInfo.arrayLayers = 6;
        imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageCreateInfo.extent = { m_Specification.Width, m_Specification.Height, 1 };
        imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT;
        imageCreateInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
        m_MemoryAlloc = allocator.AllocateImage(imageCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, m_Image);

        m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;

        // Copy data if data is present
        if (m_Buffer)
        {
            // Create staging buffer
            VkBufferCreateInfo bufferCreateInfo{};
            bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferCreateInfo.size = m_Buffer.m_Size;
            bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            
            VkBuffer stagingBuffer;
            VmaAllocation stagingBufferAllocation = allocator.AllocateBuffer(bufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

            // Copy data to staging buffer
            uint8 *dstData = allocator.MapMemory<uint8>(stagingBufferAllocation);
            memcpy(dstData, m_Buffer.m_Data, m_Buffer.m_Size);
            allocator.UnmapMemory(stagingBufferAllocation);

            VkCommandBuffer copyCmd = device->CreateCommandBuffer(true);

            // Image memory barriers
            VkImageSubresourceRange subresourceRange = {};
            subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            subresourceRange.baseMipLevel = 0;
            subresourceRange.levelCount = 1;
            subresourceRange.layerCount = 6;

            VkImageMemoryBarrier memoryBarrier{};
            memoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            memoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            memoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

            memoryBarrier.image = m_Image;
            memoryBarrier.subresourceRange = subresourceRange;
            memoryBarrier.srcAccessMask = 0;
            memoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            memoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            memoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

            vkCmdPipelineBarrier(copyCmd, 
                                 VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 
                                 0,
                                 0, nullptr,
                                 0, nullptr,
                                 1, &memoryBarrier);

            VkBufferImageCopy bufferCopyRegion = {};
            bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            bufferCopyRegion.imageSubresource.mipLevel = 0;
            bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
            bufferCopyRegion.imageSubresource.layerCount = 6;
            bufferCopyRegion.imageExtent.width = m_Specification.Width;
            bufferCopyRegion.imageExtent.height = m_Specification.Height;
            bufferCopyRegion.imageExtent.depth = 1;
            bufferCopyRegion.bufferOffset = 0;

            // Copy mip levels from staging buffer
            vkCmdCopyBufferToImage(copyCmd, stagingBuffer, m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferCopyRegion);

            utils::InsertTextureMemoryBarrier(copyCmd, m_Image,
                                              VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
                                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                              VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                              subresourceRange);

            device->FlushCommandBuffer(copyCmd);
            allocator.DestroyBuffer(stagingBuffer, stagingBufferAllocation);
        }

        VkCommandBuffer layoutCmd = device->CreateCommandBuffer(true);

        VkImageSubresourceRange subresourceRange = {};
        subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        subresourceRange.baseMipLevel = 0;
        subresourceRange.levelCount = mipCount;
        subresourceRange.layerCount = 6;

        utils::SetTextureLayout(layoutCmd, m_Image, VK_IMAGE_LAYOUT_UNDEFINED, m_DescriptorImageInfo.imageLayout, subresourceRange);
        device->FlushCommandBuffer(layoutCmd);

        // =====================================================================================================================================================
        // ====================================================== Create Texture Sampler =======================================================================
        // =====================================================================================================================================================

        VkSamplerCreateInfo sampler{};
        sampler.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        sampler.anisotropyEnable = VK_FALSE;
        sampler.maxAnisotropy = 1.0f;
        sampler.minFilter = utils::VulkanSamplerFilter(m_Specification.Properties.SamplerFilter);
        sampler.magFilter = utils::VulkanSamplerFilter(m_Specification.Properties.SamplerFilter);
        sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        sampler.addressModeU = utils::VulkanSamplerWrap(m_Specification.Properties.SamplerWrap);
        sampler.addressModeV = utils::VulkanSamplerWrap(m_Specification.Properties.SamplerWrap);
        sampler.addressModeW = utils::VulkanSamplerWrap(m_Specification.Properties.SamplerWrap);
        sampler.mipLodBias = 0.0f;
        sampler.compareOp = VK_COMPARE_OP_NEVER;
        sampler.minLod = 0.0f;
        sampler.maxLod = (float)mipCount;
        sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
        VK_CHECK_RESULT(vkCreateSampler(nativeDevice, &sampler, nullptr, &m_DescriptorImageInfo.sampler));

        // Create Image View
        VkImageViewCreateInfo view{};
        view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        view.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
        view.format = format;
        view.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
        view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        view.subresourceRange.baseMipLevel = 0;
        view.subresourceRange.baseArrayLayer = 0;
        view.subresourceRange.layerCount = 6;
        view.subresourceRange.levelCount = mipCount;
        view.image = m_Image;
        VK_CHECK_RESULT(vkCreateImageView(nativeDevice, &view, nullptr, &m_DescriptorImageInfo.imageView));
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
        HL_ASSERT(false, "Not implemented yet");
    }

    glm::ivec4 VulkanTexture3D::ReadPixel(uint32 row, uint32 column)
    {
        // TODO
        HL_ASSERT(false, "Not implemented yet");
        return glm::ivec4();
    }

    void VulkanTexture3D::UpdateResourceData(void *data)
    {
        uint32 size = m_Specification.Width * m_Specification.Height * 4 * sizeof(float);
        if (m_Buffer)
            m_Buffer.Release();

        m_Buffer = Allocator::Copy(data, size);
        Invalidate();
    }

    void VulkanTexture3D::UpdateResourceData()
    {
        Invalidate();
    }

    uint32 VulkanTexture3D::GetMipLevelCount()
    {
        return utils::CalculateMipCount(m_Specification.Width, m_Specification.Height);
    }

    void VulkanTexture3D::GenerateMips(bool readOnly)
    {
        auto &device = VulkanContext::GetCurrentDevice();
        auto nativeDevice = device->GetNativeDevice();

        VkCommandBuffer blitCmd = device->CreateCommandBuffer(true);
        uint32 mipLevels = GetMipLevelCount();
        
        for (uint32 face = 0; face < 6; ++face)
        {
            VkImageSubresourceRange mipSubRange = {};
            mipSubRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            mipSubRange.baseMipLevel = 0;
            mipSubRange.baseArrayLayer = face;
            mipSubRange.levelCount = 1;
            mipSubRange.layerCount = 1;

            // Prepare current mup level as image blit destination
            utils::InsertTextureMemoryBarrier(blitCmd, m_Image,
                                              0, VK_ACCESS_TRANSFER_WRITE_BIT,
                                              VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                              VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                              mipSubRange);
        }

        for (uint32 i = 1; i < mipLevels; ++i)
        {
            for (uint32 face = 0; face < 6; ++face)
            {
                VkImageBlit imageBlit{};

                // Source
                imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                imageBlit.srcSubresource.layerCount = 1;
                imageBlit.srcSubresource.mipLevel = i - 1;
                imageBlit.srcSubresource.baseArrayLayer = face;
                imageBlit.srcOffsets[1].x = (int32)(m_Specification.Width >> (i - 1));
                imageBlit.srcOffsets[1].y = (int32)(m_Specification.Height >> (i - 1));
                imageBlit.srcOffsets[1].z = 1;
                 
                // Destination
                imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                imageBlit.dstSubresource.layerCount = 1;
                imageBlit.dstSubresource.mipLevel = i;
                imageBlit.dstSubresource.baseArrayLayer= face;
                imageBlit.dstOffsets[1].x = (int32)(m_Specification.Width >> i);
                imageBlit.dstOffsets[1].y = (int32)(m_Specification.Height >> i);
                imageBlit.dstOffsets[1].z = 1;

                VkImageSubresourceRange mipSubRange = {};
                mipSubRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                mipSubRange.baseMipLevel = i;
                mipSubRange.baseArrayLayer = face;
                mipSubRange.levelCount = 1;
                mipSubRange.layerCount = 1;

                // Prepare current mip level as image blit destination
                utils::InsertTextureMemoryBarrier(blitCmd, m_Image,
                                                  0, VK_ACCESS_TRANSFER_WRITE_BIT,
                                                  VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                                  VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                                  mipSubRange);

                // Bit from previous level
                vkCmdBlitImage(blitCmd, m_Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageBlit, VK_FILTER_LINEAR);

                // Prepare current mip level as image blit source for next level
                utils::InsertTextureMemoryBarrier(blitCmd, m_Image,
                                                  VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
                                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                                  VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                                  mipSubRange);
            }
        }

        // After the loop, all mip layers are in TRANSFER_SRC layout, so transition all of them into SHADER_READ

        VkImageSubresourceRange subresourceRange = {};
        subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        subresourceRange.layerCount = 6;
        subresourceRange.levelCount = mipLevels;

        utils::InsertTextureMemoryBarrier(blitCmd, m_Image,
                                          VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT,
                                          VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, readOnly ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_GENERAL,
                                          VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                                          subresourceRange);

        device->FlushCommandBuffer(blitCmd);
        m_MipsGenerated = true;
        m_DescriptorImageInfo.imageLayout = readOnly ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_GENERAL;
    }
    
    VkImageView VulkanTexture3D::CreateImageViewSingleMip(uint32 mip)
    {
        // TODO: Assert mip count

        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        VkFormat format = utils::VulkanImageFormat(m_Specification.Format);
        VkImageView result;

        VkImageViewCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
        imageInfo.format = format;
        imageInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
        imageInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageInfo.subresourceRange.baseMipLevel = mip;
        imageInfo.subresourceRange.baseArrayLayer = 0;
        imageInfo.subresourceRange.layerCount = 6;
        imageInfo.subresourceRange.levelCount = 1;
        imageInfo.image = m_Image;

        VK_CHECK_RESULT(vkCreateImageView(device, &imageInfo, nullptr, &result));
        return result;
    }
}

#endif // HIGHLO_API_VULKAN

