#include "HighLoPch.h"
#include "VulkanTexture3D.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanUtils.h"

namespace highlo
{
    VulkanTexture3D::VulkanTexture3D(const FileSystemPath &filePath, bool flipOnLoad)
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
        return Allocator();
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
        return uint32();
    }
    
    std::pair<uint32, uint32> VulkanTexture3D::GetMipSize(uint32 mip)
    {
        return std::pair<uint32, uint32>();
    }
    
    void VulkanTexture3D::GenerateMips()
    {
    }
    
    void VulkanTexture3D::Bind(uint32 slot) const
    {
    }
    
    void VulkanTexture3D::Unbind(uint32 slot) const
    {
    }
}

#endif // HIGHLO_API_VULKAN

