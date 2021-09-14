// Copyright (c) 2021 Albert Slepak and Can Karka. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Queue.h"
#include "Engine/Core/Core.h"

namespace highlo
{
	template<typename T>
	class BinaryTree
	{
	private:

		struct Node
		{
			T Value;
			Node *Left, *Right;

			Node(T value)
				: Value(value), Left(nullptr), Right(nullptr) {}

			uint32 CalcHeight()
			{
				uint32 leftVal = 0, rightVal = 0;

				if (this->Left)
					leftVal = this->Left->CalcHeight();

				if (this->Right)
					rightVal = this->Right->CalcHeight();

				return 1 + HL_MAX(leftVal, rightVal);
			}
		};

		Node *Root;
		uint32 Length;

		void RemoveDeepestNode(Node *node, Node *dNode)
		{
			HLQueue<Node*> q;
			q.Enqueue(node);

			Node *temp;
			while (!q.IsEmpty())
			{
				temp = q.Front();
				q.Dequeue();

				if (temp == dNode)
				{
					delete temp;
					temp = nullptr;
					--Length;
					return;
				}

				if (temp->Right)
				{
					if (temp->Right == dNode)
					{
						delete temp->Right;
						temp->Right = nullptr;
						--Length;
						return;
					}
					else
					{
						q.Enqueue(temp->Right);
					}
				}

				if (temp->Left)
				{
					if (temp->Left == dNode)
					{
						delete temp->Left;
						temp->Left = nullptr;
						--Length;
						return;
					}
					else
					{
						q.Enqueue(temp->Left);
					}
				}
			}
		}

		void Swap(Node *node)
		{
			Node *temp = node->Left;
			node->Left = node->Right;
			node->Right = temp;
		}

	public:

		HLAPI BinaryTree()
		{
			Root = nullptr;
			Length = 0;
		}

		HLAPI ~BinaryTree()
		{
			RemoveAll();
			Length = 0;
		}

		HLAPI void Append(T value) { Append(Root, value); }
		HLAPI void Append(Node *node, T value)
		{
			if (Root == nullptr)
			{
				Root = new Node(value);
				++Length;
				return;
			}

			HLQueue<Node *> q;
			q.Enqueue(node);

			while (!q.IsEmpty())
			{
				Node *temp = q.Front();
				q.Dequeue();

				if (!temp->Left)
				{
					temp->Left = new Node(value);
					++Length;
					break;
				}
				else
				{
					q.Enqueue(temp->Left);
				}

				if (!temp->Right)
				{
					temp->Right = new Node(value);
					++Length;
					break;
				}
				else
				{
					q.Enqueue(temp->Right);
				}
			}
		}

		HLAPI void RemoveAll() { RemoveAll(Root); }
		HLAPI void RemoveAll(Node *node)
		{
			if (node == nullptr)
				return;

			RemoveAll(node->Left);
			RemoveAll(node->Right);

			delete node;
			node = nullptr;
		}

		HLAPI Node *Remove(T value) { Remove(Root, value); }
		HLAPI Node *Remove(Node *node, T value)
		{
			if (node == nullptr)
				return nullptr;

			if (node->Left == nullptr && node->Right == nullptr)
			{
				if (node->Value == value)
				{
					return nullptr;
				}
				else
				{
					return node;
				}
			}

			HLQueue<Node*> q;
			q.Enqueue(node);

			Node *temp;
			Node *keyNode = nullptr;

			while (!q.IsEmpty())
			{
				temp = q.Front();
				q.Dequeue();

				if (temp->Value == value)
					keyNode = temp;

				if (temp->Left)
					q.Enqueue(temp->Left);

				if (temp->Right)
					q.Enqueue(temp->Right);
			}

			if (keyNode == nullptr)
			{
				T x = temp->Value;
				RemoveDeepestNode(node, temp);
				keyNode->Value = x;
			}

			return node;
		}

		HLAPI Node *GetNode(T value) { return GetNode(Root, value); }
		HLAPI Node *GetNode(Node *node, T value)
		{
			if (!node)
				return nullptr;

			HLQueue<Node*> q;
			q.Enqueue(node);

			while (!q.IsEmpty())
			{
				Node *current = q.Front();
				q.Dequeue();

				if (current->Left)
					q.Enqueue(current->Left);

				if (current->Right)
					q.Enqueue(current->Right);

				if (current->Value == value)
					return current;
			}

			return nullptr;
		}

		HLAPI void Invert() { Invert(Root); }
		HLAPI void Invert(Node *node)
		{
			if (node == nullptr)
				return;

			Swap(node);
			Invert(node->Left);
			Invert(node->Right);
		}

		HLAPI T FindMin() { return FindMin(Root); }
		HLAPI T FindMin(Node *node)
		{
			HLQueue<Node*> q;
			q.Enqueue(node);

			T min = fake_max_value<T>();
			while (!q.IsEmpty())
			{
				Node *current = q.Front();
				q.Dequeue();

				if (current->Left)
					q.Enqueue(current->Left);

				if (current->Right)
					q.Enqueue(current->Right);

				if (current->Value < min)
					min = current->Value;
			}

			return min;
		}

		HLAPI T FindMax() { return FindMax(Root); }
		HLAPI T FindMax(Node *node)
		{
			HLQueue<Node*> q;
			q.Enqueue(node);

			T max = fake_min_value<T>();
			while (!q.IsEmpty())
			{
				Node *current = q.Front();
				q.Dequeue();

				if (current->Left)
					q.Enqueue(current->Left);

				if (current->Right)
					q.Enqueue(current->Right);

				if (current->Value > max)
					max = current->Value;
			}

			return max;
		}

		HLAPI Node *FindMinNode() { return FindMinNode(Root); }
		HLAPI Node *FindMinNode(Node *node)
		{
			HLQueue<Node*> q;
			q.Enqueue(node);

			T min = fake_max_value<T>();
			Node *ret = nullptr;
			while (!q.IsEmpty())
			{
				Node *current = q.Front();
				q.Dequeue();

				if (current->Left)
					q.Enqueue(current->Left);

				if (current->Right)
					q.Enqueue(current->Right);

				if (current->Value < min)
				{
					min = current->Value;
					ret = current;
				}
			}

			return ret;
		}

		HLAPI Node *FindMaxNode() { return FindMaxNode(Root); }
		HLAPI Node *FindMaxNode(Node *node)
		{
			HLQueue<Node*> q;
			q.Enqueue(node);

			T max = fake_min_value<T>();
			Node *ret = nullptr;
			while (!q.IsEmpty())
			{
				Node *current = q.Front();
				q.Dequeue();

				if (current->Left)
					q.Enqueue(current->Left);

				if (current->Right)
					q.Enqueue(current->Right);

				if (current->Value > max)
				{
					max = current->Value;
					ret = current;
				}
			}

			return ret;
		}

		HLAPI bool ValueExists(T value) { return ValueExists(Root, value); }
		HLAPI bool ValueExists(Node *node, T value)
		{
			if (!node)
				return false;

			HLQueue<Node*> q;
			q.Enqueue(node);

			while (!q.IsEmpty())
			{
				Node *current = q.Front();
				q.Dequeue();

				if (current->Left)
					q.Enqueue(current->Left);

				if (current->Right)
					q.Enqueue(current->Right);

				if (current->Value == value)
					return true;
			}

			return false;
		}

		HLAPI bool IsEmpty()
		{
			return Length == 0;
		}

		HLAPI uint32 Size()
		{
			return Length;
		}

		HLAPI uint32 GetHeight() { return GetHeight(Root); }
		HLAPI uint32 GetHeight(Node *node)
		{
			if (node == nullptr)
				return 0;

			return node->CalcHeight();
		}

		HLAPI void LevelOrder() { LevelOrder(Root); }
		HLAPI void LevelOrder(Node *node)
		{
			if (node == nullptr)
				return;

			HLQueue<Node*> q;
			q.Enqueue(node);

			while (!q.IsEmpty())
			{
				Node *current = q.Front();
				std::cout << current->Value << std::endl;

				if (current->Left)
					q.Enqueue(current->Left);

				if (current->Right)
					q.Enqueue(current->Right);

				q.Dequeue();
			}
		}

		HLAPI void PreOrder() { PreOrder(Root); }
		HLAPI void PreOrder(Node *node)
		{
			if (node == nullptr)
				return;

			std::cout << node->Value << std::endl;
			PreOrder(node->Left);
			PreOrder(node->Right);
		}
		
		HLAPI void InOrder() { InOrder(Root); }
		HLAPI void InOrder(Node *node)
		{
			if (node == nullptr)
				return;

			InOrder(node->Left);
			std::cout << node->Value << std::endl;
			InOrder(node->Right);
		}

		HLAPI void PostOrder() { PostOrder(Root); }
		HLAPI void PostOrder(Node *node)
		{
			if (node == nullptr)
				return;

			PostOrder(node->Left);
			PostOrder(node->Right);
			std::cout << node->Value << std::endl;
		}
	};
}
