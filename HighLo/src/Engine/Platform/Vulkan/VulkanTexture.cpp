// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanTexture.h"

#ifdef HIGHLO_API_VULKAN

#include "Engine/Utils/ImageUtils.h"

namespace highlo
{
    /// =======================================================================================================================
    /// ============================================== Texture2D ==============================================================
    /// =======================================================================================================================

    VulkanTexture2D::VulkanTexture2D(const FileSystemPath &filePath, TextureFormat format, bool flipOnLoad)
    {
    }

    VulkanTexture2D::VulkanTexture2D(const glm::vec3 &rgb, TextureFormat format)
    {
    }
    
    VulkanTexture2D::VulkanTexture2D(const glm::vec3 &rgb, uint32 width, uint32 height, TextureFormat format)
    {
    }
    
    VulkanTexture2D::VulkanTexture2D(void *imgData, uint32 width, uint32 height, TextureFormat format)
    {
    }
    
    VulkanTexture2D::VulkanTexture2D(TextureFormat format, uint32 width, uint32 height, const void *data, TextureProperties props)
    {
    }
    
    VulkanTexture2D::VulkanTexture2D(TextureFormat format, uint32 width, uint32 height)
    {
    }
    
    VulkanTexture2D::~VulkanTexture2D()
    {
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
    }

    void VulkanTexture2D::Invalidate()
    {
    }

    void VulkanTexture2D::Lock()
    {
    }

    void VulkanTexture2D::Unlock()
    {
    }

    void VulkanTexture2D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
    {
    }

    glm::ivec4 VulkanTexture2D::ReadPixel(uint32 row, uint32 column)
    {
        return glm::ivec4();
    }

    void VulkanTexture2D::UpdateResourceData(void *data)
    {
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

    /// =======================================================================================================================
    /// ============================================== Texture3D ==============================================================
    /// =======================================================================================================================

    VulkanTexture3D::VulkanTexture3D(const std::vector<HLString> &filepaths, bool flipOnLoad)
    {
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