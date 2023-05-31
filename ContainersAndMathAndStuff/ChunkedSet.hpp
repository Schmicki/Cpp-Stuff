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

template <class T> class ChunkedSetIterator;

/**
* ChunkedSet declaration
* 
* in the chunked set each chunk is a small bounded set. If somthing needs to be placed
* into a full chunk, the first element will be pushed to the previous chunk. This is a O(1)
* operation because the chunks grow from 0 to MAX. If there is no space in the previous
* chunk, a new chunk will be placed inside the chunk container. And in case that there is no
* space for a chunk inside this container, a new container will be created inside the
* chunked set.
*/

template <class T>
class ChunkedSet
{
public:

	struct Chunk
	{
		U32 size, capacity;
		T* data;

		Chunk(U32 size, U32 capacity, T* data);
	};

	struct ChunkContainer
	{
		U32 size, capacity;
		Chunk* chunks;

		ChunkContainer(U32 size, U32 capacity, Chunk* chunks);
	};

	U32 size, capacity;
	ChunkContainer* containers;

	ChunkedSet();

	ChunkedSet(const ChunkedSet& other);
	ChunkedSet(RRef<ChunkedSet>& other) noexcept;

	~ChunkedSet();

	ChunkedSet& operator=(const ChunkedSet& other);
	ChunkedSet& operator=(RRef<ChunkedSet>& other) noexcept;

	T& operator [] (U32 index);
	const T& operator [] (U32 index) const;

	T& Get(U32 index);
	const T& Get(U32 index) const;

	void Clear();
	ChunkContainer* _InternalGetOrCreatePreviousContainer(ChunkContainer*& container);
	Chunk* _InternalGetOrCreatePreviousChunk(ChunkContainer* container, Chunk*& chunk);

	Bool Find(const T& element, U32& outIndex);

	void Add(const T& element);
	void Add(RRef<T>& element);
	void Add(const ChunkedSet& other);
	void Add(RRef<ChunkedSet>& other);

	// Only insert elements into an index which was acquired by a call to "Find" which returned false!
	void Insert(const T& element, U32 index);

	// Only insert elements into an index which was acquired by a call to "Find" which returned false!
	void Insert(RRef<T>& element, U32 index);

	void Remove(U32 index);

	ChunkedSetIterator<T> begin();
	const ChunkedSetIterator<T> begin() const;

	ChunkedSetIterator<T> end();
	const ChunkedSetIterator<T> end() const;
};

template <class T>
class ChunkedSetIterator
{
public:

	U16 container, chunk, index;
	ChunkedSet<T>* set;

	ChunkedSetIterator();
	ChunkedSetIterator(const ChunkedSetIterator& other);
	ChunkedSetIterator(RRef<ChunkedSetIterator>& other) noexcept;
	~ChunkedSetIterator();

	ChunkedSetIterator& operator=(const ChunkedSetIterator& other);
	ChunkedSetIterator& operator=(RRef<ChunkedSetIterator>& other) noexcept;

	T& operator*();
	const T& operator*() const;
	T* operator->();
	const T* operator->() const;

	ChunkedSetIterator& operator++();
	ChunkedSetIterator operator++(int);

	ChunkedSetIterator& operator--();
	ChunkedSetIterator operator--(int);

	Bool operator==(const ChunkedSetIterator& other) const;
	Bool operator!=(const ChunkedSetIterator& other) const;
};