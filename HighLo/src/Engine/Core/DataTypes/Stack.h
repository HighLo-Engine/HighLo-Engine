// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

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
	struct HLStack
	{
	private:
		List<T> m_List;

	public:

		HLAPI HLStack() {}
		HLAPI ~HLStack() {}

		HLAPI void Push(T value)
		{
			m_List.Append(value);
		}

		HLAPI void Pop()
		{
			m_List.RemoveLast();
		}

		HLAPI T Top()
		{
			return m_List.GetLast();
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

		HLAPI friend std::ostream &operator<<(std::ostream &stream, HLStack<T> &stack)
		{
			while (!stack.IsEmpty())
			{
				T content = stack.Top();
				stack.Pop();
				stream << content << ", ";
			}

			return stream << "\n";
		}
	};
}

