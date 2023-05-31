/**************************************************************************************************
* MIT License
* 
* Copyright (c) 2023 Nick Wettstein (@Schmicki)
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
**************************************************************************************************/

#pragma once

#include "Aliases.hpp"

template <class T>
struct RBTree
{
public:

	struct Node
	{
		Node* left;
		Node* right;
		U8 color;
		T key;

		Node(const T& in_key)
			: left(NULL), right(NULL), color(RED), key(in_key)
		{
		}
	};

	enum Color {
		BLACK = 0, RED = 1
	};

	enum Postition {
		ROOT = 0, LEFT = 1, RIGHT = 2
	};

	static void Swap(Node* in_a, Node* in_b)
	{
		Node tmp = *in_a;
		*in_a = *in_b;
		*in_b = tmp;
	}

	static void SwapChildPtr(Node* in_parent, Node* in_old_child,
		Node* in_new_child)
	{
		if (in_parent->left == in_old_child)
		{
			in_parent->left = in_new_child;
			return;
		}
		else
		{
			in_parent->right = in_new_child;
			return;
		}
	}

	static void RotateLeft(Node** io_root, Node* in_parent,
		Node* in_node)
	{
		Node* right = in_node->right;
		in_node->right = in_node->right->left;
		right->left = in_node;

		if (in_parent == NULL)
		{
			*io_root = right;
			return;
		}

		SwapChildPtr(in_parent, in_node, right);
	}

	static void RotateRight(Node** io_root, Node* in_parent,
		Node* in_node)
	{
		Node* left = in_node->left;
		in_node->left = in_node->left->right;
		left->right = in_node;

		if (in_parent == NULL)
		{
			*io_root = left;
			return;
		}

		SwapChildPtr(in_parent, in_node, left);
	}

	static void Add(Node** io_root, Node* in_node)
	{
		Node* node = *io_root;
		Node* stack[32];
		U32 stackSize = 0;
		U32 position = ROOT;

		// search
		while (node != NULL)
		{
			if (node->key < in_node->key)
			{
				stack[stackSize++] = node;
				node = node->right;
				position = RIGHT;
				continue;
			}
			else if (in_node->key < node->key)
			{
				stack[stackSize++] = node;
				node = node->left;
				position = LEFT;
				continue;
			}

			// found node
			return;
		}

		node = in_node;
		node->color = RED;
		node->left = NULL;
		node->right = NULL;

		// insert
		switch (position)
		{
		case ROOT:
			*io_root = node;
			break;
		case LEFT:
			stack[stackSize - 1]->left = node;
			break;
		case RIGHT:
			stack[stackSize - 1]->right = node;
			break;
		}

		// fix
		while (true)
		{
			if (stackSize == 0)
			{
				// root
				node->color = BLACK;
				return;
			}

			Node* parent = stack[stackSize - 1];

			if (parent->color == BLACK)
				return;

			Node* grandParent = stack[stackSize - 2];

			if (grandParent->left == parent)
			{
				// left
				Node* uncle = grandParent->right;

				if (uncle != NULL && uncle->color == RED)
				{
					// red uncle
					uncle->color = BLACK;
					parent->color = BLACK;
					grandParent->color = RED;
					node = grandParent;
					stackSize -= 2;
					continue;
				}

				if (parent->right == node)
				{
					// black uncle, left, right
					RotateLeft(NULL, grandParent, parent);
					RotateRight(io_root,
						(stackSize != 2) ? stack[stackSize - 3] : NULL,
						grandParent);
					node->color = BLACK;
					grandParent->color = RED;
					return;
				}
				else
				{
					// black uncle, left, left
					RotateRight(io_root,
						(stackSize != 2) ? stack[stackSize - 3] : NULL,
						grandParent);
					grandParent->color = RED;
					parent->color = BLACK;
					return;
				}
			}
			else
			{
				// right
				Node* uncle = grandParent->left;

				if (uncle != NULL && uncle->color == RED)
				{
					// red uncle
					uncle->color = BLACK;
					parent->color = BLACK;
					grandParent->color = RED;
					node = grandParent;
					stackSize -= 2;
					continue;
				}

				if (parent->right == node)
				{
					// black uncle, right, right
					RotateLeft(io_root,
						(stackSize != 2) ? stack[stackSize - 3] : NULL,
						grandParent);
					grandParent->color = RED;
					parent->color = BLACK;
					return;
				}
				else
				{
					// black uncle, right, left
					RotateRight(NULL, grandParent, parent);
					RotateLeft(io_root,
						(stackSize != 2) ? stack[stackSize - 3] : NULL,
						grandParent);
					node->color = BLACK;
					grandParent->color = RED;
					return;
				}
			}
		}
	}

	static Node* Remove(Node** io_root, const T& in_key)
	{
		Node* node = *io_root;
		Node* stack[32];
		U32 stackSize = 0;

		// search
		while (true)
		{
			if (node == NULL)
				return NULL;
			else if (node->key < in_key)
			{
				stack[stackSize++] = node;
				node = node->right;
				continue;
			}
			else if (in_key < node->key)
			{
				stack[stackSize++] = node;
				node = node->left;
				continue;
			}

			// found node
			break;
		}

		// delete
		while (true)
		{
			if (node->left == NULL)
			{
				if (node->right != NULL)
				{
					// single right child
					node->right->color = BLACK;

					if (stackSize == 0)
					{
						*io_root = node->right;
						return node;
					}

					SwapChildPtr(stack[stackSize - 1], node,
						node->right);
					return node;
				}

				if (node->color == RED)
				{
					// red node, no children
					SwapChildPtr(stack[stackSize - 1], node, NULL);
					return node;
				}
				else if (stackSize != 0)
				{
					// black node, no children
					break;
				}
				else
				{
					*io_root = NULL;
					return node;
				}
			}
			else if (node->right == NULL)
			{
				// single left child
				node->left->color = BLACK;

				if (stackSize == 0)
				{
					*io_root = node->left;
					return node;
				}

				SwapChildPtr(stack[stackSize - 1], node,
					node->left);
				return node;
			}
			else
			{
				// two children
				U32 nodeIndex = stackSize;
				stack[stackSize++] = node;
				Node* successor = node->right;

				while (successor->left != NULL)
				{
					stack[stackSize++] = successor;
					successor = successor->left;
				}

				stack[nodeIndex] = successor;
				Swap(node, successor);

				if ((stackSize - 1) != nodeIndex)
				{
					// successor is not right child
					SwapChildPtr(stack[stackSize - 1], successor, node);
				}
				else
				{
					successor->right = node;
				}
				
				if (nodeIndex == 0)
				{
					*io_root = successor;
					continue;
				}
				else
				{
					SwapChildPtr(stack[nodeIndex - 1], node, successor);
					continue;
				}
			}
		}

		// fix
		Node* _parent = stack[stackSize - 1];
		Node* _node = node;

		while (true)
		{
			if (stackSize == 0)
				break;

			Node* parent = stack[stackSize - 1];
			Node* sibling;

			if (parent->left == node)
			{
				// right sibling
				sibling = parent->right;

				if (sibling->color == BLACK)
				{
					if (sibling->left != NULL
						&& sibling->left->color == RED)
					{
						// black sibling, red child, right, left
						RotateRight(io_root, parent, sibling);
						RotateLeft(io_root, (stackSize == 1) ? NULL :
							stack[stackSize - 2], parent);
						sibling->left->color = parent->color;
						sibling->color = BLACK;
						parent->color = BLACK;
						break;
					}
					else if (sibling->right != NULL
						&& sibling->right->color == RED)
					{
						// black sibling, red child, right, right
						RotateLeft(io_root, (stackSize == 1) ? NULL :
							stack[stackSize - 2], parent);
						sibling->color = parent->color;
						parent->color = RED;
						break;
					}
				}
				else
				{
					// red sibling, right
					RotateLeft(io_root, (stackSize == 1) ? NULL :
						stack[stackSize - 2], parent);
					parent->color = RED;
					sibling->color = BLACK;
					stack[stackSize - 1] = sibling;
					stack[stackSize++] = parent;
					continue;
				}
			}
			else
			{
				// left sibling
				sibling = parent->left;

				if (sibling->color == BLACK)
				{
					if (sibling->left != NULL
						&& sibling->left->color == RED)
					{
						// black sibling, red child, left, left
						RotateRight(io_root, (stackSize == 1) ? NULL :
							stack[stackSize - 2], parent);
						sibling->color = parent->color;
						parent->color = RED;
						break;
					}
					else if (sibling->right != NULL
						&& sibling->right->color == RED)
					{
						// black sibling, red child, left, right
						RotateLeft(io_root, parent, sibling);
						RotateRight(io_root, (stackSize == 1) ? NULL :
							stack[stackSize - 2], parent);
						sibling->right->color = parent->color;
						sibling->color = BLACK;
						parent->color = BLACK;
						break;
					}
				}
				else
				{
					// red sibling, left
					RotateRight(io_root, (stackSize == 1) ? NULL :
						stack[stackSize - 2], parent);
					parent->color = RED;
					sibling->color = BLACK;
					stack[stackSize - 1] = sibling;
					stack[stackSize++] = parent;
					continue;
				}
			}

			if (parent->color == RED)
			{
				// black sibling, black children, red parent
				sibling->color = RED;
				parent->color = BLACK;
				break;
			}
			else
			{
				// black sibling, black children, black parent
				sibling->color = RED;
				node = parent;
				stackSize--;
				continue;
			}
		}

		SwapChildPtr(_parent, _node, NULL);
		return _node;
	}
};
