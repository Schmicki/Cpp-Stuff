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

class TLSFAllocator
{
public:

	enum Internals
	{
		HEADER_SIZE = 8,
		NODE_SIZE = sizeof(Void*) * 2,
		COMBINED_NODE_SIZE = HEADER_SIZE + NODE_SIZE,

		HEAP_NODE_SIZE = sizeof(Void*) * 3,

		MIN_ALIGNMENT = 8,
		MIN_ALIGNMENT_LOG2 = 3,

		SMALL_SIZE = MIN_ALIGNMENT * 32,
		MAX_SIZE = (1 << 31) + (31 << (31 - 5)),

		USED_MASK = 1,
		NEXT_MASK = 2,
		FLAGS_MASK = 3,
		SIZE_MASK = ~3,
	};

	struct HeapNode
	{
		Void* mem;
		HeapNode* prev;
		HeapNode* next;
	};

	struct Node
	{
		U32 prevSize;
		U32 size;
		Node* prevFree;
		Node* nextFree;
	};

	Node* heads[32][32];
	U32 xMask;
	U32 yMasks[32];

	TLSFAllocator();
	~TLSFAllocator();

	static inline void* GetMem(Node* inNode)
	{
		return (Void*)((UPtr)inNode + (UPtr)HEADER_SIZE);
	}

	static inline Node* GetNode(Void* inMem)
	{
		return (Node*)((UPtr)inMem - (UPtr)HEADER_SIZE);
	}

	static inline U8 HasNext(Node* inNode)
	{
		return inNode->size & NEXT_MASK;
	}

	static inline U8 HasPrev(Node* inNode)
	{
		return inNode->prevSize != 0;
	}

	static inline U8 IsUsed(Node* inNode)
	{
		return inNode->size & USED_MASK;
	}

	static inline Node* GetNext(Node* inNode)
	{
		return (Node*)((UPtr)inNode + inNode->size + HEADER_SIZE);
	}

	static inline Node* GetPrev(Node* inNode)
	{
		return (Node*)((UPtr)inNode - inNode->prevSize - HEADER_SIZE);
	}

	static inline U32 GetSize(Node* inNode)
	{
		return inNode->size & SIZE_MASK;
	}

	static Node* CreateNode(Void* inHeap, U32 inSize);

	// 
	// Finds the index of the next largest free memory block.
	// Return TRUE if found.
	// Return FALSE if there is no large enough block.
	// 
	U8 GetBestIndex(U32 inSize, U32* outX, U32* outY) const;

	// 
	// Finds the index of largest block smaller or equal to than size.
	// 
	static void GetMemIndex(U32 inSize, U32* outX, U32* outY);

	// 
	// Pop best fitting node from free list and mark as used.
	// 
	Node* PopBest(U32 inSize);

	// 
	// Pop given node from free list.
	// 
	void PopNode(Node* inNode);

	// 
	// Push node into free list and mark as free.
	// 
	void Push(Node* inNode);

	// 
	// Split node in two.
	// 
	static Node* Split(Node* inNode, U32 inSize);

	// 
	// Merge two nodes from in contiguos memory.
	// 
	static void Merge(Node* inNode, Node* inNext);
};