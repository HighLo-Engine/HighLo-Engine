// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-11-19) initial release
//

#pragma once

#include "Engine/Graphics/PhysicalDevice.h"

#include "Vulkan.h"

namespace highlo
{
	class VulkanPhysicalDevice : public PhysicalDevice
	{
	public:

		VulkanPhysicalDevice();
		virtual ~VulkanPhysicalDevice();

		virtual bool IsExtensionSupported(const HLString &name) const override;

	private:


	};
}

