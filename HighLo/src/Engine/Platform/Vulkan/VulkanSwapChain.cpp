#include "HighLoPch.h"
#include "VulkanSwapChain.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
    VulkanSwapChain::VulkanSwapChain()
    {
    }

    VulkanSwapChain::~VulkanSwapChain()
    {
    }
    
    void VulkanSwapChain::Init(const Ref<RenderingContext> &context)
    {
    }
    
    void VulkanSwapChain::InitSurface(void *windowHandle)
    {
    }
    
    void VulkanSwapChain::Create(uint32 *width, uint32 *height, bool vsync)
    {
    }
    
    void VulkanSwapChain::Cleanup()
    {
    }
    
    void VulkanSwapChain::OnResize(uint32 width, uint32 height)
    {
    }
    
    void VulkanSwapChain::BeginFrame()
    {
    }
    
    void VulkanSwapChain::EndFrame()
    {
    }
    
    void VulkanSwapChain::Present()
    {
    }
    
    uint32 VulkanSwapChain::GetImageCount() const
    {
        return 0;
    }
    
    uint32 VulkanSwapChain::GetWidth() const
    {
        return 0;
    }
    
    uint32 VulkanSwapChain::GetHeight() const
    {
        return 0;
    }
    
    uint32 VulkanSwapChain::GetCurrentBufferIndex() const
    {
        return 0;
    }
}

#endif // HIGHLO_API_VULKAN

