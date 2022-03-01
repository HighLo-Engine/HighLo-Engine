// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Engine/Core/Atomic.h"

#ifdef HL_PLATFORM_WINDOWS

#include <Windows.h>

namespace highlo
{
	int32 Atomic::AtomicRead(int32 volatile *dst)
	{
		return (int32)_InterlockedCompareExchange((long volatile*)dst, 0, 0);
	}

	int64 Atomic::AtomicRead(int64 volatile *dst)
	{
		return _InterlockedCompareExchange64(dst, 0, 0);
	}

	void Atomic::AtomicStore(int32 volatile *dst, int32 value)
	{
		_InterlockedExchange((long volatile*)dst, value);
	}

	void Atomic::AtomicStore(int64 volatile *dst, int64 value)
	{
		_interlockedexchange64(dst, value);
	}

	int64 Atomic::InterlockedExchange64(int64 volatile *dst, int64 exchange)
	{
		return _interlockedexchange64(dst, exchange);
	}

	int64 Atomic::InterlockedExchange(int64 volatile *dst, int64 exchange, int64 comperand)
	{
		return _InterlockedCompareExchange64(dst, exchange, comperand);
	}

	int32 Atomic::InterlockedExchange(int32 volatile *dst, int32 exchange, int32 comperand)
	{
		return _InterlockedCompareExchange((long volatile*)dst, exchange, comperand);
	}
}

#endif // HL_PLATFORM_WINDOWS

