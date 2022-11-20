// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanPhysicalDevice.h"

namespace highlo
{
    VulkanPhysicalDevice::VulkanPhysicalDevice()
    {
    }

    VulkanPhysicalDevice::~VulkanPhysicalDevice()
    {
    }

    bool VulkanPhysicalDevice::IsExtensionSupported(const HLString &name) const
    {
        return false;
    }
}

