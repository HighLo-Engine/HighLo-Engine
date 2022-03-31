#include "HighLoPch.h"
#include "VulkanContext.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
    VulkanContext::VulkanContext(void *handle, WindowData &data)
        : m_VulkanWindowHandle(handle)
    {
    }

    VulkanContext::~VulkanContext()
    {
    }
    
    void VulkanContext::Init()
    {
    }
    
    void VulkanContext::SwapBuffers()
    {
    }
    
    void VulkanContext::MakeCurrent()
    {
    }
    
    void VulkanContext::SetSwapInterval(bool bEnabled)
    {
    }
    
    void *VulkanContext::GetCurrentContext()
    {
        return nullptr;
    }
}

#endif // HIGHLO_API_VULKAN

