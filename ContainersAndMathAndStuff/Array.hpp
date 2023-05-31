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

#include "ContainersBase.hpp"

// Forward declarations



/**
* Array declaration
*/

template <class T>
class _ArrayBase
{
public:

	_ArrayBase() noexcept;
	_ArrayBase(U32 capacity) noexcept;
	_ArrayBase(const T* data, U32 size) noexcept;

	_ArrayBase(const _ArrayBase& other) noexcept;
	_ArrayBase(RRef<_ArrayBase>& other) noexcept;

	U32 size, capacity;
	T* data;

	~_ArrayBase();

	_ArrayBase& operator=(const _ArrayBase& other) noexcept;
	_ArrayBase& operator=(RRef<_ArrayBase>& other) noexcept;

	T& operator [] (U32 index);
	const T& operator [] (U32 index) const;

	T& Get(U32 index);
	const T& Get(U32 index) const;

	void Reserve(U32 size);
	void Release();
	void Resize(U32 size, const T& defaultElement = T());
	void Clear();

	Bool Find(const T& element, U32& outIndex);

	void Add(const T& element);
	void Add(RRef<T>& element);
	void Add(const _ArrayBase<T>& other);
	void Add(RRef<_ArrayBase>& other);

	void AddUnique(const T& element);
	void AddUnique(RRef<T>& element);

	void Insert(const T& element, U32 index);
	void Insert(RRef<T>& element, U32 index);
	void Insert(const _ArrayBase& other, U32 index);
	void Insert(RRef<_ArrayBase>& other, U32 index);

	void Remove(U32 index);
	void Remove(U32 first, U32 last);

	T* begin();
	const T* begin() const;

	T* end();
	const T* end() const;
};


template <class T>
class _NumericArray : public _ArrayBase<T>
{
public:

	typedef _ArrayBase<T> Base;

	_NumericArray() noexcept;
	_NumericArray(U32 capacity) noexcept;

	_NumericArray(const _NumericArray& other) noexcept;
	_NumericArray(RRef<_NumericArray>& other) noexcept;
	_NumericArray(const T* data, U32 size) noexcept;

	T Min();
	U32 MinIdx();
	T AbsMin();
	U32 AbsMinIdx();
	T Max();
	U32 MaxIdx();
	T AbsMax();
	U32 AbsMaxIdx();
	T Sum();
	T AbsSum();
	T Avg();
	T AbsAvg();
};


template <class T>
class Array : public SelectType<_NumericArray<T>, _ArrayBase<T>, IsIntegral<T>::Value || IsFloat<T>::Value>::Type
{
public:

	typedef typename SelectType<_NumericArray<T>, _ArrayBase<T>, IsIntegral<T>::Value || IsFloat<T>::Value>::Type Base;

	Array() noexcept;
	Array(U32 capacity) noexcept;

	Array(const Array& other) noexcept;
	Array(RRef<Array>& other) noexcept;
	Array(const T* data, U32 size) noexcept;
};
