#pragma once

#include "HLList.h"

namespace highlo
{
	template<typename T>
	struct HLStack
	{
	private:
		List<T> m_List;

	public:

		HLStack() {}
		~HLStack() {}

		/**
		 *
		 * Appends a new element to the stack.
		 *
		 * @param value The value which should be added to the stack.
		 */
		void Push(T value)
		{
			m_List.Append(value);
		}

		/**
		 *
		 * Removes the current element from the stack.
		 *
		 */
		void Pop()
		{
			m_List.RemoveLast();
		}

		/**
		 *
		 * Returns the current element from the stack.
		 *
		 * @return Returns the current element from the stack.
		 */
		T Top()
		{
			return m_List.GetLast();
		}

		/**
		 *
		 * Returns true if the stack is empty.
		 *
		 * @return Returns true if the stack is empty.
		 */
		bool IsEmpty()
		{
			return m_List.IsEmpty();
		}

		/**
		 *
		 * Returns the size of the current stack.
		 *
		 * @return Returns the size of the current stack.
		 */
		uint32 Size()
		{
			return m_List.Size();
		}

		/**
		 *
		 * Clears the Stack.
		 *
		 */
		void Clear()
		{
			m_List.Clear();
		}

		/**
		 *
		 * Prints the Stack to the console.
		 *
		 */
		void Print()
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
		friend std::ostream &operator<<(std::ostream &stream, HLStack<T> &stack)
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