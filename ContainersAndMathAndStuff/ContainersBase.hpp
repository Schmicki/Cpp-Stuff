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

#include "Platform.hpp"
#include "Cast.hpp"
#include "Templates.hpp"

template<class T>
inline void CopyTo(T* dst, const T* src, U32 size)
{
	for (U32 i = 0; i < size; i++)
	{
		dst[i] = src[i];
	}
}

template<class T>
inline void CopyToNew(T* dst, const T* src, U32 size)
{
	for (U32 i = 0; i < size; i++)
	{
		new (dst + i) T(src[i]);
	}
}

template<class T>
inline void MoveTo(T* dst, T* src, U32 size)
{
	for (U32 i = 0; i < size; i++)
	{
		dst[i] = Mov(src[i]);
		src[i].~T();
	}
}

template<class T>
inline void MoveToNew(T* dst, T* src, U32 size)
{
	for (U32 i = 0; i < size; i++)
	{
		new (dst + i) T(Mov(src[i]));
		src[i].~T();
	}
}

template<class T>
inline void InitializeArrayElements(T* arr, U32 size, const T& defaultElement = T())
{
	for (U32 i = 0; i < size; i++)
	{
		new (arr + i) T(defaultElement);
	}
}

template<class T>
inline void DestroyArrayElements(T* arr, U32 size)
{
	for (U32 i = 0; i < size; i++)
	{
		arr[i].~T();
	}
}

inline U32 IncreaseContainerCapacity(U32 capacity) noexcept
{
	return SHFTM(capacity, 1) + 1;
}

inline U32 CalculateContainerCapacity(const U32 target) noexcept
{
	U32 capacity = 3;
	for (; capacity <= target; capacity = IncreaseContainerCapacity(capacity));
	return capacity;
}
