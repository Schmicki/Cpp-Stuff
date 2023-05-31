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

// forward declarations



/**
* Set declaration
*/

template <class T>
class Set
{
public:

	U32 size, capacity;
	T* data;

	Set();
	Set(U32 capacity);

	Set(const Set& other);
	Set(RRef<Set>& other) noexcept;
	Set(const T* data, U32 size) noexcept;

	~Set();

	Set& operator=(const Set& other);
	Set& operator=(RRef<Set>& other) noexcept;

	T& operator [] (U32 index);
	const T& operator [] (U32 index) const;

	T& Get(U32 index);
	const T& Get(U32 index) const;

	void Reserve(U32 size);
	void Release();
	void Clear();

	Bool Find(const T& element, U32& outIndex);

	void Add(const T& element);
	void Add(RRef<T>& element);
	void Add(const Set& other);
	void Add(RRef<Set>& other);

	// Only insert elements into an index which was acquired by a call to "Find" which returned false!
	void Insert(const T& element, U32 index);

	// Only insert elements into an index which was acquired by a call to "Find" which returned false!
	void Insert(RRef<T>& element, U32 index);

	void Remove(U32 index);

	T* begin();
	const T* begin() const;

	T* end();
	const T* end() const;
};
