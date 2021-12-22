// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanTexture.h"

#ifdef HIGHLO_API_VULKAN

#include "Engine/Utils/ImageUtils.h"
#include <stb_image.h>

#include "VulkanContext.h"

namespace highlo
{
    namespace utils
    {
        static VkSamplerAddressMode VulkanSamplerWrap(TextureWrap wrap)
        {
            switch (wrap)
            {
                case TextureWrap::Clamp:        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
                case TextureWrap::Repeat:       return VK_SAMPLER_ADDRESS_MODE_REPEAT;
            }

            HL_ASSERT(false);
            return (VkSamplerAddressMode)0;
        }

        static VkFilter VUlkanSamplerFilter(TextureFilter filter)
        {
            switch (filter)
            {
                case TextureFilter::Linear:     return VK_FILTER_LINEAR;
                case TextureFilter::Nearest:    return VK_FILTER_NEAREST;
            }

            HL_ASSERT(false);
            return (VkFilter)0;
        }

        static uint64 GetMemorySize(TextureFormat format, uint32 width, uint32 height)
        {
            switch (format)
            {
                case TextureFormat::RGBA:       return width * height * 4;
                case TextureFormat::RGBA32F:    return width * height * 4 * sizeof(float);
            }

            HL_ASSERT(false);
            return 0;
        }
    }

    /// =======================================================================================================================
    /// ============================================== Texture2D ==============================================================
    /// =======================================================================================================================

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
        // TODO
    }
    
    VulkanTexture2D::VulkanTexture2D(const glm::vec3 &rgb, uint32 width, uint32 height, TextureFormat format)
    {
        // TODO
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
    }

    void VulkanTexture2D::Invalidate()
    {
        auto device = VulkanContext::GetCurrentDevice();
        auto naticeDevice = device->GetNativeDevice();

        uint32 mipCount = m_Specification.Properties.GenerateMips ? GetMipLevelCount() : 1;


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
    }

    glm::ivec4 VulkanTexture2D::ReadPixel(uint32 row, uint32 column)
    {
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
    }

    uint32 VulkanTexture2D::GetMipLevelCount()
    {
        return utils::CalculateMipCount(m_Specification.Width, m_Specification.Height);
    }

    void VulkanTexture2D::CreatePerLayerImageViews()
    {
    }

    void VulkanTexture2D::CreateSampler(TextureProperties properties)
    {
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

    /// =======================================================================================================================
    /// ============================================== Texture3D ==============================================================
    /// =======================================================================================================================

    VulkanTexture3D::VulkanTexture3D(const FileSystemPath &filePath, bool flipOnLoad)
    {
        int32 width, height, channels;
        HL_ASSERT(!stbi_is_hdr(*filePath.String()));

        m_Buffer.m_Data = stbi_load(*filePath.String(), &width, &height, &channels, 4);
        m_Buffer.m_Size = width * height * 4;

        HL_ASSERT(m_Buffer.m_Data, "Failed to load image!");

        m_Specification.Width = width;
        m_Specification.Height = height;
        m_Specification.Format = TextureFormat::DEPTH24STENCIL8;
        m_Specification.Mips = GetMipLevelCount();
        m_Specification.Properties = TextureProperties();

        HL_ASSERT(m_Specification.Format != TextureFormat::None);

        Invalidate();
    }

    VulkanTexture3D::VulkanTexture3D(TextureFormat format, uint32 width, uint32 height, const void *data)
    {
    }

    VulkanTexture3D::~VulkanTexture3D()
    {
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
    }

    void VulkanTexture3D::Invalidate()
    {
    }

    void VulkanTexture3D::Lock()
    {
    }

    void VulkanTexture3D::Unlock()
    {
    }

    void VulkanTexture3D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
    {
    }

    glm::ivec4 VulkanTexture3D::ReadPixel(uint32 row, uint32 column)
    {
        return glm::ivec4();
    }

    void VulkanTexture3D::UpdateResourceData(void *data)
    {
    }

    void VulkanTexture3D::UpdateResourceData()
    {
    }

    uint32 VulkanTexture3D::GetMipLevelCount()
    {
        return utils::CalculateMipCount(m_Specification.Width, m_Specification.Height);
    }

}

#endif // HIGHLO_API_VULKAN