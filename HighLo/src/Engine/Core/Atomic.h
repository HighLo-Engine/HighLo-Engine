// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-10-22) initial release
//

#pragma once

#include "Engine/Core/Core.h"

// undefine Windows macro
#undef InterlockedExchange
#undef InterlockedExchange64

namespace highlo
{
	class Atomic
	{
	public:

		HLAPI static int32 AtomicRead(int32 volatile *dst);
		HLAPI static int64 AtomicRead(int64 volatile *dst);
		HLAPI static void AtomicStore(int32 volatile *dst, int32 value);
		HLAPI static void AtomicStore(int64 volatile *dst, int64 value);

		HLAPI static int64 InterlockedExchange64(int64 volatile *dst, int64 exchange);
		HLAPI static int64 InterlockedExchange(int64 volatile *dst, int64 exchange, int64 comperand);
		HLAPI static int32 InterlockedExchange(int32 volatile *dst, int32 exchange, int32 comperand);
	};
}

