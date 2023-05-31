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

#include "LockFreeTaskNodeAllocator.hpp"
#include "WorkerBase.hpp"

class HelperTaskSystemWorker;

class CACHE_ALIGN TaskSystemHelper
{
public:

	SpinLock lock;
	Byte pad0[CACHE_LINE - sizeof(SpinLock)];
	Futex state;
	Byte pad1[CACHE_LINE - sizeof(Futex)];
	U32 index;
	Byte pad2[CACHE_LINE - sizeof(U32)];
	LockFreeMPSCTaskNodeQueue queue;
};

class HelperTaskSystem
{
public:

	LockFreeTaskNodeAllocator nodeAllocator;
	TaskSystemHelper helper;
	HelperTaskSystemWorker* workers;
	U32 workerCount;
	std::vector<std::thread> threads;

	HelperTaskSystem(
		WorkerBase** _mainThreadWorker = (WorkerBase**)nullptr
	);

	~HelperTaskSystem();

	static void WorkerLoop(
		HelperTaskSystemWorker* worker
	);
};

class HelperTaskSystemWorker : public WorkerBase
{
public:

	HelperTaskSystem* taskSystem;
	U32 freeListStart;
	U32 doneListStart;
	U32 doneListEnd;
	U32 doneListSize;
	U32 blockSize;
	U32 index;
	LockFreeTaskNodeQueue workList;
	Futex state;

	HelperTaskSystemWorker(
		HelperTaskSystem*	taskSystem,
		U32					freeListStart,
		U32					index
	);

	void PushDone(U32 index);
	void PushWork(U32 index);
	U32 TryPopWork();
	U32 PopWork();
	U32 TryPopFree();
	U32 PopFree();

	U32 Help(Bool allowSleep);

	void Sleep();
	void Wake();

	void FinishTask(U32 index);
	void ExecuteTask(U32 index);

	// Creating a child task is not thread safe. Do not pass a parent handle that was already submitted.
	virtual TaskHandle NewTask(
		TaskFunction	function,
		void*			args,
		TaskHandle		dependency,
		TaskHandle		parent
	) override;

	virtual void SubmitTask(TaskHandle task) override;
	virtual void WaitOnTask(TaskHandle task) override;
};