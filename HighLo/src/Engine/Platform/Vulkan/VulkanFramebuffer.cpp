#include "HighLoPch.h"
#include "VulkanFramebuffer.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanUtils.h"

namespace highlo
{
    VulkanFramebuffer::VulkanFramebuffer(const FramebufferSpecification &spec)
        : m_Specification(spec)
    {
        
    }

    VulkanFramebuffer::~VulkanFramebuffer()
    {
    }
    
    void VulkanFramebuffer::Bind() const
    {
    }
    
    void VulkanFramebuffer::Unbind() const
    {
    }
    
    void VulkanFramebuffer::Resize(uint32 width, uint32 height, bool forceRecreate)
    {
    }
    
    void VulkanFramebuffer::AddResizeCallback(const std::function<void(Ref<Framebuffer>)> &func)
    {
        m_ResizeCallbacks.push_back(func);
    }
    
    void VulkanFramebuffer::BindTexture(uint32 attachmentIndex, uint32 slot) const
    {
    }
    
    void VulkanFramebuffer::ClearAttachment(uint32 attachmentIndex, int32 value)
    {
    }
    
    int32 VulkanFramebuffer::ReadPixel(uint32 attachmentIndex, int32 x, int32 y)
    {
        return 0;
    }
    
    Ref<Texture> VulkanFramebuffer::GetImage(uint32 attachmentIndex) const
    {
        HL_ASSERT(attachmentIndex < m_AttachmentImages.size());
        return m_AttachmentImages[attachmentIndex].As<Texture>();
    }
    
    Ref<Texture> VulkanFramebuffer::GetDepthImage() const
    {
        return m_DepthImage.As<Texture>();
    }
}

#endif // HIGHLO_API_VULKAN

