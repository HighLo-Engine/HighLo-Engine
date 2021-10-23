// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-10-21) initial release
//

#pragma once

#include "Thread.h"

namespace highlo
{
	class ThreadRegistry : public Singleton<ThreadRegistry>
	{
	public:

		HLAPI void Init();
		HLAPI void Shutdown();

		HLAPI Thread *GetThread(UUID uuid);
		HLAPI int32 Count();

		HLAPI void KillAll();

		HLAPI void Add(Thread *thread);
		HLAPI void Remove(Thread *thread);
	};
}

