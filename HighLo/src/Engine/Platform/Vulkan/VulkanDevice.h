// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-11-19) initial release
//

#pragma once

#include "Engine/Graphics/Device.h"

#include "Vulkan.h"

namespace highlo
{
	class VulkanDevice : public Device
	{
	public:

		VulkanDevice(const Ref<PhysicalDevice> &physicalDevice);
		virtual ~VulkanDevice();

		virtual void Destroy() override;

	private:

		Ref<PhysicalDevice> m_PhysicalDevice = nullptr;
	};
}

