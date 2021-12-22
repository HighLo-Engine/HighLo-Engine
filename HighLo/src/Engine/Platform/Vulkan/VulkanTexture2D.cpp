// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanTexture2D.h"

#ifdef HIGHLO_API_VULKAN

#include "Engine/Utils/ImageUtils.h"
#include <stb_image.h>

#include "VulkanContext.h"

namespace highlo
{
    namespace utils
    {
        static uint64 GetMemorySize(TextureFormat format, uint32 width, uint32 height)
        {
            switch (format)
            {
                case TextureFormat::RGBA:       return width * height * 4;
                case TextureFormat::RGBA32F:    return width * height * 4 * sizeof(float);

                case TextureFormat::Depth:      return width * height * sizeof(float);
            }

            HL_ASSERT(false);
            return 0;
        }
    }

    static std::map<VkImage, VulkanTexture2D*> s_TextureReferences;

    VulkanTexture2D::VulkanTexture2D(const FileSystemPath &filePath, TextureFormat format, bool flipOnLoad)
    {
        int32 width, height, channels;
        HL_ASSERT(!stbi_is_hdr(*filePath.String()));

        m_Buffer.m_Data = stbi_load(*filePath.String(), &width, &height, &channels, 4);
        m_Buffer.m_Size = width * height * 4;

        HL_ASSERT(m_Buffer.m_Data, "Failed to load image!");
        
        m_Specification.Width = width;
        m_Specification.Height = height;
        m_Specification.Format = format;
        m_Specification.Mips = GetMipLevelCount();
        m_Specification.Properties = TextureProperties();

        HL_ASSERT(m_Specification.Format != TextureFormat::None);

        Invalidate();
    }

    VulkanTexture2D::VulkanTexture2D(const glm::vec3 &rgb, TextureFormat format)
    {
        if (format == TextureFormat::RGBA8)
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
        m_Specification.Width = width;
        m_Specification.Height = height;
        m_Specification.Format = format;
        m_Specification.Properties = TextureProperties();
        m_Specification.Mips = GetMipLevelCount();

        uint64 size = utils::GetMemorySize(format, width, height);
        if (imgData)
            m_Buffer = Allocator::Copy(imgData, (uint32)size);

        Invalidate();
    }
    
    VulkanTexture2D::VulkanTexture2D(TextureFormat format, uint32 width, uint32 height, const void *data, TextureProperties props)
    {
        m_Specification.Width = width;
        m_Specification.Height = height;
        m_Specification.Format = format;
        m_Specification.Properties = props;
        m_Specification.Mips = GetMipLevelCount();

        uint64 size = utils::GetMemorySize(format, width, height);
        if (data)
            m_Buffer = Allocator::Copy(data, (uint32)size);

        Invalidate();
    }
    
    VulkanTexture2D::VulkanTexture2D(TextureFormat format, uint32 width, uint32 height)
    {
        m_Specification.Width = width;
        m_Specification.Height = height;
        m_Specification.Format = format;
        m_Specification.Properties = TextureProperties();
        m_Specification.Mips = GetMipLevelCount();

        uint64 size = utils::GetMemorySize(format, width, height);
        m_Buffer.Allocate((uint32)size);

        Invalidate();
    }
    
    VulkanTexture2D::~VulkanTexture2D()
    {
        Release();
    }

    Allocator VulkanTexture2D::GetData()
    {
        HL_ASSERT(m_Locked, "Texture must be locked!");
        return m_Buffer;
    }

    void VulkanTexture2D::Bind(uint32 slot) const
    {
    }

    void VulkanTexture2D::Unbind(uint32 slot) const
    {
    }

    void VulkanTexture2D::Release()
    {
        m_Buffer.Release();

        const VkDevice vulkanDevice = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        vkDestroyImageView(vulkanDevice, m_Info.ImageView, nullptr);
        vkDestroySampler(vulkanDevice, m_Info.Sampler, nullptr);

        for (auto &view : m_PerLayerImageViews)
        {
            if (view)
                vkDestroyImageView(vulkanDevice, view, nullptr);
        }

        VulkanAllocator allocator("VulkanTexture2D");
        allocator.DestroyImage(m_Info.Image, m_Info.MemoryAlloc);
        s_TextureReferences.erase(m_Info.Image);

        m_Info.Image = nullptr;
        m_Info.ImageView = nullptr;
        m_Info.Sampler = nullptr;

        m_PerLayerImageViews.clear();
        m_MipImageViews.clear();
    }

    void VulkanTexture2D::Invalidate()
    {
        auto device = VulkanContext::GetCurrentDevice();
        auto nativeDevice = device->GetNativeDevice();
        VulkanAllocator allocator("Texture2D");

        uint32 mipCount = m_Specification.Properties.GenerateMips ? GetMipLevelCount() : 1;

        // Invalidate image data
        HL_ASSERT(m_Specification.Width > 0 && m_Specification.Height > 0);

        VkImageUsageFlags usage = VK_IMAGE_USAGE_SAMPLED_BIT; // TODO: this should be parameterized
        if (m_Specification.Usage == TextureUsage::Attachment)
        {
            if (utils::IsDepthFormat(m_Specification.Format))
                usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            else
                usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        }
        else if (m_Specification.Usage == TextureUsage::Texture)
        {
            usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        }
        else if (m_Specification.Usage == TextureUsage::Storage)
        {
            usage |= VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        }

        VkImageAspectFlags aspectMask = utils::IsDepthFormat(m_Specification.Format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
        if (m_Specification.Format == TextureFormat::DEPTH24STENCIL8)
            aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

        VkFormat vulkanFormat = utils::VulkanImageFormat(m_Specification.Format);

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
        imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageCreateInfo.usage = usage;
        m_Info.MemoryAlloc = allocator.AllocateImage(imageCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, m_Info.Image);
        s_TextureReferences[m_Info.Image] = this;

        // Create default image view
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
        VK_CHECK_RESULT(vkCreateImageView(nativeDevice, &imageViewCreateInfo, nullptr, &m_Info.ImageView));

        // TODO: VulkanRenderingApi should contain a cache of Samplers
        VkSamplerCreateInfo samplerCreateInfo = {};
        samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerCreateInfo.maxAnisotropy = 1.0f;
        samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
        samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
        samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerCreateInfo.addressModeV = samplerCreateInfo.addressModeU;
        samplerCreateInfo.addressModeW = samplerCreateInfo.addressModeU;
        samplerCreateInfo.mipLodBias = 0.0f;
        samplerCreateInfo.minLod = 0.0f;
        samplerCreateInfo.maxLod = 100.0f;
        samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
        VK_CHECK_RESULT(vkCreateSampler(nativeDevice, &samplerCreateInfo, nullptr, &m_Info.Sampler));

        if (m_Specification.Usage == TextureUsage::Storage)
        {
            VkCommandBuffer commandBuffer = VulkanContext::GetCurrentDevice()->CreateCommandBuffer(true);

            VkImageSubresourceRange subresourceRange = {};
            subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            subresourceRange.baseMipLevel = 0;
            subresourceRange.levelCount = m_Specification.Mips;
            subresourceRange.layerCount = m_Specification.Layers;

            utils::InsertTextureMemoryBarrier(commandBuffer, m_Info.Image, 0, 0, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, subresourceRange);
            VulkanContext::GetCurrentDevice()->FlushCommandBuffer(commandBuffer);
        }

        UpdateDescriptor();

        if (m_Buffer)
        {
            VkDeviceSize size = m_Buffer.m_Size;

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

            // Create staging buffer
            VkBufferCreateInfo bufferCreateInfo{};
            bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferCreateInfo.size = size;
            bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            VkBuffer stagingBuffer;
            VmaAllocation stagingBufferAllocation = allocator.AllocateBuffer(bufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

            // Copy data to staging buffer
            uint8 *dstData = allocator.MapMemory<uint8>(stagingBufferAllocation);
            HL_ASSERT(m_Buffer.m_Data);
            memcpy(dstData, m_Buffer.m_Data, size);
            allocator.UnmapMemory(stagingBufferAllocation);

            VkCommandBuffer copyCmd = device->CreateCommandBuffer(true);

            // Image memory barriers for the texture image
            
            VkImageSubresourceRange subresourceRange = {};
            subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            subresourceRange.baseMipLevel = 0;
            subresourceRange.levelCount = 1;
            subresourceRange.layerCount = 1;

            VkImageMemoryBarrier imageMemoryBarrier{};
            imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

            imageMemoryBarrier.image = m_Info.Image;
            imageMemoryBarrier.subresourceRange = subresourceRange;
            imageMemoryBarrier.srcAccessMask = 0;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

            // Insert a memory dependency
            vkCmdPipelineBarrier(copyCmd, VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

            VkBufferImageCopy bufferCopyRegion = {};
            bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            bufferCopyRegion.imageSubresource.mipLevel = 0;
            bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
            bufferCopyRegion.imageSubresource.layerCount = 1;
            bufferCopyRegion.imageExtent.width = m_Specification.Width;
            bufferCopyRegion.imageExtent.height = m_Specification.Height;
            bufferCopyRegion.imageExtent.depth = 1;
            bufferCopyRegion.bufferOffset = 0;

            vkCmdCopyBufferToImage(copyCmd, stagingBuffer, m_Info.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferCopyRegion);

            if (mipCount > 1)
            {
                utils::InsertTextureMemoryBarrier(copyCmd, 
                                                  m_Info.Image,
                                                  VK_ACCESS_TRANSFER_WRITE_BIT,  VK_ACCESS_TRANSFER_READ_BIT, 
                                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,  VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                                  VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 
                                                  subresourceRange);
            }
            else
            {
                utils::InsertTextureMemoryBarrier(copyCmd,
                                                  m_Info.Image,
                                                  VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT,
                                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, GetVulkanDescriptorInfo().imageLayout,
                                                  VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                                                  subresourceRange);
            }

            device->FlushCommandBuffer(copyCmd);
            allocator.DestroyBuffer(stagingBuffer, stagingBufferAllocation);
        }
        else
        {
            VkCommandBuffer transitionCommandBuffer = device->CreateCommandBuffer(true);
            VkImageSubresourceRange subresourceRange = {};
            subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            subresourceRange.layerCount = 1;
            subresourceRange.levelCount = GetMipLevelCount();
            utils::SetTextureLayout(transitionCommandBuffer, m_Info.Image, VK_IMAGE_LAYOUT_UNDEFINED, GetVulkanDescriptorInfo().imageLayout, subresourceRange);
            device->FlushCommandBuffer(transitionCommandBuffer);
        }
        
        // ===============================================================================================================================================
        // ===================================================== Create Texture Sampler ==================================================================
        // ===============================================================================================================================================

        VkSamplerCreateInfo sampler{};
        sampler.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
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

        sampler.anisotropyEnable = false;
        sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
        VK_CHECK_RESULT(vkCreateSampler(nativeDevice, &sampler, nullptr, &m_Info.Sampler));

        if (!m_Specification.Properties.Storage)
        {
            VkImageViewCreateInfo imageView = {};
            imageView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageView.format = utils::VulkanImageFormat(m_Specification.Format);
            imageView.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
            imageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageView.subresourceRange.baseMipLevel = 0;
            imageView.subresourceRange.baseArrayLayer = 0;
            imageView.subresourceRange.layerCount = 1;
            imageView.subresourceRange.levelCount = mipCount;
            imageView.image = m_Info.Image;
            VK_CHECK_RESULT(vkCreateImageView(nativeDevice, &imageView, nullptr, &m_Info.ImageView));
        }

        if (m_Buffer && m_Specification.Properties.GenerateMips && mipCount > 1)
            GenerateMips();

        m_Buffer.Release();
    }

    void VulkanTexture2D::Lock()
    {
        m_Locked = true;
    }

    void VulkanTexture2D::Unlock()
    {
        m_Locked = false;
        Invalidate();
    }

    void VulkanTexture2D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
    {
        // TODO
        HL_ASSERT(false, "Not implemented yet");
    }

    glm::ivec4 VulkanTexture2D::ReadPixel(uint32 row, uint32 column)
    {
        // TODO
        HL_ASSERT(false, "Not implemented yet");
        return glm::ivec4();
    }

    void VulkanTexture2D::UpdateResourceData(void *data)
    {
        uint64 size = utils::GetMemorySize(m_Specification.Format, m_Specification.Width, m_Specification.Height);
        if (m_Buffer)
            m_Buffer.Release();

        m_Buffer = Allocator::Copy(data, (uint32)size);
        Invalidate();
    }

    void VulkanTexture2D::UpdateResourceData()
    {
        Invalidate();
    }

    uint32 VulkanTexture2D::GetMipLevelCount()
    {
        return utils::CalculateMipCount(m_Specification.Width, m_Specification.Height);
    }

    void VulkanTexture2D::CreatePerLayerImageViews()
    {
        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();

        VkImageAspectFlags aspectMask = utils::IsDepthFormat(m_Specification.Format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
        if (m_Specification.Format == TextureFormat::DEPTH24STENCIL8)
            aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

        const VkFormat vulkanFormat = utils::VulkanImageFormat(m_Specification.Format);
        for (uint32 layer = 0; layer < m_Specification.Layers; ++layer)
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
        }
    }

    void VulkanTexture2D::CreateSampler(TextureProperties properties)
    {
        // TODO
        HL_ASSERT(false, "Not implemented yet");
    }

    void VulkanTexture2D::GenerateMips()
    {
        auto &device = VulkanContext::GetCurrentDevice();
        auto nativeDevice = device->GetNativeDevice();

        const VkCommandBuffer blitCmd = VulkanContext::GetCurrentDevice()->CreateCommandBuffer(true);

        VkImageMemoryBarrier barrier = {};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.image = m_Info.Image;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

        const auto &mipLevels = GetMipLevelCount();
        for (uint32 i = 1; i < mipLevels; ++i)
        {
            VkImageBlit imageBlit{};

            // Source
            imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageBlit.srcSubresource.layerCount = 1;
            imageBlit.srcSubresource.mipLevel = i - 1;
            imageBlit.srcOffsets[1].x = (int32)(m_Specification.Width >> (i - 1));
            imageBlit.srcOffsets[1].y = (int32)(m_Specification.Height >> (i - 1));
            imageBlit.srcOffsets[1].z = 1;

            // Destination
            imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageBlit.dstSubresource.layerCount = 1;
            imageBlit.dstSubresource.mipLevel = i;
            imageBlit.dstOffsets[1].x = (int32)(m_Specification.Width >> i);
            imageBlit.dstOffsets[1].y = (int32)(m_Specification.Height >> i);
            imageBlit.dstOffsets[1].z = 1;

            VkImageSubresourceRange subresourceRange = {};
            subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            subresourceRange.baseMipLevel = i;
            subresourceRange.levelCount = 1;
            subresourceRange.layerCount = 1;

            // Prepare current mip level as image blit destination
            utils::InsertTextureMemoryBarrier(blitCmd, m_Info.Image, 
                                              0, VK_ACCESS_TRANSFER_WRITE_BIT, 
                                              VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                              VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                              subresourceRange);

            vkCmdBlitImage(blitCmd, m_Info.Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_Info.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageBlit, VK_FILTER_LINEAR);

            // Prepare current mip level as image blit source for the next level
            utils::InsertTextureMemoryBarrier(blitCmd, m_Info.Image,
                                              VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
                                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                              VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                              subresourceRange);
        }

        // After the loop all mip layers are in TRANSFER_SRC layout, transition all of the to SHADER_READ
        VkImageSubresourceRange subresourceRange = {};
        subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        subresourceRange.layerCount = 1;
        subresourceRange.levelCount = mipLevels;

        utils::InsertTextureMemoryBarrier(blitCmd, m_Info.Image,
                                          VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT,
                                          VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                          VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                                          subresourceRange);

        VulkanContext::GetCurrentDevice()->FlushCommandBuffer(blitCmd);
    }

    VkImageView VulkanTexture2D::GetMipImageView(uint32 mip)
    {
        if (m_MipImageViews.find(mip) != m_MipImageViews.end())
            return m_MipImageViews.at(mip);

        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        VkImageAspectFlags aspectMask = utils::IsDepthFormat(m_Specification.Format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;

        if (m_Specification.Format == TextureFormat::DEPTH24STENCIL8)
            aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

        VkFormat vulkanFormat = utils::VulkanImageFormat(m_Specification.Format);

        m_PerLayerImageViews.resize(m_Specification.Layers);
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

        VK_CHECK_RESULT(vkCreateImageView(device, &imageViewCreateInfo, nullptr, &m_MipImageViews[mip]));
        return m_MipImageViews.at(mip);
    }

    void VulkanTexture2D::CreatePerSpecificLayerImageViews(const std::vector<uint32> &layerIndices)
    {
        HL_ASSERT(m_Specification.Layers > 1);

        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();

        VkImageAspectFlags aspectMask = utils::IsDepthFormat(m_Specification.Format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
        if (m_Specification.Format == TextureFormat::DEPTH24STENCIL8)
            aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

        const VkFormat vulkanFormat = utils::VulkanImageFormat(m_Specification.Format);

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
        }
    }

    void VulkanTexture2D::UpdateDescriptor()
    {
        if (m_Specification.Format == TextureFormat::DEPTH24STENCIL8 || m_Specification.Format == TextureFormat::DEPTH32F)
            m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
        else if (m_Specification.Usage == TextureUsage::Storage)
            m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
        else
            m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        // In case another branch was selected before
        if (m_Specification.Usage == TextureUsage::Storage)
            m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;

        m_DescriptorImageInfo.imageView = m_Info.ImageView;
        m_DescriptorImageInfo.sampler = m_Info.Sampler;
    }
}

#endif // HIGHLO_API_VULKAN

