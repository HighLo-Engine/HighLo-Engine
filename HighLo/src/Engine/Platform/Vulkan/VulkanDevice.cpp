// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanDevice.h"

namespace highlo
{
	VulkanDevice::VulkanDevice(const Ref<PhysicalDevice> &physicalDevice)
		: m_PhysicalDevice(physicalDevice)
	{
	}

	VulkanDevice::~VulkanDevice()
	{
	}

	void VulkanDevice::Destroy()
	{
	}
}

