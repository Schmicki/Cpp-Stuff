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

#include "Aliases.hpp"
#include "TLSFAllocator.hpp"

TLSFAllocator::TLSFAllocator()
	: heads(), xMask(0), yMasks()
{

}

TLSFAllocator::~TLSFAllocator()
{
}

TLSFAllocator::Node* TLSFAllocator::CreateNode(Void* inHeap, U32 inSize)
{
	return NULL;
}

U8 TLSFAllocator::GetBestIndex(U32 inSize, U32* outX, U32* outY) const
{
	U32 x, y, xmask, ymask;

	inSize = inSize - 1;

	if (inSize < SMALL_SIZE)
	{
		x = HighestBit(SMALL_SIZE - 1);
		y = inSize >> MIN_ALIGNMENT_LOG2;
	}
	else
	{
		x = HighestBit(inSize);
		y = (inSize - (1 << x)) >> (x - 5);
	}

	ymask = yMasks[x] & (UINT32_MAX << y);

	if (ymask == 0)
	{
		xmask = xMask & (UINT32_MAX << (x + 1));

		if (xmask == 0)
		{
			*outX = UINT32_MAX;
			*outY = UINT32_MAX;

			return FALSE;
		}

		x = LowestBit(xmask);
		ymask = yMasks[x];
	}

	y = LowestBit(ymask);

	*outX = x;
	*outY = y;

	return TRUE;
}

void TLSFAllocator::GetMemIndex(U32 inSize, U32* outX, U32* outY)
{
	if (inSize <= SMALL_SIZE)
	{
		*outX = HighestBit(SMALL_SIZE - 1);
		*outY = (inSize - MIN_ALIGNMENT) >> MIN_ALIGNMENT_LOG2;
		return;
	}
	else
	{
		U32 x, y;

		x = HighestBit(inSize - 1);
		y = x - 5;
		y = (inSize - (1 << x) - (1 << y)) >> y;

		*outX = x;
		*outY = y;
		return;
	}
}

TLSFAllocator::Node* TLSFAllocator::PopBest(U32 inSize)
{
	U32 x, y;
	Node* node;
	Node* next;

	if (inSize == 0 || inSize > MAX_SIZE)
		return NULL;
	
	if (inSize < NODE_SIZE)
		inSize = NODE_SIZE;

	inSize = AlignUp(inSize, (U32)MIN_ALIGNMENT);

	if (GetBestIndex(inSize, &x, &y))
	{
		node = heads[x][y];
		node->size |= USED_MASK;
		next = node->nextFree;
		heads[x][y] = next;

		if (next != NULL)
		{
			next->prevFree = NULL;
			return node;
		}
		else
		{
			yMasks[x] &= ~(1 << y);

			if (yMasks[x] == 0)
				xMask &= ~(1 << x);

			return node;
		}
	}

	return NULL;
}

void TLSFAllocator::PopNode(Node* inNode)
{
	U32 x, y, size;
	Node* next;
	Node* prev;

	inNode->size &= ~USED_MASK;

	size = inNode->size & SIZE_MASK;
	prev = inNode->prevFree;

	if (prev != NULL)
	{
		next = inNode->nextFree;
		prev->nextFree = next;

		if (next != NULL)
			next->prevFree = prev;
		
		return;
	}
	else
	{
		GetMemIndex(size, &x, &y);

		next = inNode->nextFree;

		heads[x][y] = next;

		if (next != NULL)
		{
			next->prevFree = NULL;
			return;
		}
		else
		{
			yMasks[x] &= ~(1 << y);

			if (yMasks[x] == 0)
				xMask &= ~(1 << x);

			return;
		}
	}
}

void TLSFAllocator::Push(Node* inNode)
{
	U32 x, y, size;
	Node* next;

	inNode->size &= ~USED_MASK;
	size = inNode->size & SIZE_MASK;

	GetMemIndex(size, &x, &y);

	next = heads[x][y];

	if (next != NULL)
		next->prevFree = inNode;

	inNode->prevFree = NULL;
	inNode->nextFree = next;

	heads[x][y] = inNode;
	yMasks[x] |= (1 << y);
	xMask |= (1 << x);
}

// 
// Parameter inNode must be a valid node.
// Parameter inSize must be greater or equal inNode->size.
// Parameter inSize must be aligned to MIN_ALIGNMENT.
// 
// Return NULL if there is not enough overhead to create a second node.
// Return Node* overhead.
// 
TLSFAllocator::Node* TLSFAllocator::Split(Node* inNode, U32 inSize)
{
	U32 rest, flags;
	
	rest = (inNode->size & SIZE_MASK) - inSize;
	flags = inNode->size & FLAGS_MASK;
	
	if (rest < COMBINED_NODE_SIZE)
		return NULL;

	inNode->size = inSize | flags | NEXT_MASK;

	Node* overhead;

	overhead = (Node*)((UPtr)inNode + HEADER_SIZE + inSize);
	overhead->prevSize = inSize;
	overhead->size = (rest - HEADER_SIZE) | flags;

	return overhead;
}

void TLSFAllocator::Merge(Node* inNode, Node* inNext)
{
	U32 size;

	size = inNext->size + HEADER_SIZE;
	size += inNode->size & SIZE_MASK;
	inNode->size = size;
}

//void* BestFitAllocator::Allocate(U32 inSize, U32 inAlignment, U32* outSize)
//{
//	if (inAlignment <= MIN_ALIGNMENT)
//	{
//		Node* node;
//		Node* overhead;
//
//		inSize = AlignUp(inSize, (U32)MIN_ALIGNMENT);
//
//		node = Pop(inSize);
//
//		if (node == NULL)
//			return NULL;
//
//		overhead = Split(node, inSize);
//
//		if (overhead != NULL)
//			Push(overhead);
//
//		if (outSize != NULL)
//			*outSize = node->size & SIZE_MASK;
//
//		return GetMem(node);
//	}
//	else
//	{
//		U32 offset;
//		Node* node;
//		Node* overhead;
//
//		inSize = AlignUp(inSize, inAlignment);
//
//		overhead = Pop(inSize + inAlignment + COMBINED_NODE_SIZE);
//
//		if (overhead == NULL)
//			return NULL;
//
//		offset = inAlignment - (((U32)((UPtr)GetMem(overhead))) & (inAlignment - 1));
//
//		if (offset < COMBINED_NODE_SIZE)
//			offset += inAlignment;
//
//		node = Split(overhead, offset - HEADER_SIZE);
//		Push(overhead);
//
//		overhead = Split(node, inSize);
//		
//		if (overhead != NULL)
//			Push(overhead);
//
//		if (outSize != NULL)
//			*outSize = node->size & SIZE_MASK;
//
//		return GetMem(node);
//	}
//}

//void BestFitAllocator::Free(void* inMemory)
//{
//	U32 combinedSize;
//	Node* node;
//	Node* tmp;
//
//	if (inMemory == NULL)
//		return;
//
//	node = GetNode(inMemory);
//
//	if (node->size & NEXT_MASK)
//	{
//		combinedSize = (node->size & SIZE_MASK) + HEADER_SIZE;
//		tmp = (Node*)((UPtr)node + (UPtr)combinedSize);
//
//		if ((tmp->size & USED_MASK) == 0)
//		{
//			PopNode(tmp);
//			Merge(node, tmp);
//		}
//	}
//
//	if (node->prevSize != 0)
//	{
//		combinedSize = node->prevSize + HEADER_SIZE;
//		tmp = (Node*)((UPtr)node - (UPtr)combinedSize);
//
//		if ((tmp->size & USED_MASK) == 0)
//		{
//			PopNode(tmp);
//			Merge(tmp, node);
//			Push(tmp);
//
//			return;
//		}
//	}
//
//	Push(node);
//}