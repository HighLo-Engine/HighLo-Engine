// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanTexture2D.h"

#ifdef HIGHLO_API_VULKAN

#include <stb_image.h>

#include "Engine/Utils/ImageUtils.h"

#include "VulkanUtils.h"

#define TEXTURE2D_LOG_PREFIX "Texture2D>    "

namespace highlo
{
    static std::map<VkImage, WeakRef<VulkanTexture2D>> s_ImageReferences;

    VulkanTexture2D::VulkanTexture2D(const FileSystemPath &filePath, TextureFormat format, bool flipOnLoad)
        : m_FilePath(filePath)
    {
        // Load the actual image
        int32 width, height, channels;
        HL_ASSERT(!stbi_is_hdr(**filePath), "Texture is not allowed to be a HDR Texture! Please use Texture3D instead.");

        m_Buffer.Data = (Byte*)stbi_load(**filePath, &width, &height, &channels, 4);
        m_Buffer.Size = width * height * 4;

        if (!m_Buffer.Data)
        {
            HL_CORE_ERROR(TEXTURE2D_LOG_PREFIX "[-] Could not load texture {0} [-]", **filePath);
            m_Loaded = false;
            HL_ASSERT(false);
        }
        else
        {
            m_Loaded = true;
            HL_CORE_INFO(TEXTURE2D_LOG_PREFIX "[+] Loaded {0} [+]", *filePath.String());
        }

        m_Specification.Width = width;
        m_Specification.Height = height;
        m_Specification.Format = format;
        m_Specification.Properties = TextureProperties();
        m_Specification.Usage = TextureUsage::Texture;
        m_Specification.Mips = utils::CalculateMipCount(width, height);

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

        if (m_Buffer)
            m_Loaded = true;

        m_Specification.Width = 1;
        m_Specification.Height = 1;
        m_Specification.Format = format;
        m_Specification.Properties = TextureProperties();
        m_Specification.Usage = TextureUsage::Texture;
        m_Specification.Mips = utils::CalculateMipCount(1, 1);

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

        if (m_Buffer)
            m_Loaded = true;

        m_Specification.Width = width;
        m_Specification.Height = height;
        m_Specification.Format = format;
        m_Specification.Properties = TextureProperties();
        m_Specification.Usage = TextureUsage::Texture;
        m_Specification.Mips = utils::CalculateMipCount(width, height);

        Invalidate();
    }
    
    VulkanTexture2D::VulkanTexture2D(void *imgData, uint32 width, uint32 height, TextureFormat format)
    {
        uint32 size = (uint32)utils::GetImageMemorySize(format, width, height);
        if (imgData)
        {
            m_Buffer = Allocator::Copy(imgData, size);
            m_Loaded = true;
        }

        m_Specification.Format = format;
        m_Specification.Width = width;
        m_Specification.Height = height;
        m_Specification.Properties = TextureProperties();
        m_Specification.Usage = TextureUsage::Texture;
        m_Specification.Mips = utils::CalculateMipCount(width, height);

        Invalidate();
    }
    
    VulkanTexture2D::VulkanTexture2D(TextureFormat format, uint32 width, uint32 height, const void *data, TextureProperties props)
    {
        uint32 size = (uint32)utils::GetImageMemorySize(format, width, height);
        if (data)
        {
            m_Buffer = Allocator::Copy(data, size);
            m_Loaded = true;
        }

        m_Specification.Format = format;
        m_Specification.Width = width;
        m_Specification.Height = height;
        m_Specification.Properties = props;
        m_Specification.Usage = TextureUsage::Texture;
        m_Specification.Mips = utils::CalculateMipCount(width, height);

        Invalidate();
    }
    
    VulkanTexture2D::VulkanTexture2D(TextureFormat format, uint32 width, uint32 height)
    {
        m_Specification.Format = format;
        m_Specification.Width = width;
        m_Specification.Height = height;
        m_Specification.Properties = TextureProperties();
        m_Specification.Usage = TextureUsage::Texture;
        m_Specification.Mips = utils::CalculateMipCount(width, height);
        m_Loaded = false;

        Invalidate();
    }
    
    VulkanTexture2D::VulkanTexture2D(const TextureSpecification &spec)
        : m_Specification(spec)
    {
        Invalidate();
    }
    
    VulkanTexture2D::~VulkanTexture2D()
    {
        Release();
    }
    
    Allocator VulkanTexture2D::GetData()
    {
        return m_Buffer;
    }
    
    void VulkanTexture2D::Release()
    {
        ReleaseTextureInfo();

        m_Buffer.Release();
    }
    
    void VulkanTexture2D::Invalidate()
    {
        Ref<VulkanDevice> device = VulkanContext::GetCurrentDevice();
        VkDevice vulkanDevice = device->GetNativeDevice();

        ReleaseTextureInfo();

        if (!m_Buffer)
            m_Specification.Usage = TextureUsage::Storage;

        VkFormat format = utils::VulkanTextureFormat(m_Specification.Format);
        uint32 mipCount = m_Specification.Properties.GenerateMips ? GetMipLevelCount() : 1;

        InvalidateTextureInfo(m_Specification.Format, m_Specification.Usage, m_Specification.Width, m_Specification.Height, mipCount);

        if (m_Buffer)
        {
            VkDeviceSize size = (VkDeviceSize)m_Buffer.Size;

            VkBufferCreateInfo bufferCreateInfo = {};
            bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferCreateInfo.size = size;
            bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            
            VkBuffer stagingBuffer;
            VmaAllocation stagingBufferAllocation = utils::AllocateBuffer(bufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

            // Copy data to staging buffer
            uint8 *dstData = utils::MapMemory<uint8>(stagingBufferAllocation);
            HL_ASSERT(m_Buffer.Data);
            memcpy(dstData, m_Buffer.Data, size);
            utils::UnmapMemory(stagingBufferAllocation);

            VkCommandBuffer copyCmd = device->CreateCommandBuffer(true);

            // The sub resource range describes the regions of the image that will be transitioned using the memory barriers below
            VkImageSubresourceRange subresourceRange = {};
            subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            subresourceRange.baseMipLevel = 0;
            subresourceRange.levelCount = 1;
            subresourceRange.layerCount = 1;

            // Transition the texture image layout to transfer target, so we can safely copy our buffer data to it.
            VkImageMemoryBarrier imageMemoryBarrier = {};
            imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            imageMemoryBarrier.image = m_Info.Image;
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
            bufferCopyRegion.imageSubresource.layerCount = 1;
            bufferCopyRegion.imageExtent.width = m_Specification.Width;
            bufferCopyRegion.imageExtent.height = m_Specification.Height;
            bufferCopyRegion.imageExtent.depth = 1;
            bufferCopyRegion.bufferOffset = 0;

            // Copy mip levels from staging buffer
            vkCmdCopyBufferToImage(copyCmd, stagingBuffer, m_Info.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferCopyRegion);

            if (mipCount > 1) // we have mips to be generated
            {
                utils::InsertImageMemoryBarrier(copyCmd, m_Info.Image,
                                                VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT, 
                                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                                subresourceRange);
            }
            else
            {
                utils::InsertImageMemoryBarrier(copyCmd, m_Info.Image,
                                                VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT,
                                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_DescriptorImageInfo.imageLayout,
                                                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                                                subresourceRange);
            }

            device->FlushCommandBuffer(copyCmd);

            // Clean up staging buffer
            utils::DestroyBuffer(stagingBuffer, stagingBufferAllocation);
        }
        else
        {
            VkCommandBuffer transitionCommandBuffer = device->CreateCommandBuffer(true);
            VkImageSubresourceRange subresourceRange = {};
            subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            subresourceRange.layerCount = 1;
            subresourceRange.levelCount = GetMipLevelCount();
            utils::SetImageLayout(transitionCommandBuffer, m_Info.Image,
                                  VK_IMAGE_LAYOUT_UNDEFINED, m_DescriptorImageInfo.imageLayout,
                                  subresourceRange);

            device->FlushCommandBuffer(transitionCommandBuffer);
        }

        if (!m_Specification.Properties.Storage)
        {
            VkImageViewCreateInfo imageView = {};
            imageView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageView.pNext = nullptr;
            imageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageView.format = utils::VulkanTextureFormat(m_Specification.Format);
            imageView.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
            imageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageView.subresourceRange.baseMipLevel = 0;
            imageView.subresourceRange.baseArrayLayer = 0;
            imageView.subresourceRange.layerCount = 1;
            imageView.subresourceRange.levelCount = mipCount;
            imageView.image = m_Info.Image;
            VK_CHECK_RESULT(vkCreateImageView(vulkanDevice, &imageView, nullptr, &m_Info.ImageView));
            utils::SetDebugUtilsObjectName(vulkanDevice, VK_OBJECT_TYPE_IMAGE_VIEW, fmt::format("Texture view: {}", m_Specification.Properties.DebugName), m_Info.ImageView);
            
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
        m_Locked = true;
    }
    
    void VulkanTexture2D::Unlock()
    {
        m_Locked = false;
        Invalidate();
    }
    
    void VulkanTexture2D::CreatePerLayerImageViews()
    {
        HL_ASSERT(m_Specification.Layers > 1);

        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();

        VkImageAspectFlags aspectMask = utils::IsDepthFormat(m_Specification.Format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
        if (m_Specification.Format == TextureFormat::DEPTH24STENCIL8)
            aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

        const VkFormat vulkanFormat = utils::VulkanTextureFormat(m_Specification.Format);

        m_PerLayerImageViews.resize(m_Specification.Layers);
        for (uint32 layer = 0; layer < m_Specification.Layers; ++layer)
        {
            VkImageViewCreateInfo imageView = {};
            imageView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageView.pNext = nullptr;
            imageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageView.flags = 0;
            imageView.format = vulkanFormat;
            imageView.subresourceRange = {};
            imageView.subresourceRange.aspectMask = aspectMask;
            imageView.subresourceRange.baseMipLevel = 0;
            imageView.subresourceRange.levelCount = m_Specification.Mips;
            imageView.subresourceRange.baseArrayLayer = layer;
            imageView.subresourceRange.layerCount = 1;
            imageView.image = m_Info.Image;
            VK_CHECK_RESULT(vkCreateImageView(device, &imageView, nullptr, &m_PerLayerImageViews[layer]));
            utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_IMAGE_VIEW, fmt::format("{} image view layer: {}", *m_Specification.DebugName, layer), m_PerLayerImageViews[layer]);
        }
    }
    
    void VulkanTexture2D::CreateSampler(TextureProperties properties)
    {
        // TODO
        HL_ASSERT(false);
    }

    void VulkanTexture2D::CreatePerSpecificLayerImageViews(const std::vector<uint32> &layerIndices)
    {
        HL_ASSERT(m_Specification.Layers > 1);

        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();

        VkImageAspectFlags aspectFlags = utils::IsDepthFormat(m_Specification.Format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
        if (m_Specification.Format == TextureFormat::DEPTH24STENCIL8)
            aspectFlags |= VK_IMAGE_ASPECT_STENCIL_BIT;

        const VkFormat vulkanFormat = utils::VulkanTextureFormat(m_Specification.Format);

        for (uint32 layer : layerIndices)
        {
            VkImageViewCreateInfo imageView = {};
            imageView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageView.pNext = nullptr;
            imageView.flags = 0;
            imageView.format = vulkanFormat;
            imageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageView.image = m_Info.Image;
            imageView.subresourceRange = {};
            imageView.subresourceRange.aspectMask = aspectFlags;
            imageView.subresourceRange.baseMipLevel = 0;
            imageView.subresourceRange.levelCount = m_Specification.Mips;
            imageView.subresourceRange.layerCount = 1;
            imageView.subresourceRange.baseArrayLayer = layer;
            VK_CHECK_RESULT(vkCreateImageView(device, &imageView, nullptr, &m_PerLayerImageViews[layer]));
            utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_IMAGE_VIEW, fmt::format("{} image view layer: {}", *m_Specification.DebugName, layer), m_PerLayerImageViews[layer]);
        }
    }
    
    void VulkanTexture2D::UpdateResourceData()
    {
        // TODO
    }
    
    void VulkanTexture2D::UpdateResourceData(void *data)
    {
        // TODO
    }
    
    void VulkanTexture2D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
    {
        // TODO
    }
    
    glm::ivec4 VulkanTexture2D::ReadPixel(uint32 row, uint32 column)
    {
        // TODO
        return {};
    }
    
    uint32 VulkanTexture2D::GetMipLevelCount()
    {
        return utils::CalculateMipCount(m_Specification.Width, m_Specification.Height);
    }
    
    std::pair<uint32, uint32> VulkanTexture2D::GetMipSize(uint32 mip)
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
    
    void VulkanTexture2D::GenerateMips(bool readonly)
    {
        Ref<VulkanDevice> device = VulkanContext::GetCurrentDevice();
        VkDevice vulkanDevice = device->GetNativeDevice();

        const VkCommandBuffer blitCmd = device->CreateCommandBuffer(true);

        VkImageMemoryBarrier imageMemoryBarrier = {};
        imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageMemoryBarrier.pNext = nullptr;
        imageMemoryBarrier.image = m_Info.Image;
        imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

        const uint32 mipLevels = GetMipLevelCount();
        for (uint32 i = 1; i < mipLevels; ++i)
        {
            VkImageBlit imageBlit = {};
            imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageBlit.srcSubresource.layerCount = 1;
            imageBlit.srcSubresource.mipLevel = i - 1;
            imageBlit.srcOffsets[1].x = int32(m_Specification.Width >> (i - 1));
            imageBlit.srcOffsets[1].y = int32(m_Specification.Height >> (i - 1));
            imageBlit.srcOffsets[1].z = 1;

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
            utils::InsertImageMemoryBarrier(blitCmd , m_Info.Image,
                                            0, VK_ACCESS_TRANSFER_WRITE_BIT,
                                            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                            mipSubRange);

            // Blit from previous level
            vkCmdBlitImage(blitCmd, m_Info.Image, 
                           VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_Info.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                           1, &imageBlit,
                           utils::VulkanSamplerFilter(m_Specification.Properties.SamplerFilter));

            // Prepare current mip level as image blit source for next level
            utils::InsertImageMemoryBarrier(blitCmd, m_Info.Image,
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
        
        utils::InsertImageMemoryBarrier(blitCmd, m_Info.Image,
                                        VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT,
                                        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                                        subresourceRange);

        device->FlushCommandBuffer(blitCmd);
    }

    // Vulkan-specific
    VkImageView VulkanTexture2D::GetLayerImageView(uint32 layer)
    {
        HL_ASSERT(layer < m_PerLayerImageViews.size());
        return m_PerLayerImageViews[layer];
    }
    
    VkImageView VulkanTexture2D::GetMipImageView(uint32 mip)
    {
        if (m_PerMipImageViews.find(mip) == m_PerMipImageViews.end())
        {
            VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();

            VkImageAspectFlags aspectMask = utils::IsDepthFormat(m_Specification.Format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
            if (m_Specification.Format == TextureFormat::DEPTH24STENCIL8)
                aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

            VkFormat vulkanFormat = utils::VulkanTextureFormat(m_Specification.Format);

            VkImageViewCreateInfo imageViewCreateInfo = {};
            imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageViewCreateInfo.pNext = nullptr;
            imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewCreateInfo.format = vulkanFormat;
            imageViewCreateInfo.flags = 0;
            imageViewCreateInfo.subresourceRange = {};
            imageViewCreateInfo.subresourceRange.aspectMask = aspectMask;
            imageViewCreateInfo.subresourceRange.baseMipLevel = mip;
            imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
            imageViewCreateInfo.subresourceRange.levelCount = 1;
            imageViewCreateInfo.subresourceRange.layerCount = 1;
            imageViewCreateInfo.image = m_Info.Image;

            VK_CHECK_RESULT(vkCreateImageView(device, &imageViewCreateInfo, nullptr, &m_PerMipImageViews[mip]));
            utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_IMAGE_VIEW, fmt::format("{} image view mip: {}", *m_Specification.DebugName, mip), m_PerMipImageViews[mip]);
        }

        return m_PerMipImageViews.at(mip);
    }
    
    void VulkanTexture2D::InvalidateTextureInfo(TextureFormat format, TextureUsage usage, uint32 width, uint32 height, uint32 mips)
    {
        HL_ASSERT(width > 0 && height > 0);

        // Try release first if necessary
        ReleaseTextureInfo();

        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();

        VkImageUsageFlags vulkanUsage = VK_IMAGE_USAGE_SAMPLED_BIT; // TODO: this (probably) shouldn't be implied
        if (usage == TextureUsage::Attachment)
        {
            if (utils::IsDepthFormat(m_Specification.Format))
                vulkanUsage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            else
                vulkanUsage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        }
        else if (m_Specification.Usage == TextureUsage::Texture)
        {
            vulkanUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        }
        else if (m_Specification.Usage == TextureUsage::Storage)
        {
            vulkanUsage |= VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        }

        VkImageAspectFlags aspectMask = utils::IsDepthFormat(format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
        if (format == TextureFormat::DEPTH24STENCIL8)
            aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

        VkFormat vulkanFormat = utils::VulkanTextureFormat(format);

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
        imageCreateInfo.usage = vulkanUsage;
        m_Info.MemoryAlloc = utils::AllocateImage(imageCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, m_Info.Image);
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
        if (utils::IsIntegerBased(format))
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
            VkCommandBuffer commandBuffer = VulkanContext::GetCurrentDevice()->CreateCommandBuffer(true);

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

        UpdateDescriptor();
    }

    void VulkanTexture2D::ReleaseTextureInfo()
    {
        if (!m_Info.Image)
            return;

        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        vkDestroyImageView(device, m_Info.ImageView, nullptr);
        vkDestroyImage(device, m_Info.Image, nullptr);

        for (auto &view : m_PerMipImageViews)
        {
            if (view.second)
                vkDestroyImageView(device, view.second, nullptr);
        }

        for (auto &view : m_PerLayerImageViews)
        {
            if (view)
                vkDestroyImageView(device, view, nullptr);
        }

        utils::DestroyImage(m_Info.Image, m_Info.MemoryAlloc);
        s_ImageReferences.erase(m_Info.Image);

        m_Info.Image = nullptr;
        m_Info.ImageView = nullptr;
        m_Info.Sampler = nullptr;

        m_PerLayerImageViews.clear();
        m_PerMipImageViews.clear();
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

        m_DescriptorImageInfo.imageView = m_Info.ImageView;
        m_DescriptorImageInfo.sampler = m_Info.Sampler;
    }
}

#endif // HIGHLO_API_VULKAN

