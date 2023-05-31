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

#include "LockFreeTaskNodeQueue.hpp"

LockFreeTaskNodeQueue::LockFreeTaskNodeQueue(LockFreeTaskNode* taskNodes)
    : list(),
    taskNodes(taskNodes)
{
}

U32 LockFreeTaskNodeQueue::push(U32 index)
{
	taskNodes[index].next.store(UINT32_MAX, std::memory_order_relaxed);

	U32 last = list.last.exchange(index);

	if (last != UINT32_MAX)
	{
		taskNodes[last].next.store(index, std::memory_order_release);
		return 0;
	}

	U32 tmp;
	while (!list.first.compare_exchange_weak(tmp = UINT32_MAX, index))
		while (list.first.load(std::memory_order_acquire) != UINT32_MAX);

	return 1;
}

U32 LockFreeTaskNodeQueue::tryPop()
{
	U32 tmp, first = list.first.load(std::memory_order_acquire);

	if (first == UINT32_MAX && list.first.compare_exchange_strong(first, first))
		return UINT32_MAX;

	while (true)
	{
		if (first == UINT32_MAX)
		{
			return UINT32_MAX;
		}
		else if (first == (U32)-2)
		{
			while ((first = list.first.load(std::memory_order_acquire)) == (U32)-2);

			if (first == UINT32_MAX)
				return UINT32_MAX;
		}

		tmp = taskNodes[first].next.load(std::memory_order_acquire);

		if (tmp != UINT32_MAX)
		{
			if (list.first.compare_exchange_weak(first, tmp))
				return first;
			continue;
		}
		else
		{
			if (!list.first.compare_exchange_weak(first, (U32)-2))
				continue;

			U32 last = first;
			if (list.last.compare_exchange_strong(last, tmp))
			{
				list.first.store(tmp, std::memory_order_release);
				return first;
			}
			else
			{
				while ((tmp = taskNodes[first].next.load(std::memory_order_acquire)) == -1);

				list.first.store(tmp, std::memory_order_release);
				return first;
			}
		}
	}
}

Bool LockFreeTaskNodeQueue::IsEmpty()
{
	return list.first.load(std::memory_order_relaxed) == UINT32_MAX;
}

LockFreeMPSCTaskNodeQueue::LockFreeMPSCTaskNodeQueue(LockFreeTaskNode* taskNodes)
	: list(), taskNodes(taskNodes)
{
}

U32 LockFreeMPSCTaskNodeQueue::push(U32 index)
{
	taskNodes[index].next.store(UINT32_MAX, std::memory_order_relaxed);

	U32 last = list.last.exchange(index);

	if (last != UINT32_MAX)
	{
		taskNodes[last].next.store(index, std::memory_order_release);
		return 0;
	}

	U32 tmp;
	while (!list.first.compare_exchange_weak(tmp = UINT32_MAX, index))
		while (list.first.load(std::memory_order_acquire) != UINT32_MAX);

	return 1;
}

U32 LockFreeMPSCTaskNodeQueue::tryPop()
{
	U32 tmp, first = list.first.load(std::memory_order_acquire);

	while (true)
	{
		if (first == UINT32_MAX)
			return UINT32_MAX;

		tmp = taskNodes[first].next.load(std::memory_order_acquire);

		if (tmp != UINT32_MAX)
		{
			list.first.store(tmp, std::memory_order_release);
			return first;
		}
		else
		{
			U32 last = first;
			if (list.last.compare_exchange_strong(last, tmp))
			{
				list.first.store(tmp, std::memory_order_release);
				return first;
			}
			else
			{
				while ((tmp = taskNodes[first].next.load(std::memory_order_acquire)) == -1);

				list.first.store(tmp, std::memory_order_release);
				return first;
			}
		}
	}
}

Bool LockFreeMPSCTaskNodeQueue::IsEmpty()
{
	return list.first.load(std::memory_order_relaxed) == UINT32_MAX;
}
