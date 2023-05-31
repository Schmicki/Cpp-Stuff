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

#include "Array.hpp"
#include "Set.hpp"
#include "Map.hpp"

template<class T>
inline Set<T>::Set()
	: size(0), capacity(0), data((T*)nullptr)
{
}

template<class T>
inline Set<T>::Set(U32 capacity)
	: size(0), capacity(0), data((T*)nullptr)
{
	Reserve(capacity);
}

template<class T>
inline Set<T>::Set(const Set& other)
	: size(other.size), capacity(other.capacity), data((T*)nullptr)
{
	if (size == 0)
	{
		capacity = 0;
		return;
	}

	data = Allocate<T>(other.size);
	CopyToNew(data, other.data, other.size);
}

template<class T>
inline Set<T>::Set(RRef<Set>& other) noexcept
	: size(other->size), capacity(other->capacity), data(other->data)
{
	other->size = 0;
	other->capacity = 0;
	other->data = (T*)nullptr;
}

template<class T>
inline Set<T>::Set(const T* data, U32 size) noexcept
	: size(0), capacity(0), data((T*)nullptr)
{
	Reserve(size);
	for (U32 i = 0; i < size; i++)
		Add(data[i]);
}

template<class T>
inline Set<T>::~Set()
{
	if (data)
	{
		DestroyArrayElements(data, size);
		Free(data);
	}
}

template<class T>
inline Set<T>& Set<T>::operator=(const Set& other)
{
	if (this != &other)
	{
		Clear();

		if (other.size == 0)
		{
			size = 0;
			capacity = 0;
			data = (T*)nullptr;
			return *this;
		}

		size = other.size;
		capacity = other.capacity;
		data = Allocate<T>(other.size);
		CopyToNew(data, other.data, other.size);
	}
	return *this;
}

template<class T>
inline Set<T>& Set<T>::operator=(RRef<Set>& other) noexcept
{
	if (this != &(*other))
	{
		Clear();

		size = other->size;
		capacity = other->capacity;
		data = other->data;

		other->size = 0;
		other->capacity = 0;
		other->data = (T*)nullptr;
	}
	return *this;
}

template<class T>
inline T& Set<T>::operator[](U32 index)
{
	return data[index];
}

template<class T>
inline const T& Set<T>::operator[](U32 index) const
{
	return data[index];
}

template<class T>
inline T& Set<T>::Get(U32 index)
{
	return data[index];
}

template<class T>
inline const T& Set<T>::Get(U32 index) const
{
	return data[index];
}

template<class T>
inline void Set<T>::Reserve(U32 size)
{
	if (size <= capacity) return;

	capacity = CalculateContainerCapacity(size);
	T* oldData = data;
	data = (T*)malloc(capacity * sizeof(T));
	MoveToNew(data, oldData, this->size);

	if (oldData) Free(oldData);
}

template<class T>
inline void Set<T>::Release()
{
	if ((capacity / 4) > size)
	{
		capacity /= 2;
		T* oldData = data;
		data = Allocate<T>(capacity);
		MoveToNew(data, oldData, size);

		if (oldData) Free(oldData);
	}
}

template<class T>
inline void Set<T>::Clear()
{
	if (data)
	{
		DestroyArrayElements(data, size);
		Free(data);

		size = 0;
		capacity = 0;
		data = (T*)nullptr;
	}
}

template<class T>
inline Bool Set<T>::Find(const T& element, U32& outIndex)
{
	outIndex = 0;
	U32 stepSize = 0x400;
	while (stepSize > 1)
	{
		do
		{
			outIndex += stepSize;
		} while (outIndex < size && data[outIndex] < element);
		outIndex -= stepSize;
		SHFTDS(stepSize, 2);
	}

	for (; outIndex < size && data[outIndex] < element; outIndex++);
	return outIndex < size && !(element < data[outIndex]);
}

template<class T>
inline void Set<T>::Add(const T& element)
{
	U32 index;
	if (!Find(element, index))
	{
		Reserve(size + 1);

		for (U32 i = size; i > index; --i)
		{
			new (data + i) T(Mov(data[i - 1]));
			data[i - 1].~T();
		}

		new (data + index) T(element);
		size++;
	}
}

template<class T>
inline void Set<T>::Add(RRef<T>& element)
{
	U32 index;
	if (!Find(element, index))
	{
		Reserve(size + 1);

		for (U32 i = size; i > index; --i)
		{
			new (data + i) T(Mov(data[i - 1]));
			data[i - 1].~T();
		}

		new (data + index) T(Mov(element));
		size++;
	}
	else
	{
		T tmp = Mov(element);
	}
}

template<class T>
inline void Set<T>::Add(const Set& other)
{
	Reserve(size + other.size);
	for (U32 i = 0; i < other.size; i++)
		Add(other[i]);
}

template<class T>
inline void Set<T>::Add(RRef<Set>& other)
{
	if (other->size > 0)
	{
		Reserve(size + other->size);
		for (U32 i = 0; i < other->size; i++)
			Add(Mov((*other)[i]));

		DestroyArrayElements(other->data, other->size);
		Free(other.data);

		other->size = 0;
		other->capacity = 0;
		other->data = (T*)nullptr;
	}
}

template<class T>
inline void Set<T>::Insert(const T& element, U32 index)
{
	Reserve(size + 1);

	for (U32 i = size; i > index; --i)
	{
		new (data + i) T(Mov(data[i - 1]));
		data[i - 1].~T();
	}

	new (data + index) T(element);
	size++;
}

template<class T>
inline void Set<T>::Insert(RRef<T>& element, U32 index)
{
	Reserve(size + 1);

	for (U32 i = size; i > index; --i)
	{
		new (data + i) T(Mov(data[i - 1]));
		data[i - 1].~T();
	}

	new (data + index) T(Mov(element));
	size++;
}

template<class T>
inline void Set<T>::Remove(U32 index)
{
	data[index].~T();
	--size;
	MoveToNew(data + index, data + index + 1, size - index);
	Release();
}

template<class T>
inline T* Set<T>::begin()
{
	return data;
}

template<class T>
inline const T* Set<T>::begin() const
{
	return data;
}

template<class T>
inline T* Set<T>::end()
{
	return data + size;
}

template<class T>
inline const T* Set<T>::end() const
{
	return data + size;
}
