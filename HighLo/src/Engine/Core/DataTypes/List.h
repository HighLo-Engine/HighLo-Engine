#pragma once

#include "HighLoPch.h"
#include "Engine/Core/Core.h"
#include "Engine/Core/Log.h"
#include "Engine/Core/Assert.h"

namespace highlo
{
	template<typename T>
	class List
	{
	private:

		struct Node
		{
			T Value;
			Node *Next;

			Node(T value)
				: Value(value), Next(nullptr) {}
		};

		Node *m_Root;
		uint32 m_Length;

	private:

		HLAPI void Append(Node *node, const T &value)
		{
			if (node->Next == nullptr)
			{
				node->Next = new Node(value);
				m_Length++;
			}
			else
			{
				Append(node->Next, value);
			}
		}

	public:

		HLAPI List()
		{
			m_Root = nullptr;
			m_Length = 0;
		}

		HLAPI ~List()
		{
		}

		HLAPI void Append(const T &value)
		{
			if (m_Root == nullptr)
			{
				m_Root = new Node(value);
				m_Length++;
			}
			else
			{
				Append(m_Root, value);
			}
		}

		HLAPI void Insert(const T &value)
		{
			Node *node = new Node(value);
			m_Length++;

			node->Next = node;
			m_Root = node;
		}

		HLAPI T GetFirst()
		{
			return m_Root->Value;
		}

		HLAPI T GetLast()
		{
			Node *current = m_Root;
			HL_ASSERT(current, "Root was nullptr!");

			while (current->Next != nullptr)
				{
				current = current->Next;
				}

			return current->Value;
		}

		HLAPI T Get(uint32 index)
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
			uint32 i = 0;

			while (current != nullptr)
			{
				if (i == index)
				{
					return current->Value;
				}

				current = current->Next;
				++i;
			}

			return {};
		}

		HLAPI void RemoveFirst()
		{
			Node *tmp = m_Root;
			HL_ASSERT(tmp, "Root was nullptr!");
			m_Root = m_Root->Next;

			delete tmp;
			tmp = nullptr;
			--m_Length;
		}

		HLAPI void RemoveLast()
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
				if (current->Next->Next == nullptr)
				{
					delete current->Next;
					current->Next = nullptr;
					break;
				}

				current = current->Next;
			}
		}

		HLAPI void Remove(uint32 index)
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
					Node *tmp = current->Next;
					current->Next = current->Next->Next;

					delete tmp;
					tmp = nullptr;

					break;
				}
				current = current->Next;
				++i;
			}
		}

		HLAPI bool IsEmpty()
		{
			return m_Length == 0;
		}

		HLAPI uint32 Size()
		{
			return m_Length;
		}

		HLAPI void Clear()
		{
			if (m_Length > 0)
			{
				Node *current = m_Root;
				HL_ASSERT(current != nullptr, "Root was nullptr!");

				while (current != nullptr)
				{
					Node *tmp = current->Next;
					delete current;
					current = tmp;
				}

				m_Root = nullptr;
				m_Length = 0;
			}
		}

		HLAPI T *ToArray()
		{
			T *arr = new T[m_Length];
			Node *current = m_Root;
			HL_ASSERT(current, "Root was nullptr!");
			uint32 i = 0;

			while (current != nullptr)
			{
				arr[i] = current->Value;
				++i;
				current = current->Next;
			}

			return arr;
		}

		HLAPI void Print()
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
				std::cout << current->Value << std::endl;
				current = current->Next;
			}
		}

		HLAPI friend std::ostream &operator<<(std::ostream &stream, List<T> &list)
		{
			Node *current = list.m_Root;
			HL_ASSERT(current, "Root was nullptr!");
			uint32 i = 0;

			stream << "[";
			while (current != nullptr)
			{
				if ((i + 1) == list.Size())
				{
					stream << current->Value;
				}
				else
				{
					stream << current->Value << ",";
				}

				++i;
				current = current->Next;
			}
			stream << "]";
			return stream;
		}
	};
}
