#pragma once

#include "HLList.h"

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

		/**
		 *
		 * Appends a new item to the Queue.
		 *
		 * @param value The Value which should be added to the Queue.
		 */
		HLAPI void Enqueue(const T &value)
		{
			m_List.Append(value);
		}

		/**
		 *
		 * Removes the last element from the Queue.
		 *
		 */
		HLAPI void Dequeue()
		{
			m_List.RemoveFirst();
		}

		/**
		 *
		 * Returns the current element from the Queue.
		 *
		 * @return Returns the current element from the Queue.
		 */
		HLAPI T Front()
		{
			return m_List.GetFirst();
		}

		/**
		 *
		 * Returns true if the Queue is empty.
		 *
		 * @return Returns true if the Queue is empty.
		 */
		HLAPI bool IsEmpty()
		{
			return m_List.IsEmpty();
		}

		/**
		 *
		 * Returns the size of the current Queue.
		 *
		 * @return Returns the size of the current Queue.
		 */
		HLAPI uint32 Size()
		{
			return m_List.Size();
		}

		/**
		 *
		 * Clears the Queue.
		 *
		 */
		HLAPI void Clear()
		{
			m_List.Clear();
		}

		/**
		 *
		 * Prints the Queue to the console.
		 *
		 */
		HLAPI void Print()
		{
			m_List.Print();
		}

		/**
		 *
		 * Writes the hole Queue into the output stream.
		 *
		 * @param stream The stream, which should be filled with the content of the Queue.
		 * @param queue The Queue, which should be written to the stream.
		 * @return Returns the stream, which was filled with the content of the Queue.
		 */
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