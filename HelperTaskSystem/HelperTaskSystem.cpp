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

#include "HelperTaskSystem.hpp"

HelperTaskSystem::HelperTaskSystem(WorkerBase** _mainThreadWorker)
	: nodeAllocator(),
	helper(),
	workers((HelperTaskSystemWorker*)nullptr),
	workerCount(0),
	threads()
{
	U32 threadCount = std::thread::hardware_concurrency();

	if (threadCount < 4)
		threadCount = 4;

	workerCount = threadCount;

	// task nodes
	nodeAllocator.Initialize(workerCount * 3, 0x400);
	helper.index = 0;
	helper.queue.taskNodes = nodeAllocator.freeTaskNodeList.taskNodes;
	helper.state.val.store(1, std::memory_order_relaxed);

	// workers
	workers = (HelperTaskSystemWorker*)malloc(sizeof(HelperTaskSystemWorker) * threadCount);

	for (U32 i = 0; i < workerCount; i++)
	{
		U32 first = nodeAllocator.TryPop();
		new (workers + i) HelperTaskSystemWorker(this, first, i);
	}

	// threads
	for (U32 i = 1; i < workerCount; i++)
	{
		threads.push_back(std::thread(&WorkerLoop, workers + i));
	}

	if (_mainThreadWorker != nullptr)
		*_mainThreadWorker = workers;
}

HelperTaskSystem::~HelperTaskSystem()
{
	for (U32 i = 0; i < workerCount; i++)
	{
		TaskHandle quitTask = workers[0].NewTask(nullptr, nullptr, TaskHandle(), TaskHandle());
		nodeAllocator.freeTaskNodeList.taskNodes[quitTask.index].task.flags = TaskFlags::TFQuit;
		workers[0].SubmitTask(quitTask);
	}

	for (U32 i = 0; i < threads.size(); i++)
	{
		threads[i].join();
	}

	for (U32 i = 0; i < workerCount; i++)
		workers[i].~HelperTaskSystemWorker();
	Free(workers);
}

void HelperTaskSystem::WorkerLoop(HelperTaskSystemWorker* worker)
{
	while (true)
	{
		U32 task = worker->PopWork();
		
		if (worker->workList.taskNodes[task].task.flags == TaskFlags::TFQuit)
			break;

		worker->ExecuteTask(task);
	}
}

HelperTaskSystemWorker::HelperTaskSystemWorker(
	HelperTaskSystem* taskSystem,
	U32 freeListStart,
	U32 index
)
	: taskSystem(taskSystem),
	freeListStart(freeListStart),
	doneListStart(-1),
	doneListEnd(-1),
	doneListSize(0),
	blockSize(taskSystem->nodeAllocator.blockSize),
	index(index),
	workList(taskSystem->nodeAllocator.freeTaskNodeList.taskNodes)
{
}

void HelperTaskSystemWorker::PushDone(U32 index)
{
	if (doneListSize != blockSize)
	{
		if (doneListSize != 0)
		{
			workList.taskNodes[doneListEnd].task.dependency.index = index;
			doneListEnd = index;
			doneListSize++;
			return;
		}

		doneListStart = index;
		doneListEnd = index;
		doneListSize = 1;
		return;
	}

	taskSystem->nodeAllocator.Push(doneListStart, doneListEnd);

	doneListStart = index;
	doneListEnd = index;
	doneListSize = 1;
}

void HelperTaskSystemWorker::PushWork(U32 index)
{
	if (workList.push(index))
	{
		Wake();
	}
}

U32 HelperTaskSystemWorker::TryPopWork()
{
	return workList.tryPop();
}

U32 HelperTaskSystemWorker::PopWork()
{
	U32 task;
	while (true)
	{
		if ((task = TryPopWork()) != UINT32_MAX)
			return task;

		Sleep();
	}
}

U32 HelperTaskSystemWorker::TryPopFree()
{
	// pop from worker free list
	U32 tmp;
	if (freeListStart != UINT32_MAX)
	{
		tmp = freeListStart;
		freeListStart = workList.taskNodes[freeListStart].task.dependency.index;
		return tmp;
	}

	// pop from allocator free list
	if ((tmp = taskSystem->nodeAllocator.TryPop()) != UINT32_MAX)
	{
		freeListStart = workList.taskNodes[tmp].task.dependency.index;
		return tmp;
	}

	return UINT32_MAX;
}

U32 HelperTaskSystemWorker::PopFree()
{
	U32 tmp;
	while (true)
	{
		if ((tmp = TryPopFree()) != UINT32_MAX)
			return tmp;

		if ((tmp = TryPopWork()) != UINT32_MAX)
		{
			ExecuteTask(tmp);
			continue;
		}

		(void)Help(false);
	}
}

U32 HelperTaskSystemWorker::Help(Bool allowSleep)
{
	if (!taskSystem->helper.lock.try_lock())
		return 0;

	TaskSystemHelper& helper = taskSystem->helper;
	HelperTaskSystemWorker* workers = taskSystem->workers;
	U32 myIndex = index - 1, task, workerCount = taskSystem->workerCount - 1;

	if (helper.index == myIndex)
		helper.index = (helper.index + 1) % workerCount;


	// Split work from helper queue

	while ((task = helper.queue.tryPop()) != UINT32_MAX)
	{
		workers[1 + helper.index].PushWork(task);

		helper.index = (helper.index + 1) % workerCount;

		if (helper.index != myIndex)
			continue;
			
		helper.index = (helper.index + 1) % workerCount;
		continue;
	}


	// Split work from workers


	// Steal work

	if (myIndex == UINT32_MAX)
	{
		for (U32 i = 0; i < workerCount; i++)
		{
			if ((task = taskSystem->workers[1 + i].workList.tryPop()) != UINT32_MAX)
			{
				PushWork(task);
				taskSystem->helper.lock.unlock();
				return 1;
			}
		}
	}
	else
	{
		for (U32 i = 0; i < myIndex; i++)
		{
			if ((task = taskSystem->workers[1 + i].workList.tryPop()) != UINT32_MAX)
			{
				PushWork(task);
				taskSystem->helper.lock.unlock();
				return 1;
			}
		}

		for (U32 i = myIndex + 1; i < workerCount; i++)
		{
			if ((task = taskSystem->workers[1 + i].workList.tryPop()) != UINT32_MAX)
			{
				PushWork(task);
				taskSystem->helper.lock.unlock();
				return 1;
			}
		}
	}


	// Sleep

	if (!allowSleep)
	{
		taskSystem->helper.lock.unlock();
		return 1;
	}
	
	I32 tmp = helper.state.val.load(std::memory_order_acquire);
	while (true)
	{
		I32 target = (tmp >= 1 && tmp <= 2) ? tmp - 1 : tmp;
		if (helper.state.val.compare_exchange_strong(tmp, target))
		{
			if (tmp >= 0 && tmp <= 1)
				helper.state.Wait(0, UINT32_MAX);

			break;
		}
	}

	taskSystem->helper.lock.unlock();
	return 1;
}

void HelperTaskSystemWorker::Sleep()
{
	while (true)
	{
		if (Help(true))
			return;

		if (taskSystem->helper.state.val.load(std::memory_order_relaxed) == 0)
			break;

		TimePoint start = Clock::now();

		while (std::chrono::nanoseconds(Clock::now() - start).count() < 100000)
		{
			if (!workList.IsEmpty())
				return;
		}
	}

	I32 tmp = state.val.load(std::memory_order_acquire);
	while (true)
	{
		I32 target = (tmp >= 1 && tmp <= 2) ? tmp - 1 : tmp;
		if (state.val.compare_exchange_strong(tmp, target))
		{
			if (tmp >= 0 && tmp <= 1)
			{
				if (Help(true))
					return;

				state.Wait(0, UINT32_MAX);
			}

			break;
		}
	}
}

void HelperTaskSystemWorker::Wake()
{
	I32 tmp = state.val.load(std::memory_order_acquire);
	while (true)
	{
		I32 target = (tmp >= 0 && tmp <= 1) ? tmp + 1 : tmp;
		if (state.val.compare_exchange_strong(tmp, target))
		{
			if (tmp == 0)
				state.WakeSingle();

			break;
		}
	}
}

void HelperTaskSystemWorker::FinishTask(U32 index)
{
	LockFreeTaskNode& node = workList.taskNodes[index];
	U32 count = node.task.count.fetch_sub((U32)1, std::memory_order_seq_cst);

	if (count == 1)
	{
		if (node.task.parent.index != UINT32_MAX)
			FinishTask(node.task.parent.index);

		PushDone(index);
	}
}

void HelperTaskSystemWorker::ExecuteTask(U32 index)
{
	Task& t = workList.taskNodes[index].task;

	if (t.dependency.index != UINT32_MAX && workList.taskNodes[t.dependency.index].generation.load(
		std::memory_order_relaxed) == t.dependency.generation)
	{
		SubmitTask(TaskHandle(index, workList.taskNodes[index].generation.load(std::memory_order_relaxed)));
		return;
	}

	if (t.function != nullptr)
		t.function(this, t.args);

	FinishTask(index);
}

// Do not pass a submitted task as parent
TaskHandle HelperTaskSystemWorker::NewTask(
	TaskFunction	function,
	void*			args,
	TaskHandle		dependency,
	TaskHandle		parent
)
{
	U32 index = PopFree();
	LockFreeTaskNode& node = workList.taskNodes[index];
	node.task.function = function;
	node.task.args = args;
	node.task.dependency = dependency;
	node.task.parent = parent;
	node.task.flags = 0;
	node.task.count.store(1, std::memory_order_relaxed);

	if (parent.index != UINT32_MAX)
	{
		LockFreeTaskNode& parentNode = workList.taskNodes[parent.index];
		parentNode.task.count.store(parentNode.task.count.load(std::memory_order_relaxed) + 1,
			std::memory_order_relaxed);
	}

	return TaskHandle(index, node.generation.load(std::memory_order_relaxed));
}

// Do not create child tasks after you submitted the parent
void HelperTaskSystemWorker::SubmitTask(TaskHandle task)
{
	if (taskSystem->helper.queue.push(task.index))
	{
		TaskSystemHelper& helper = taskSystem->helper;
		I32 tmp = helper.state.val.load(std::memory_order_acquire);
		while (true)
		{
			I32 target = (tmp >= 0 && tmp <= 1) ? tmp + 1 : tmp;
			if (helper.state.val.compare_exchange_strong(tmp, target))
			{
				if (tmp == 0)
					helper.state.WakeSingle();

				break;
			}
		}
	}
}

void HelperTaskSystemWorker::WaitOnTask(TaskHandle task)
{
	U32 tmp;
	while (true)
	{
		if (workList.taskNodes[task.index].generation.load(std::memory_order_relaxed) != task.generation)
			return;

		if ((tmp = TryPopWork()) != UINT32_MAX)
		{
			ExecuteTask(tmp);
			continue;
		}

		(void)Help(false);
	}
}
