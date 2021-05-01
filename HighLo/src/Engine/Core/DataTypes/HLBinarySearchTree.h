#pragma once

#include "HLQueue.h"

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

			/**
			 *
			 * Calculates the Height of the current BST. A height is determined by the amount of levels a BST has.
			 *
			 * @return Returns the height of the current BST instance.
			 */
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

		BinarySearchTree()
		{
			Length = 0;
			Root = nullptr;
		}

		~BinarySearchTree()
		{
			RemoveAll();
			Length = 0;
		}

		void Append(T value)
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

		void RemoveAll() { RemoveAll(Root); }
		void RemoveAll(Node *node)
		{
			if (node == nullptr)
				return;

			RemoveAll(node->Left);
			RemoveAll(node->Right);

			delete node;
			node = nullptr;
		}

		Node *Remove(T value) { return Remove(Root, value); }
		Node *Remove(Node *node, T value)
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

		T FindMin() { return FindMin(Root); }
		T FindMin(Node *node)
		{
			Node *current = node;
			while (current->Left)
			{
				current = current->Left;
			}

			return current->Value;
		}

		T FindMax() { return FindMax(Root); }
		T FindMax(Node *node)
		{
			Node *current = node;
			while (current->Right)
			{
				current = current->Right;
			}

			return current->Value;
		}

		Node *FindMinNode() { return FindMinNode(Root); }
		Node *FindMinNode(Node *node)
		{
			Node *current = node;
			while (current->Left)
			{
				current = current->Left;
			}

			return current;
		}

		Node *FindMaxNode() { return FindMaxNode(Root); }
		Node *FindMaxNode(Node *node)
		{
			Node *current = node;
			while (current->Right)
			{
				current = current->Right;
			}

			return current;
		}

		void Invert() { Invert(Root); }
		void Invert(Node *node)
		{
			if (!node)
				return;

			Swap(node);
			Invert(node->Left);
			Invert(node->Right);
		}

		uint32 GetHeight() { return GetHeight(Root); }
		uint32 GetHeight(Node *node)
		{
			if (node == nullptr)
				return 0;

			return node->CalcHeight();
		}

		bool IsEmpty()
		{
			return Length == 0;
		}

		uint32 Size()
		{
			return Length;
		}

		void LevelOrder() { LevelOrder(Root); }
		void LevelOrder(Node *node)
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

		void PreOrder() { PreOrder(Root); }
		void PreOrder(Node *node)
		{
			if (node == nullptr)
				return;

			std::cout << node->Value << std::endl;
			PreOrder(node->Left);
			PreOrder(node->Right);
		}

		void InOrder() { InOrder(Root); }
		void InOrder(Node *node)
		{
			if (node == nullptr)
				return;

			InOrder(node->Left);
			std::cout << node->Value << std::endl;
			InOrder(node->Right);
		}

		void PostOrder() { PostOrder(Root); }
		void PostOrder(Node *node)
		{
			if (node == nullptr)
				return;

			PostOrder(node->Left);
			PostOrder(node->Right);
			std::cout << node->Value << std::endl;
		}
	};
}