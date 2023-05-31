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

void ExampleTask(WorkerBase* worker, void* text)
{
	std::cout << (char*)text << "\n";
}

int main(int argc, char** args)
{
	std::cout <<
		"HelperTaskSystem\n"
		"\n"
		"A task system I wrote from my mind. Which uses a dynamic helper thread to split/steal\n"
		"tasks. It uses a futex to go to sleep if there is no work in the loop.\n"
		"\n"
		"Throughput test, submitting empty tasks.\n"
		"\n";

	{
		WorkerBase* worker;
		HelperTaskSystem taskSystem(&worker);

		test_loop(0x10)
		{
			test_loop_begin_test;

			for (U32 i = 0; i < 0x100000; i++)
			{
				TaskHandle task = worker->NewTask(nullptr, nullptr, TaskHandle(), TaskHandle());
				worker->SubmitTask(task);
			}

			test_loop_end_test;
		}
		test_loop_print_result("HelperTaskSystem" << " - " << ((F64)_avg__ / (F64)0x100000) << " ns/task");

		// Submit example task
		const char* text = "\nExampleTask: Hello World!\n";

		TaskHandle task = worker->NewTask(&ExampleTask, (void*)text, TaskHandle(), TaskHandle());
		worker->SubmitTask(task);
	}
}