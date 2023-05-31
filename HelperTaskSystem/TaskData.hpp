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

#include "Core.hpp"

typedef void (*TaskFunction)(class WorkerBase*, void*);

struct TaskFlags
{
	enum
	{
		TFNone = 0,
		TFQuit = 0xFFFF,
	};
};

struct TaskHandle
{
	U32 index;
	U32 generation;

	TaskHandle() : index(UINT32_MAX), generation(UINT32_MAX) {}
	TaskHandle(U32 index, U32 generation) : index(index), generation(generation) {}
};

struct Task
{
	TaskFunction function;
	void* args;
	TaskHandle dependency;
	TaskHandle parent;
	U32 flags;
	std::atomic<U32> count;

	Task()
		: function((TaskFunction)nullptr),
		args(nullptr),
		dependency(),
		parent(),
		count(0),
		flags(0)
	{}
};

struct CACHE_ALIGN LockFreeTaskNode
{
	Task task;
	std::atomic<U32> next;
	std::atomic<U32> generation;

	LockFreeTaskNode() :task(), next(0), generation(0) {}
};

struct CACHE_ALIGN SafeList
{
	std::atomic<U32> first;
	char pad0[CACHE_LINE - sizeof(std::atomic<U32>)];
	std::atomic<U32> last;

	SafeList() :first(UINT32_MAX), pad0(), last(UINT32_MAX) {}
};