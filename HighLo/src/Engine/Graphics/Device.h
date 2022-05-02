// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-04-04) initial release
//

#pragma once

#include "Engine/Graphics/PhysicalDevice.h"

namespace highlo
{
	class Device : public IsSharedReference
	{
	public:

		HLAPI virtual ~Device() {}

		HLAPI virtual void Destroy() = 0;

		HLAPI static Ref<Device> Create(const Ref<PhysicalDevice> &physicalDevice);
	};
}

