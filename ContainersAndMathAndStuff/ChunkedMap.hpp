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

template <class KeyType, class DataType> class ChunkedMapIterator;

/**
* ChunkedMap declaration
*
* in the chunked map each chunk is a small bounded set. If somthing needs to be placed
* into a full chunk, the first element will be pushed to the previous chunk. This is a O(1)
* operation because the chunks grow from 0 to MAX. If there is no space in the previous
* chunk, a new chunk will be placed inside the chunk container. And in case that there is no
* space for a chunk inside this container, a new container will be created inside the
* chunked map.
*/

template <class KeyType, class DataType>
class ChunkedMap
{
public:
	enum
	{
		offset = ConstexprAlign((U32)sizeof(KeyType) * 0x100, (U32)alignof(DataType)),
		combinedSize = offset + (sizeof(DataType) * 0x100)
	};

	struct Chunk
	{
		U32 size, capacity;
		KeyType* keys;
		DataType* data;

		Chunk(U32 size, U32 capacity, KeyType* keys, DataType* data);
	};

	struct ChunkContainer
	{
		U32 size, capacity;
		Chunk* chunks;

		ChunkContainer(U32 size, U32 capacity, Chunk* chunks);
	};

	U32 size, capacity;
	ChunkContainer* containers;

	ChunkedMap();

	ChunkedMap(const ChunkedMap& other);
	ChunkedMap(RRef<ChunkedMap>& other) noexcept;

	~ChunkedMap();

	ChunkedMap& operator=(const ChunkedMap& other);
	ChunkedMap& operator=(RRef<ChunkedMap>& other) noexcept;

	DataType& operator [] (U32 index);
	const DataType& operator [] (U32 index) const;

	DataType& Get(U32 index);
	const DataType& Get(U32 index) const;

	void Clear();
	ChunkContainer* _InternalGetOrCreatePreviousContainer(ChunkContainer*& container);
	Chunk* _InternalGetOrCreatePreviousChunk(ChunkContainer* container, Chunk*& chunk);

	Bool Find(const KeyType& key, U32& outIndex);

	void Add(const KeyType& key, const DataType& value);
	void Add(RRef<KeyType>& key, RRef<DataType>& value);

	// Only insert elements into an index which was acquired by a call to "Find" which returned false!
	void Insert(const KeyType& key, const DataType& value, U32 index);

	// Only insert elements into an index which was acquired by a call to "Find" which returned false!
	void Insert(RRef<KeyType>& key, RRef<DataType>& value, U32 index);

	void Remove(U32 index);

	ChunkedMapIterator<KeyType, DataType> begin();
	const ChunkedMapIterator<KeyType, DataType> begin() const;

	ChunkedMapIterator<KeyType, DataType> end();
	const ChunkedMapIterator<KeyType, DataType> end() const;
};

template <class KeyType, class DataType>
class ChunkedMapIterator
{
public:

	U16 container, chunk, index;
	ChunkedMap<KeyType, DataType>* map;

	ChunkedMapIterator();
	ChunkedMapIterator(const ChunkedMapIterator& other);
	ChunkedMapIterator(RRef<ChunkedMapIterator>& other) noexcept;
	~ChunkedMapIterator();

	ChunkedMapIterator& operator=(const ChunkedMapIterator& other);
	ChunkedMapIterator& operator=(RRef<ChunkedMapIterator>& other) noexcept;

	DataType& operator*();
	const DataType& operator*() const;
	DataType* operator->();
	const DataType* operator->() const;

	ChunkedMapIterator& operator++();
	ChunkedMapIterator operator++(int);

	ChunkedMapIterator& operator--();
	ChunkedMapIterator operator--(int);

	Bool operator==(const ChunkedMapIterator& other) const;
	Bool operator!=(const ChunkedMapIterator& other) const;
};
