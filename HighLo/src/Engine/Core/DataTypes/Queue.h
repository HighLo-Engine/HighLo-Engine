// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2022-03-24) removed comments
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "List.h"

namespace highlo
{
	template<typename T>
	class HLQueue
	{
	private:
		List<T> m_List;

	public:

		HLAPI HLQueue() {}
		HLAPI ~HLQueue() {}

		HLAPI void Enqueue(const T &value)
		{
			m_List.Append(value);
		}

		HLAPI void Dequeue()
		{
			m_List.RemoveFirst();
		}

		HLAPI T Front()
			{
			return m_List.GetFirst();
			}

		HLAPI bool IsEmpty()
		{
			return m_List.IsEmpty();
		}

		HLAPI uint32 Size()
		{
			return m_List.Size();
		}

		HLAPI void Clear()
		{
			m_List.Clear();
		}

		HLAPI void Print()
		{
			m_List.Print();
		}
		
		HLAPI friend std::ostream &operator<<(std::ostream &stream, HLQueue<T> &queue)
		{
			while (!queue.IsEmpty())
			{
				T content = queue.Front();
				queue.Dequeue();
				stream << content << ", ";
			}

			return stream << "\n";
		}
	};
}

