// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Queue.h"

namespace highlo
{
	template<typename T>
	class BinarySearchTree
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

				return 1 + MAX(leftVal, rightVal);
			}
		};

		Node *Root;
		uint32 Length;

		void Swap(Node *node)
		{
			Node *temp = node->Left;
			node->Left = node->Right;
			node->Right = temp;
		}

	public:

		HLAPI BinarySearchTree()
		{
			Length = 0;
			Root = nullptr;
		}

		HLAPI ~BinarySearchTree()
		{
			RemoveAll();
			Length = 0;
		}

		HLAPI void Append(T value)
		{
			Node *node = new Node(value);
			Node *child, *parent = nullptr;
			++Length;

			child = Root;
			while (child != nullptr)
			{
				parent = child;

				if (node->Value == child->Value)
				{
					break;
				}
				else if (node->Value < child->Value)
				{
					child = child->Left;
				}
				else
				{
					child = child->Right;
				}
			}

			if (parent == nullptr)
			{
				Root = node;
			}
			else if (node->Value < parent->Value)
			{
				parent->Left = node;
			}
			else
			{
				parent->Right = node;
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

		HLAPI Node *Remove(T value) { return Remove(Root, value); }
		HLAPI Node *Remove(Node *node, T value)
		{
			if (!node || node == Root)
				return nullptr;

			if (value < node->Value)
			{
				node->Left = Remove(node->Left, value);
			}
			else if (value > node->Value)
			{
				node->Right = Remove(node->Right, value);
			}
			else
				{
				if (!node->Left)
				{
					// Node with only right child
					--Length;
					Node *temp = node->Right;
					delete node;
					return temp;
				}
				else if (!node->Right)
				{
					// Node with only left child
					--Length;
					Node *temp = node->Left;
					delete node;
					return temp;
				}

				Node *inorderSuccessor = FindMinNode(node->Right);
				node->Value = inorderSuccessor->Value;
				node->Right = Remove(node->Right, inorderSuccessor->Value);
			}

			return node;
		}

		HLAPI T FindMin() { return FindMin(Root); }
		HLAPI T FindMin(Node *node)
		{
			Node *current = node;
			while (current->Left)
			{
				current = current->Left;
			}

			return current->Value;
		}

		HLAPI T FindMax() { return FindMax(Root); }
		HLAPI T FindMax(Node *node)
		{
			Node *current = node;
			while (current->Right)
			{
				current = current->Right;
			}

			return current->Value;
		}

		HLAPI Node *FindMinNode() { return FindMinNode(Root); }
		HLAPI Node *FindMinNode(Node *node)
		{
			Node *current = node;
			while (current->Left)
			{
				current = current->Left;
			}

			return current;
		}

		HLAPI Node *FindMaxNode() { return FindMaxNode(Root); }
		HLAPI Node *FindMaxNode(Node *node)
		{
			Node *current = node;
			while (current->Right)
			{
				current = current->Right;
			}

			return current;
		}

		HLAPI void Invert() { Invert(Root); }
		HLAPI void Invert(Node *node)
		{
			if (!node)
				return;

			Swap(node);
			Invert(node->Left);
			Invert(node->Right);
		}

		HLAPI uint32 GetHeight() { return GetHeight(Root); }
		HLAPI uint32 GetHeight(Node *node)
		{
			if (node == nullptr)
				return 0;

			return node->CalcHeight();
		}

		HLAPI bool IsEmpty()
		{
			return Length == 0;
		}

		HLAPI uint32 Size()
		{
			return Length;
		}

		HLAPI void LevelOrder() { LevelOrder(Root); }
		HLAPI void LevelOrder(Node *node)
		{
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

