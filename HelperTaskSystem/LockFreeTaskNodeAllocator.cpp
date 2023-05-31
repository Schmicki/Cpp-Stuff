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

#include "LockFreeTaskNodeAllocator.hpp"

LockFreeTaskNodeAllocator::LockFreeTaskNodeAllocator(U32 blockCount, U32 blockSize)
	: taskNodeCount(0),
	blockSize(0),
	freeTaskNodeList()
{
	Initialize(blockCount, blockSize);
}

LockFreeTaskNodeAllocator::~LockFreeTaskNodeAllocator()
{
	Destroy();
}

void LockFreeTaskNodeAllocator::Initialize(U32 blockCount, U32 blockSize)
{
	// task nodes
	if (blockCount == 0 || blockSize == 0)
		return;

	this->blockSize = blockSize;
	taskNodeCount = blockCount * blockSize;
	freeTaskNodeList.taskNodes = new LockFreeTaskNode[taskNodeCount];

	for (U32 i = 0; i < taskNodeCount; i++)
	{
		if (((i + 1) % blockSize) == 0)
			freeTaskNodeList.taskNodes[i].task.dependency.index = UINT32_MAX;
		else
			freeTaskNodeList.taskNodes[i].task.dependency.index = i + 1;

		if ((i % blockSize) == 0)
		{
			U32 next = i + blockSize;
			if (taskNodeCount > next)
				freeTaskNodeList.taskNodes[i].next.store(next, std::memory_order_relaxed);
			else
				freeTaskNodeList.taskNodes[i].next.store(UINT32_MAX, std::memory_order_relaxed);
		}
	}

	freeTaskNodeList.list.first.store(0, std::memory_order_relaxed);
	freeTaskNodeList.list.last.store(taskNodeCount - blockSize, std::memory_order_relaxed);
}

void LockFreeTaskNodeAllocator::Destroy()
{
	delete[] freeTaskNodeList.taskNodes;
}

U32 LockFreeTaskNodeAllocator::TryPop()
{
	return freeTaskNodeList.tryPop();
}

void LockFreeTaskNodeAllocator::Push(U32 first, U32 last)
{
	freeTaskNodeList.taskNodes[last].task.dependency.index = UINT32_MAX;
	freeTaskNodeList.push(first);
}
