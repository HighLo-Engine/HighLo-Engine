#include "HighLoPch.h"
#include "VulkanTexture2D.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanUtils.h"

namespace highlo
{
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
    
    VulkanTexture2D::VulkanTexture2D(const TextureSpecification &spec)
    {
    }
    
    VulkanTexture2D::~VulkanTexture2D()
    {
    }
    
    Allocator VulkanTexture2D::GetData()
    {
        return Allocator();
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
    
    void VulkanTexture2D::CreatePerLayerImageViews()
    {
    }
    
    void VulkanTexture2D::CreateSampler(TextureProperties properties)
    {
    }
    
    void VulkanTexture2D::UpdateResourceData()
    {
    }
    
    void VulkanTexture2D::UpdateResourceData(void *data)
    {
    }
    
    void VulkanTexture2D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
    {
    }
    
    glm::ivec4 VulkanTexture2D::ReadPixel(uint32 row, uint32 column)
    {
        return glm::ivec4();
    }
    
    uint32 VulkanTexture2D::GetMipLevelCount()
    {
        return uint32();
    }
    
    std::pair<uint32, uint32> VulkanTexture2D::GetMipSize(uint32 mip)
    {
        return std::pair<uint32, uint32>();
    }
    
    void VulkanTexture2D::GenerateMips()
    {
    }
    
    void VulkanTexture2D::Bind(uint32 slot) const
    {
    }
    
    void VulkanTexture2D::Unbind(uint32 slot) const
    {
    }

    // Vulkan-specific
    VkImageView VulkanTexture2D::GetLayerImageView(uint32 layer)
    {
        return VkImageView();
    }
    
    VkImageView VulkanTexture2D::GetMipImageView(uint32 mip)
    {
        return VkImageView();
    }
}

#endif // HIGHLO_API_VULKAN

