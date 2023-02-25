// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Device.h"

#ifdef HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanDevice.h"
#endif // HIGHLO_API_VULKAN

namespace highlo
{
	Ref<Device> Device::Create(const Ref<PhysicalDevice> &physicalDevice)
	{
	#ifdef HIGHLO_API_VULKAN
		return Ref<VulkanDevice>::Create(physicalDevice);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_VULKAN
	}
}

