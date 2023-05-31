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

#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#include <iostream>
#include "Platform.hpp"
#include "SpinLock.hpp"
#include "Futex.hpp"

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::high_resolution_clock::time_point TimePoint;

#define test_loop(count)				\
I64 _avg__ = 0;							\
I64 _max__ = 0;							\
for (U32 _i_ = 0; _i_ < count; _i_++)

#define test_loop_begin_test TimePoint _start__ = Clock::now()

#define test_loop_end_test						            \
TimePoint _end__ = Clock::now();					        \
												            \
if (_i_ == 0)									            \
{           									            \
    _avg__ = (_end__ - _start__).count();			        \
    _max__ = _avg__;				                        \
}           				                                \
else											            \
{   											            \
    _avg__ = (_avg__ + (_end__ - _start__).count()) / 2;    \
    _max__ = _max__ < _avg__ ? _avg__ : _max__;             \
} do {} while(false)

#define test_loop_print_result(name) std::cout << name << " - Average Time: " << _avg__ << ", Max Time:" << _max__ << "\n"

static void spinWait(F32 seconds) {
    I64 nanoseconds = (I64)(seconds * 1000000000.0f);
    TimePoint _start__ = Clock::now();
    while ((Clock::now() - _start__).count() <= nanoseconds);
}
