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

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <new>
#include <iostream>

#define Assert(condition, message) \
if (!(condition)) \
{ \
	printf("%s\n", message); \
	assert(false); \
}

#ifndef NULL
#define NULL 0
#endif

enum LanguageConstants
{
	ALLOC_STEP_SIZE = 0x40000
};

#define WINDOWS_LEAN_AND_MEAN 1

#include <Windows.h>

typedef int8_t I8;
typedef int16_t I16;
typedef int32_t I32;
typedef int64_t I64;
typedef ptrdiff_t IPtr;

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;
typedef size_t UPtr;

typedef float F32;
typedef double F64;
typedef void Void;

inline U32 LowestBit(U32 inValue)
{
	U32 result = 0;

	if ((inValue & 0xFFFF) == 0)
	{ 
		result += 16;
		inValue >>= 16;
	}

	if ((inValue & 0xFF) == 0)
	{
		result += 8;
		inValue >>= 8;
	}

	if ((inValue & 0xF) == 0)
	{
		result += 4;
		inValue >>= 4;
	}

	if ((inValue & 0x3) == 0)
	{
		result += 2;
		inValue >>= 2;
	}

	result += ((~inValue) & 0x1);

	return result;
}

inline U32 HighestBit(U32 inValue)
{
	U32 result = 0;

	if ((inValue & 0xFFFF0000) == 0)
	{
		result += 16;
		inValue <<= 16;
	}
	if ((inValue & 0xFF000000) == 0)
	{
		result += 8;
		inValue <<= 8;
	}
	if ((inValue & 0xF0000000) == 0)
	{
		result += 4;
		inValue <<= 4;
	}
	if ((inValue & 0xC0000000) == 0)
	{
		result += 2;
		inValue <<= 2;
	}

	result += ((~inValue) & 0x80000000);

	return 31 - result;
}

inline U32 AlignUp(U32 inValue, U32 inAlignment)
{
	inAlignment = inAlignment - 1;
	return (inValue + inAlignment) & ~inAlignment;
}

inline U32 AlignDown(U32 inValue, U32 inAlignment)
{
	return inValue & ~(inAlignment - 1);
}

inline U64 AlignUp(U64 inPtr, U64 inAlignment)
{
	inAlignment = inAlignment - 1;
	return (inPtr + inAlignment) & ~inAlignment;
}

inline U64 AlignDown(U64 inPtr, U64 inAlignment)
{
	return inPtr & ~(inAlignment - 1);
}

inline void* AlignUp(void* inPtr, UPtr inAlignment)
{
	inAlignment = inAlignment - 1;
	return (void*)(((UPtr)inPtr + inAlignment) & ~inAlignment);
}

inline void* AlignDown(void* inPtr, UPtr inAlignment)
{
	return (void*)((UPtr)inPtr & ~(inAlignment - 1));
}

inline IPtr Distance(void* inFrom, void* inTo)
{
	return (IPtr)((UPtr)inTo - (UPtr)inFrom);
}

inline U8 InRangeInclusive(U32 inValue, U32 inMin, U32 inMax)
{
	return (inValue >= inMin) & (inValue < inMax);
}