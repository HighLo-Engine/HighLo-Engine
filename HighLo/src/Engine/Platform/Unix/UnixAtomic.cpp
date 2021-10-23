// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Engine/Core/Atomic.h"

#ifdef HL_PLATFORM_UNIX

namespace highlo
{
	int32 Atomic::AtomicRead(int32 volatile *dst)
	{
		int32 result;
		__atomic_load(dst, &result, __ATOMIC_SEQ_CST);
		return result;
	}

	int64 Atomic::AtomicRead(int64 volatile *dst)
	{
		int64 result;
		__atomic_load(dst, &result, __ATOMIC_SEQ_CST);
		return result;
	}

	void Atomic::AtomicStore(int32 volatile *dst, int32 value)
	{
		__atomic_store(dst, &value, __ATOMIC_SEQ_CST);
	}

	void Atomic::AtomicStore(int64 volatile *dst, int64 value)
	{
		__atomic_store(dst, &value, __ATOMIC_SEQ_CST);
	}

	int64 Atomic::InterlockedExchange64(int64 volatile *dst, int64 exchange)
	{
		return __sync_lock_test_and_set(dst, exchange);
	}

	int64 Atomic::InterlockedExchange(int64 volatile *dst, int64 exchange, int64 comperand)
	{
		return __sync_val_compare_and_swap(dst, exchange, comperand);
	}

	int32 Atomic::InterlockedExchange(int32 volatile *dst, int32 exchange, int32 comperand)
	{
		return __sync_val_compare_and_swap(dst, exchange, comperand);
	}
}

#endif // HL_PLATFORM_UNIX