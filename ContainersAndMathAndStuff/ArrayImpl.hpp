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
inline _ArrayBase<T>::_ArrayBase() noexcept
	: size(0), capacity(0), data((T*)nullptr)
{
}

template<class T>
inline _ArrayBase<T>::_ArrayBase(U32 capacity) noexcept
	: size(0), capacity(0), data((T*)nullptr)
{
	Reserve(capacity);
}

template<class T>
inline _ArrayBase<T>::_ArrayBase(const T* data, U32 size) noexcept
	: size(0), capacity(0), data((T*)nullptr)
{
	Reserve(size);
	this->size = size;
	for (U32 i = 0; i < size; ++i)
		new (this->data + i) T(data[i]);
}

template<class T>
inline _ArrayBase<T>::_ArrayBase(const _ArrayBase& other) noexcept
	: size(other.size), capacity(other.capacity), data((T*)nullptr)
{
	if (size == 0)
	{
		capacity = 0;
		return;
	}

	data = Allocate<T>(other.size);
	CopyToUninitialized(data, other.data, other.size);
}

template<class T>
inline _ArrayBase<T>::_ArrayBase(RRef<_ArrayBase>& other) noexcept
	: size(other->size), capacity(other->capacity), data(other->data)
{
	other->size = 0;
	other->capacity = 0;
	other->data = (T*)nullptr;
}

template<class T>
inline _ArrayBase<T>::~_ArrayBase()
{
	if (data)
	{
		DestroyArrayElements(data, size);
		Free(data);
	}
}

template<class T>
inline _ArrayBase<T>& _ArrayBase<T>::operator=(const _ArrayBase& other) noexcept
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
		CopyToUninitialized(data, other.data, other.size);
	}
	return *this;
}

template<class T>
inline _ArrayBase<T>& _ArrayBase<T>::operator=(RRef<_ArrayBase>& other) noexcept
{
	if (this != (_ArrayBase*)&other)
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
inline T& _ArrayBase<T>::operator[](U32 index)
{
	return data[index];
}

template<class T>
inline const T& _ArrayBase<T>::operator[](U32 index) const
{
	return data[index];
}

template<class T>
inline T& _ArrayBase<T>::Get(U32 index)
{
	return data[index];
}

template<class T>
inline const T& _ArrayBase<T>::Get(U32 index) const
{
	return data[index];
}

template<class T>
inline void _ArrayBase<T>::Reserve(U32 size)
{
	if (size <= capacity) return;

	capacity = CalculateContainerCapacity(size);
	T* oldData = data;
	data = (T*)malloc(capacity * sizeof(T));
	MoveToNew(data, oldData, this->size);

	if (oldData) Free(oldData);
}

template<class T>
inline void _ArrayBase<T>::Release()
{
	if ((capacity / 4) > size)
	{
		capacity /= 2;
		T* oldData = data;
		data = Allocate<T>(capacity);
		MoveToUninitialized(data, oldData, size);

		if (oldData) Free(oldData);
	}
}

template<class T>
inline void _ArrayBase<T>::Resize(U32 size, const T& defaultElement)
{
	if (size == this->size)
		return;

	if (size == 0)
	{
		DestroyArrayElements(data, this->size);

		if (data)
			Free(data);

		size = 0;
		capacity = 0;
		data = (T*)nullptr;
		return;
	}

	if (size > capacity || size < SHFTD(this->capacity, 1))
	{
		capacity = CalculateContainerCapacity(size);
		T* oldData = data;
		data = (T*)malloc(capacity * sizeof(T));

		if (size < this->size)
		{
			MoveToNew(data, oldData, size);
			DestroyArrayElements(oldData + size, this->size - size);
		}
		else
		{
			MoveToNew(data, oldData, this->size);
			InitializeArrayElements(data + this->size, size - this->size, defaultElement);
		}

		this->size = size;

		if (oldData)
			Free(oldData);

		return;
	}
	else
	{
		if (size > this->size)
		{
			InitializeArrayElements(data + this->size, size - this->size, defaultElement);
		}
		else
		{
			DestroyArrayElements(data + size, this->size - size);
		}

		this->size = size;
	}
}

template<class T>
inline void _ArrayBase<T>::Clear()
{
	if (data)
	{
		DestroyArrayElements(data, this->size);
		Free(data);
		size = 0;
		capacity = 0;
		data = (T*)nullptr;
	}
}

template<class T>
inline Bool _ArrayBase<T>::Find(const T& element, U32& outIndex)
{
	outIndex = 0;
	for (; outIndex < size && data[outIndex] != element; ++outIndex);
	return outIndex < size&& data[outIndex] == element;
}

template<class T>
inline void _ArrayBase<T>::Add(const T& element)
{
	Reserve(size + 1);
	new (data + size) T(element);
	++size;
}

template<class T>
inline void _ArrayBase<T>::Add(RRef<T>& element)
{
	Reserve(size + 1);
	new (data + size) T(Mov(element));
	++size;
}

template<class T>
inline void _ArrayBase<T>::Add(const _ArrayBase& other)
{
	Reserve(size + other.size);
	CopyToUninitialized(data + size, other.data, other.size);
}

template<class T>
inline void _ArrayBase<T>::Add(RRef<_ArrayBase>& other)
{
	Reserve(size + other->size);
	MoveToNew(data + size, other->data, other->size);

	if (other->data)
		Free(other->data);

	other->size = 0;
	other->capacity = 0;
	other->data = (T*)nullptr;
}

template<class T>
inline void _ArrayBase<T>::AddUnique(const T& element)
{
	U32 index;
	if (!Find(element, index))
		add(element);
}

template<class T>
inline void _ArrayBase<T>::AddUnique(RRef<T>& element)
{
	U32 index;
	if (!Find(*element, index))
		add(Mov(element));
}

template<class T>
inline void _ArrayBase<T>::Insert(const T& element, U32 index)
{
	if (index >= size)
		return add(element);

	Reserve(size + 1);

	for (U32 i = size; i > index; --i)
	{
		new (data + i) T(Mov(data[i - 1]));
		data[i - 1].~T();
	}

	new (data + index) T(element);
	++size;
}

template<class T>
inline void _ArrayBase<T>::Insert(RRef<T>& element, U32 index)
{
	if (index >= size)
		return add(Mov(element));

	Reserve(size + 1);

	for (U32 i = size; i > index; --i)
	{
		new (data + i) T(Mov(data[i - 1]));
		data[i - 1].~T();
	}

	new (data + index) T(Mov(element));
	++size;
}

template<class T>
inline void _ArrayBase<T>::Insert(const _ArrayBase<T>& other, U32 index)
{
	if (index >= size)
		return Add(other);

	Reserve(size + other.size);

	U32 end = index + other.size;
	for (U32 i = size + other.size - 1; i > end; --i)
	{
		new (data + i) T(Mov(data[i - other.size]));
		data[i - other.size].~T();
	}

	CopyToUninitialized(data + index, other.data, other.size);
	size += other.size;
}

template<class T>
inline void _ArrayBase<T>::Insert(RRef<_ArrayBase>& other, U32 index)
{
	if (index >= size)
		return Add(Mov(other));

	Reserve(size + other->size);

	U32 end = index + other->size;
	for (U32 i = size + other->size - 1; i > end; --i)
	{
		new (data + i) T(Mov(data[i - other->size]));
		data[i - other->size].~T();
	}

	MoveToNew(data + index, other->data, other->size);
	size += other->size;

	if (other->data)
		Free(other->data);

	other->size = 0;
	other->capacity = 0;
	other->data = (T*)nullptr;
}

template<class T>
inline void _ArrayBase<T>::Remove(U32 index)
{
	if (index >= size)
		return;
	
	data[index].~T();
	--size;
	MoveToNew(data + index, data + index + 1, size - index);
	Release();
}

template<class T>
inline void _ArrayBase<T>::Remove(U32 first, U32 last)
{
	if (first >= size || first > last)
		return;

	if (last >= size)
		last = size - 1;

	DestroyArrayElements(data + first, last - first + 1);
	MoveToNew(data + first, data + last + 1, size - last - 1);
	size -= last - first + 1;
	Release();
}

template<class T>
inline T* _ArrayBase<T>::begin()
{
	return data;
}

template<class T>
inline const T* _ArrayBase<T>::begin() const
{
	return data;
}

template<class T>
inline T* _ArrayBase<T>::end()
{
	return data + size;
}

template<class T>
inline const T* _ArrayBase<T>::end() const
{
	return data + size;
}


template<class T>
inline _NumericArray<T>::_NumericArray() noexcept
	:Base()
{
}

template<class T>
inline _NumericArray<T>::_NumericArray(U32 capacity) noexcept
	:Base(capacity)
{
}

template<class T>
inline _NumericArray<T>::_NumericArray(const _NumericArray& other) noexcept
	:Base(other)
{
}

template<class T>
inline _NumericArray<T>::_NumericArray(RRef<_NumericArray>& other) noexcept
	:Base(Mov(CastRR<Base>(other)))
{
}

template<class T>
inline _NumericArray<T>::_NumericArray(const T* data, U32 size) noexcept
	:Base(data, size)
{
}

template<class T>
inline T _NumericArray<T>::Min()
{
	if (this->size == 0)
		return (T)0.0;

	return this->data[MinIdx()];
}

template<class T>
inline U32 _NumericArray<T>::MinIdx()
{
	if (this->size == 0)
		return UINT32_MAX;

	U32 n = 0;
	for (U32 i = 1; i < this->size; ++i)
		if (this->data[i] < this->data[n])
			n = i;

	return n;
}

template<class T>
inline T _NumericArray<T>::AbsMin()
{
	if (this->size == 0)
		return (T)0.0;

	return this->data[AbsMinIdx()];
}

template<class T>
inline U32 _NumericArray<T>::AbsMinIdx()
{
	if (this->size == 0)
		return UINT32_MAX;

	U32 n = 0;
	for (U32 i = 1; i < this->size; ++i)
		if (Abs(this->data[i]) < Abs(this->data[n]))
			n = i;

	return n;
}

template<class T>
inline T _NumericArray<T>::Max()
{
	if (this->size == 0)
		return (T)0.0;

	return this->data[MaxIdx()];
}

template<class T>
inline U32 _NumericArray<T>::MaxIdx()
{
	if (this->size == 0)
		return UINT32_MAX;

	U32 n = 0;
	for (U32 i = 1; i < this->size; ++i)
		if (this->data[i] > this->data[n])
			n = i;

	return n;
}

template<class T>
inline T _NumericArray<T>::AbsMax()
{
	if (this->size == 0)
		return (T)0.0;

	return this->data[AbsMaxIdx()];
}

template<class T>
inline U32 _NumericArray<T>::AbsMaxIdx()
{
	if (this->size == 0)
		return UINT32_MAX;

	U32 n = 0;
	for (U32 i = 1; i < this->size; ++i)
		if (Abs(this->data[i]) > Abs(this->data[n]))
			n = i;

	return n;
}

template<class T>
inline T _NumericArray<T>::Sum()
{
	if (this->size == 0)
		return (T)0.0;

	T n = this->data[0];
	for (U32 i = 1; i < this->size; ++i)
		n += this->data[i];

	return n;
}

template<class T>
inline T _NumericArray<T>::AbsSum()
{
	if (this->size == 0)
		return (T)0.0;

	U32 n = Abs(this->data[0]);
	for (U32 i = 1; i < this->size; ++i)
		n += Abs(this->data[i]);

	return n;
}

template<class T>
inline T _NumericArray<T>::Avg()
{
	return Sum() / (T)this->size;
}

template<class T>
inline T _NumericArray<T>::AbsAvg()
{
	return AbsSum() / (T)this->size;
}

template<class T>
inline Array<T>::Array() noexcept
	:Base()
{
}

template<class T>
inline Array<T>::Array(U32 capacity) noexcept
	:Base(capacity)
{
}

template<class T>
inline Array<T>::Array(const Array& other) noexcept
	:Base(other)
{
}

template<class T>
inline Array<T>::Array(RRef<Array>& other) noexcept
	: Base(Mov(CastRR<Base>(other)))
{
}

template<class T>
inline Array<T>::Array(const T* data, U32 size) noexcept
	:Base(data, size)
{
}
