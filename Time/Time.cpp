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

#include "Time.h"

#if defined(__linux__)

#include <time.h>


std::uint64_t getTime()
{
    time_t tp = time(NULL);
    struct tm* ct = localtime(&tp);

    std::uint64_t time = (ct->tm_mon + 1) * 100000000;
    time += ct->tm_mday * 1000000;
    time += ct->tm_hour * 10000;
    time += ct->tm_min * 100;
    time += ct->tm_sec;

    return time;
}

std::uint64_t getClockTime()
{
    timespec tp;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tp);
    return tp.tv_sec * 1000000000 + tp.tv_nsec;
}

#elif defined(_WIN32)

#include <time.h>
#include <Windows.h>

std::uint32_t getTime()
{
    time_t tp = time(NULL);
    struct tm ct;
    localtime_s(&ct, &tp);

    std::uint32_t time = 100000000 * (ct.tm_mon + 1);
    time += ct.tm_mday * 1000000;
    time += ct.tm_hour * 10000;
    time += ct.tm_min * 100;
    time += ct.tm_sec;

    return time;
}

std::uint64_t getClockTime()
{
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
        
    std::uint64_t nsec = (((counter.QuadPart - (counter.QuadPart / 1000000000) * 1000000000) * 1000000000) / frequency.QuadPart) % 1000000000;
    std::uint64_t sec = counter.QuadPart / frequency.QuadPart;

    return sec * 1000000000 + nsec;
}

#endif
