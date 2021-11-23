// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-23) initial release
//

#pragma once

namespace highlo
{
	class SwapChain : public IsSharedReference
	{
	public:

		HLAPI virtual ~SwapChain() = default;



		HLAPI static Ref<SwapChain> Create();
	};
}

