#pragma once

#include "HighLoPch.h"
#include "engine/core/HLCore.h"

namespace highlo
{
	template<typename T>
	class List
	{
	private:

		struct Node
		{
			T m_Value;
			Node *m_Next;

			Node(T value)
				: m_Value(value), m_Next(nullptr) {}
		};

		Node *m_Root;
		uint32 m_Length;

	private:

		HLAPI void Append(Node *node, T value)
		{
			if (parent->m_Next == nullptr)
			{
				parent->m_Next = new Node(value);
				m_Length++;
			}
		else
			{
				Append(node->m_Next, value);
			}
		}

	public:

		List()
		{
			m_Root = nullptr;
			m_Length = 0;
		}

		~List()
		{

		}

		void Append(T value)
		{
			if (m_Root == nullptr)
			{
				m_Root = new Node(value);
				m_Length++;
			}
			else
			{
				Append(Root->m_Next, value);
			}
		}

		void Insert(T value)
		{
			Node *node = new Node(value);
			m_Length++;

			node->m_Next = node;
			m_Root = node;
		}

		T GetFirst()
		{
			return m_Root->m_Value;
		}

		T GetLast()
		{
			Node *current = m_Root;
			HL_ASSERT(current, "Root was nullptr!");

			while (current->m_Next != nullptr)
				{
				current = current->m_Next;
				}

			return current->m_Value;
		}

		T Get(int32 index)
		{
			if (index <= 0)
			{
				return GetFirst();
			}

			if (index >= m_Length)
			{
				return GetLast();
			}

			Node *current = m_Root;
			HL_ASSERT(current, "Root was nullptr!");
			T result = NULL;
			uint32 i = 0;

			while (current != nullptr)
			{
				if (i == index)
				{
					result = current->m_Value;
					break;
				}

				current = current->m_Next;
				i++;
			}

			return result;
		}

		void RemoveFirst()
		{
			Node *tmp = m_Root;
			HL_ASSERT(tmp, "Root was nullptr!");
			m_Root = m_Root->m_Next;

			delete tmp;
			tmp = nullptr;
			--m_Length;
		}

		void RemoveLast()
		{
			if (m_Length == 1)
			{
				delete m_Root;
				m_Root = nullptr;
				m_Length = 0;
				return;
			}

			--m_Length;
			Node *current = m_Root;
			HL_ASSERT(current, "Root was nullptr!");

			while (current != nullptr)
			{
				if (current->m_Next->m_Next == nullptr)
				{
					delete current->m_Next;
					current->m_Next = nullptr;
					break;
				}

				current = current->m_Next;
			}
		}

		void Remove(uint32 index)
		{
			if (0 == index)
			{
				RemoveFirst();
				return;
			}

			if (m_Length == index)
			{
				RemoveLast();
				return;
			}

			Node *current = m_Root;
			HL_ASSERT(current, "Root was nullptr!");
			uint32 i = 0;
			--m_Length;

			while (current != nullptr)
			{
				if ((i + 1) == index)
				{
					Node *tmp = current->m_Next;
					current->m_Next = current->m_Next->m_Next;

					delete tmp;
					tmp = nullptr;

					break;
				}
				current = current->m_Next;
				++i;
			}
		}

		void RemoveAll()
		{
			Clear();
			delete m_Root;
		}

		bool IsEmpty()
		{
			return m_Length == 0;
		}

		uint32 Size()
		{
			return m_Length;
		}

		void Clear()
		{
			if (m_Length > 0)
			{
				Node *current = m_Root;
				HL_ASSERT(current != nullptr, "Root was nullptr!");

				while (current != nullptr)
				{
					Node *tmp = current->m_Next;
					delete current;
					current = tmp;
				}

				m_Root = nullptr;
				m_Length = 0;
			}
		}

		T *ToArray()
		{
			T *arr = new T[m_Length];
			Node *current = m_Root;
			HL_ASSERT(current, "Root was nullptr!");
			uint32 i = 0;

			while (current != nullptr)
			{
				arr[i] = current->m_Value;
				++i;
				current = current->m_Next;
			}

			return arr;
		}

		void Print()
		{
			if (m_Length == 0)
			{
				HL_CORE_WARN("List was empty!");
				return;
			}

			Node *current = m_Root;
			HL_ASSERT(current, "Root was nullptr!");

			while (current != nullptr)
			{
				std::cout << current->m_Value << std::endl;
				current = current->m_Next;
			}
		}

		friend std::ostream &operator<<(std::ostream &stream, List<T> &list)
		{
			Node *current = list.m_Root;
			HL_ASSERT(current, "Root was nullptr!");
			uint32 i = 0;

			stream << "[";
			while (current != nullptr)
			{
				if ((i + 1) == list.Size())
				{
					stream << current->m_Value;
				}
				else
				{
					stream << current->m_Value << ",";
				}

				++i;
				current = current->m_Next;
			}
			stream << "]";
			return stream;
		}
	};
}