// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-10-21) initial release
//

#pragma once

#include "Engine/Core/Core.h"

namespace highlo
{
	class ThreadLocker : public IsSharedReference
	{
	public:

		HLAPI virtual void Lock() const = 0;
		HLAPI virtual bool TryLock() const = 0;
		HLAPI virtual void Unlock() const = 0;

		HLAPI static ThreadLocker *Create();
	};
}

