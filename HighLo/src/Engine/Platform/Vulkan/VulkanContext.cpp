// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanContext.h"

#include "Engine/Window/Window.h"

namespace highlo
{
    VulkanContext::VulkanContext(void *handle, WindowData &data)
        : m_NativeWindowHandle(handle)
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

