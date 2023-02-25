// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-10-22) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Atomic.h"
#include "Thread.h"

namespace highlo
{
	template<typename T, int32 MaxThreads = HL_PLATFORM_THREADS_LIMIT, bool ClearMemory = true>
	class LocalThread
	{
	private:

		struct Node
		{
			volatile int64 ThreadID;
			T Value;
		};

		Node m_Nodes[MaxThreads];

	public:

		HLAPI LocalThread()
		{
			if (ClearMemory)
			{
				memset(m_Nodes, 0, sizeof(m_Nodes));
			}
			else
			{
				for (int32 i = 0; i < MaxThreads; ++i)
					m_Nodes[i].ThreadID = 0;
			}
		}

		HLAPI T &Get()
		{
			return m_Nodes[GetIndex()].Value;
		}

		HLAPI void Set(const T &value)
		{
			m_Nodes[GetIndex()].Value = value;
		}

		HLAPI int32 Count() const
		{
			int32 result = 0;

			for (int32 i = 0; i < MaxThreads; ++i)
			{
				if (Atomic::AtomicRead((int64 volatile*)&m_Nodes[i].ThreadID) != 0)
					++result;
			}

			return result;
		}

	protected:

		HL_FORCE_INLINE static int32 Hash(const int64 value)
		{
			return value & (MaxThreads - 1);
		}

		HL_FORCE_INLINE int32 GetIndex()
		{
			int64 key = (int64)Thread::GetCurrentThreadID();
			auto index = Hash(key);

			while (1)
			{
				const int64 value = Atomic::AtomicRead(&m_Nodes[index].ThreadID);
				if (value == key)
					break;

				if (value == 0 && Atomic::InterlockedExchange(&m_Nodes[index].ThreadID, key, 0) == 0)
					break;

				index = Hash(index + 1);
			}

			return index;
		}
	};
}

