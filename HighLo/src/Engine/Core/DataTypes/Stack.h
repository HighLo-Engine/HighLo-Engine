// Copyright (c) 2021 Albert Slepak and Can Karka. All rights reserved.

//
// version history:
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

		/**
		 *
		 * Appends a new element to the stack.
		 *
		 * @param value The value which should be added to the stack.
		 */
		HLAPI void Push(T value)
		{
			m_List.Append(value);
		}

		/**
		 *
		 * Removes the current element from the stack.
		 *
		 */
		HLAPI void Pop()
		{
			m_List.RemoveLast();
		}

		/**
		 *
		 * Returns the current element from the stack.
		 *
		 * @return Returns the current element from the stack.
		 */
		HLAPI T Top()
		{
			return m_List.GetLast();
		}

		/**
		 *
		 * Returns true if the stack is empty.
		 *
		 * @return Returns true if the stack is empty.
		 */
		HLAPI bool IsEmpty()
		{
			return m_List.IsEmpty();
		}

		/**
		 *
		 * Returns the size of the current stack.
		 *
		 * @return Returns the size of the current stack.
		 */
		HLAPI uint32 Size()
		{
			return m_List.Size();
		}

		/**
		 *
		 * Clears the Stack.
		 *
		 */
		HLAPI void Clear()
		{
			m_List.Clear();
		}

		/**
		 *
		 * Prints the Stack to the console.
		 *
		 */
		HLAPI void Print()
		{
			m_List.Print();
		}

		/**
		 *
		 * Writes the hole Stack into the output stream.
		 *
		 * @param stream The stream, which should be filled with the content of the Stack.
		 * @param stack The Stack, which should be written to the stream.
		 * @return Returns the stream, which was filled with the content of the Stack.
		 *
		 */
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