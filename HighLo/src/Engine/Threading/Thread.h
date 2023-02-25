// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-10-21) initial release
//

#pragma once

#include "Runnable.h"
#include "Engine/Core/UUID.h"

namespace highlo
{
	enum class ThreadPriority
	{
		None = 0,
		Lowest,
		BelowNormal,
		Normal,
		AboveNormal,
		Highest
	};

	class Thread : public IsSharedReference
	{
	public:

		HLAPI virtual int32 Run() = 0;
		HLAPI virtual bool Start(uint32 stackSize) = 0;
		HLAPI virtual void Kill(bool waitForJoin = false) = 0;
		HLAPI virtual void Join() = 0;
		HLAPI virtual void SetPriority(ThreadPriority priority) = 0;

		HLAPI virtual bool IsRunning() const = 0;
		HLAPI virtual ThreadPriority GetPriority() = 0;
		HLAPI virtual UUID GetID() const = 0;
		HLAPI virtual const HLString &GetName() const = 0;

		HLAPI static bool IsInMainThread();
		HLAPI static uint64 GetCurrentThreadID();
		HLAPI static Ref<Thread> Create(Runnable *runnable, const HLString &name, ThreadPriority priority, uint32 stackSize);
	};
}

