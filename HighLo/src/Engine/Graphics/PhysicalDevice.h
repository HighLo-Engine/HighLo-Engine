// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-04-04) initial release
//

#pragma once

namespace highlo
{
	class PhysicalDevice : public IsSharedReference
	{
	public:

		HLAPI virtual ~PhysicalDevice() {}

		HLAPI virtual bool IsExtensionSupported(const HLString &name) const = 0;

		HLAPI static Ref<PhysicalDevice> Create();
	};
}

