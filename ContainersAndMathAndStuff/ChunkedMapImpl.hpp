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

#include "ChunkedMap.hpp"



/**
* ChunkedMap
* 
* U32 keySize = (U32)sizeof(KeyType) * capacity;
* U32 offset = keySize % (U32)alignof(DataType);
* offset = keySize + (offset > 0 ? (U32)alignof(DataType) - offset : 0);
* U32 combinedSize = offset + (sizeof(DataType) * capacity);
* 
* keys = (KeyType*)AllocateBytes(combinedSize);
* data = (DataType*)((char*)keys + offset);
*/

template<class KeyType, class DataType>
inline ChunkedMap<KeyType, DataType>::Chunk::Chunk(U32 size, U32 capacity, KeyType* keys, DataType* data)
	: size(size), capacity(capacity), keys(keys), data(data)
{
}

template<class KeyType, class DataType>
inline ChunkedMap<KeyType, DataType>::ChunkContainer::ChunkContainer(U32 size, U32 capacity, Chunk* chunks)
	: size(size), capacity(capacity), chunks(chunks)
{
}

template<class KeyType, class DataType>
inline ChunkedMap<KeyType, DataType>::ChunkedMap()
	: size(0), capacity(0), containers((ChunkContainer*)nullptr)
{
}

template<class KeyType, class DataType>
inline ChunkedMap<KeyType, DataType>::ChunkedMap(const ChunkedMap& other)
{
	if (other.size == 0)
		return;

	size = other.size;
	capacity = other.capacity;
	containers = Allocate<ChunkContainer>(capacity);

	for (U32 x = 0; x < other.size; x++)
	{
		ChunkContainer* container = containers + x;
		new (container) ChunkContainer( other.containers[x].size, 0x1000, Allocate<Chunk>(0x1000) );

		for (U32 y = 0; y < container->size; y++)
		{
			KeyType* keys = (KeyType*)AllocateBytes(combinedSize);
			DataType* data = (DataType*)((char*)keys + offset);

			Chunk* chunk = container->chunks + y;
			new (chunk) Chunk( other.containers[x].chunks[y].size, 0x100, keys, data );
			CopyToNew(chunk->keys, other.containers[x].chunks[y].keys, chunk->size);
			CopyToNew(chunk->data, other.containers[x].chunks[y].data, chunk->size);
		}
	}
}

template<class KeyType, class DataType>
inline ChunkedMap<KeyType, DataType>::ChunkedMap(RRef<ChunkedMap>& other) noexcept
	: size(other->size), capacity(other->capacity), containers(other->containers)
{
	other->size = 0;
	other->capacity = 0;
	other->containers = (ChunkContainer*)nullptr;
}

template<class KeyType, class DataType>
inline ChunkedMap<KeyType, DataType>::~ChunkedMap()
{
	Clear();
}

template<class KeyType, class DataType>
inline ChunkedMap<KeyType, DataType>& ChunkedMap<KeyType, DataType>::operator=(const ChunkedMap& other)
{
	if (this == &other)
		return *this;

	Clear();

	if (other.size == 0)
		return *this;

	ChunkedMap s(other);
	return (*this = Mov(s));
}

template<class KeyType, class DataType>
inline ChunkedMap<KeyType, DataType>& ChunkedMap<KeyType, DataType>::operator=(RRef<ChunkedMap>& other) noexcept
{
	if (this == &(*other))
		return *this;

	size = other->size;
	capacity = other->capacity;
	containers = other->containers;

	other->size = 0;
	other->capacity = 0;
	other->containers = (ChunkContainer*)nullptr;
}

template<class KeyType, class DataType>
inline DataType& ChunkedMap<KeyType, DataType>::operator[](U32 index)
{
	U16 x = SHFTD(index, 20), y = SHFTD(index & 0xFFF00, 8), z = (U8)index;
	return containers[x].chunks[y].data[z];
}

template<class KeyType, class DataType>
inline const DataType& ChunkedMap<KeyType, DataType>::operator[](U32 index) const
{
	U16 x = SHFTD(index, 20), y = SHFTD(index & 0xFFF00, 8), z = (U8)index;
	return containers[x].chunks[y].data[z];
}

template<class KeyType, class DataType>
inline DataType& ChunkedMap<KeyType, DataType>::Get(U32 index)
{
	U16 x = SHFTD(index, 20), y = SHFTD(index & 0xFFF00, 8), z = (U8)index;
	return containers[x].chunks[y].data[z];
}

template<class KeyType, class DataType>
inline const DataType& ChunkedMap<KeyType, DataType>::Get(U32 index) const
{
	U16 x = SHFTD(index, 20), y = SHFTD(index & 0xFFF00, 8), z = (U8)index;
	return containers[x].chunks[y].data[z];
}

template<class KeyType, class DataType>
inline void ChunkedMap<KeyType, DataType>::Clear()
{
	if (containers)
	{
		for (U32 x = 0; x < size; x++)
		{
			for (U32 y = 0; y < containers[x].size; y++)
			{
				DestroyArrayElements(containers[x].chunks[y].keys, containers[x].chunks[y].size);
				DestroyArrayElements(containers[x].chunks[y].data, containers[x].chunks[y].size);
				Free(containers[x].chunks[y].keys);
			}
			Free(containers[x].chunks);
		}
		Free(containers);
	}

	size = 0;
	capacity = 0;
	containers = (ChunkContainer*)nullptr;
}

template<class KeyType, class DataType>
inline typename ChunkedMap<KeyType, DataType>::ChunkContainer* ChunkedMap<KeyType, DataType>::_InternalGetOrCreatePreviousContainer(ChunkContainer*& container)
{
	// get previous container
	if (container != containers && (container - 1)->size < 0x1000)
		return (container - 1);

	// grow container array
	if (size == capacity)
	{
		capacity = IncreaseContainerCapacity(capacity);
		ChunkContainer* oldContainers = containers;
		containers = Allocate<ChunkContainer>(capacity);
		container = containers + (container - oldContainers);
		MoveToNew(containers, oldContainers, size);
		Free(oldContainers);
	}

	// insert new container
	for (ChunkContainer* c = containers + size; c != container; --c)
	{
		new (c) ChunkContainer(Mov(*(c - 1)));
		(c - 1)->~ChunkContainer();
	}
	size++;
	new (container) ChunkContainer( 0, 0x1000, Allocate<Chunk>(0x1000) );
	container++;
	return (container - 1);
}

template<class KeyType, class DataType>
inline typename ChunkedMap<KeyType, DataType>::Chunk* ChunkedMap<KeyType, DataType>::_InternalGetOrCreatePreviousChunk(ChunkContainer* container, Chunk*& chunk)
{
	// get previous chunk
	if (chunk != container->chunks && (chunk - 1)->size < 0x100)
		return (chunk - 1);

	if (container != containers)
	{
		ChunkContainer* c = (container - 1);
		if (c->chunks[c->size - 1].size < 0x100)
			return c->chunks + (c->size - 1);
	}

	// place new chunk in container
	if (container->size < 0x1000)
	{
		// insert new chunk
		for (Chunk* c = container->chunks + container->size; c != chunk; --c)
		{
			new (c) Chunk(Mov(*(c - 1)));
			(c - 1)->~Chunk();
		}
		++container->size;
		KeyType* keys = (KeyType*)AllocateBytes(combinedSize);
		DataType* data = (DataType*)((char*)keys + offset);
		new (chunk) Chunk( 0, 0x100, keys, data );
		chunk++;
		return (chunk - 1);
	}

	ChunkContainer* previousContainer = _InternalGetOrCreatePreviousContainer(container);

	// place new chunk into previous container
	if (chunk == container->chunks)
	{
		Chunk* newChunk = previousContainer->chunks + previousContainer->size;
		++previousContainer->size;
		KeyType* keys = (KeyType*)AllocateBytes(combinedSize);
		DataType* data = (DataType*)((char*)keys + offset);
		new (newChunk) Chunk( 0, 0x100, keys, data );
		return newChunk;
	}

	// place first chunk into previous container
	new (previousContainer->chunks + previousContainer->size) Chunk(Mov(*container->chunks));
	container->chunks->~Chunk();
	++previousContainer->size;

	Chunk* newChunk = chunk - 1;
	MoveToNew(container->chunks, container->chunks + 1, (U32)(newChunk - container->chunks));
	KeyType* keys = (KeyType*)AllocateBytes(combinedSize);
	DataType* data = (DataType*)((char*)keys + offset);
	new (newChunk) Chunk( 0, 0x100, keys, data );
	return newChunk;
}

template<class KeyType, class DataType>
inline Bool ChunkedMap<KeyType, DataType>::Find(const KeyType& key, U32& outIndex)
{
	if (size == 0)
	{
		outIndex = 0;
		return false;
	}

	U32 x = 0, y = 0, z = 0, stepSize = 0x400;

	while (stepSize > 0)
	{
		do
		{
			x += stepSize;
		} while (x < size
			&& containers[x].chunks[0].keys[0] <= key);

		x -= stepSize;
		SHFTDS(stepSize, 2);
	}

	ChunkContainer& container = containers[x];
	stepSize = 0x400;

	while (stepSize > 0)
	{
		do
		{
			y += stepSize;
		} while (y < container.size
			&& container.chunks[y].keys[0] <= key);

		y -= stepSize;
		SHFTDS(stepSize, 2);
	}

	Chunk& chunk = container.chunks[y];
	stepSize = 0x40;

	while (stepSize > 1)
	{
		do
		{
			z += stepSize;
		} while (z < chunk.size && chunk.keys[z] < key);

		z -= stepSize;
		SHFTDS(stepSize, 2);
	}

	while (z < chunk.size && chunk.keys[z] < key) z++;

	if (z < chunk.size)
	{
		outIndex = SHFTM(x, 20) | SHFTM(y, 8) | z;
		return chunk.keys[z] == key;
	}

	if (z == 0x100)
		z = 0xFF;

	outIndex = SHFTM(x, 20) | SHFTM(y, 8) | z;
	return false;
}

template<class KeyType, class DataType>
inline void ChunkedMap<KeyType, DataType>::Add(const KeyType& key, const DataType& value)
{
	U32 index;
	if (!Find(key, index))
	{
		Insert(key, value, index);
	}
}

template<class KeyType, class DataType>
inline void ChunkedMap<KeyType, DataType>::Add(RRef<KeyType>& key, RRef<DataType>& value)
{
	U32 index;
	if (!Find(*key, index))
	{
		Insert(Mov(key), Mov(value), index);
	}
}

template<class KeyType, class DataType>
inline void ChunkedMap<KeyType, DataType>::Insert(const KeyType& key, const DataType& value, U32 index)
{
	if (size == 0)
	{
		containers = Allocate<ChunkContainer>(3);
		size = 1;
		capacity = 3;

		KeyType* keys = (KeyType*)AllocateBytes(combinedSize);
		DataType* data = (DataType*)((char*)keys + offset);

		new (containers) ChunkContainer( 1, 0x1000, Allocate<Chunk>(0x1000) );
		new (containers[0].chunks) Chunk( 1, 0x100, keys, data );
		new (containers[0].chunks[0].keys) KeyType(key);
		new (containers[0].chunks[0].data) DataType(value);
		return;
	}

	U16 x = SHFTD(index, 20), y = SHFTD(index & 0xFFF00, 8), z = (U8)index;
	Chunk* chunk = containers[x].chunks + y;

	// place element in chunk
	if (chunk->size < 0x100)
	{
		for (U32 i = chunk->size; i != index; --i)
		{
			new (chunk->keys + i) KeyType(Mov(chunk->keys[i - 1]));
			chunk->keys[i - 1].~KeyType();
			new (chunk->data + i) DataType(Mov(chunk->data[i - 1]));
			chunk->data[i - 1].~DataType();
		}
		++chunk->size;
		new (chunk->keys + index) KeyType(key);
		new (chunk->data + index) DataType(value);
		return;
	}

	// get or create previous chunk
	Chunk* previousChunk = _InternalGetOrCreatePreviousChunk(containers + x, chunk);

	// place element in previous chunk
	if (z == 0)
	{
		new (previousChunk->keys + previousChunk->size) KeyType(key);
		new (previousChunk->data + previousChunk->size) DataType(value);
		++previousChunk->size;
		return;
	}

	// move first to previous chunk
	new (previousChunk->keys + previousChunk->size) KeyType(Mov(*chunk->data));
	chunk->data->~KeyType();
	new (previousChunk->data + previousChunk->size) DataType(Mov(*chunk->data));
	chunk->data->~DataType();
	++previousChunk->size;

	if (z != 0xFF || key < chunk->data[z]) --z;
	MoveToNew(chunk->keys, chunk->keys + 1, (U32)z);
	MoveToNew(chunk->data, chunk->data + 1, (U32)z);
	new (chunk->keys + z) KeyType(key);
	new (chunk->data + z) DataType(value);
}

template<class KeyType, class DataType>
inline void ChunkedMap<KeyType, DataType>::Insert(RRef<KeyType>& key, RRef<DataType>& value, U32 index)
{
	if (size == 0)
	{
		containers = Allocate<ChunkContainer>(3);
		size = 1;
		capacity = 3;

		KeyType* keys = (KeyType*)AllocateBytes(combinedSize);
		DataType* data = (DataType*)((char*)keys + offset);

		new (containers) ChunkContainer( 1, 0x1000, Allocate<Chunk>(0x1000) );
		new (containers[0].chunks) Chunk( 1, 0x100, keys, data );
		new (containers[0].chunks[0].keys) KeyType(Mov(key));
		new (containers[0].chunks[0].data) DataType(Mov(value));
		return;
	}

	U16 x = SHFTD(index, 20), y = SHFTD(index & 0xFFF00, 8), z = (U8)index;
	Chunk* chunk = containers[x].chunks + y;

	// place element in chunk
	if (chunk->size < 0x100)
	{
		for (U32 i = chunk->size; i != index; --i)
		{
			new (chunk->keys + i) KeyType(Mov(chunk->keys[i - 1]));
			chunk->keys[i - 1].~KeyType();
			new (chunk->data + i) DataType(Mov(chunk->data[i - 1]));
			chunk->data[i - 1].~DataType();
		}
		++chunk->size;
		new (chunk->keys + index) KeyType(Mov(key));
		new (chunk->data + index) DataType(Mov(value));
		return;
	}

	// get or create previous chunk
	Chunk* previousChunk = _InternalGetOrCreatePreviousChunk(containers + x, chunk);

	// place element in previous chunk
	if (z == 0)
	{
		new (previousChunk->keys + previousChunk->size) KeyType(Mov(key));
		new (previousChunk->data + previousChunk->size) DataType(Mov(value));
		++previousChunk->size;
		return;
	}

	// move first to previous chunk
	new (previousChunk->keys + previousChunk->size) KeyType(Mov(*chunk->data));
	chunk->data->~KeyType();
	new (previousChunk->data + previousChunk->size) DataType(Mov(*chunk->data));
	chunk->data->~DataType();
	++previousChunk->size;

	if (z != 0xFF || key < chunk->data[z]) --z;
	MoveToNew(chunk->keys, chunk->keys + 1, (U32)z);
	MoveToNew(chunk->data, chunk->data + 1, (U32)z);
	new (chunk->keys + z) KeyType(Mov(key));
	new (chunk->data + z) DataType(Mov(value));
}

template<class KeyType, class DataType>
inline void ChunkedMap<KeyType, DataType>::Remove(U32 index)
{
	U16 x = SHFTD(index, 20), y = SHFTD(index & 0xFFF00, 8), z = (U8)index;
	ChunkContainer* container = containers + x;
	Chunk* chunk = container->chunks + y;
	KeyType* key = chunk->keys + z;
	DataType* value = chunk->data + z;

	// remove element
	--chunk->size;
	key->~KeyType();
	value->~DataType();
	MoveToNew(key, key + 1, chunk->size - z);
	MoveToNew(value, value + 1, chunk->size - z);

	// remove chunk
	if (chunk->size > 0)
		return;

	--container->size;
	Free(chunk->data);
	chunk->~Chunk();
	MoveToNew(chunk, chunk + 1, container->size - y);

	// remove container
	if (container->size > 0)
		return;

	--size;
	Free(container->chunks);
	container->~ChunkContainer();
	MoveToNew(container, container + 1, size - x);

	// clean
	if (size < SHFTD(capacity, 2))
	{
		SHFTDS(capacity, 1);
		ChunkContainer* oldContainers = containers;
		containers = Allocate<ChunkContainer>(capacity);
		MoveToNew(containers, oldContainers, size);
		Free(oldContainers);
		return;
	}
	else if (size == 0)
	{
		Free(containers);
		size = 0;
		capacity = 0;
		containers = (ChunkContainer*)nullptr;
		return;
	}
}

template<class KeyType, class DataType>
inline ChunkedMapIterator<KeyType, DataType> ChunkedMap<KeyType, DataType>::begin()
{
	if (size == 0)
		return ChunkedMapIterator<KeyType, DataType>();

	ChunkedMapIterator<KeyType, DataType> iter;
	iter.container = 0;
	iter.chunk = 0;
	iter.index = 0;
	iter.map = this;
	return iter;
}

template<class KeyType, class DataType>
inline const ChunkedMapIterator<KeyType, DataType> ChunkedMap<KeyType, DataType>::begin() const
{
	if (size == 0)
		return ChunkedMapIterator<KeyType, DataType>();

	ChunkedMapIterator<KeyType, DataType> iter;
	iter.container = 0;
	iter.chunk = 0;
	iter.index = 0;
	iter.map = this;
	return iter;
}

template<class KeyType, class DataType>
inline ChunkedMapIterator<KeyType, DataType> ChunkedMap<KeyType, DataType>::end()
{
	if (size == 0)
		return ChunkedMapIterator<KeyType, DataType>();

	ChunkedMapIterator<KeyType, DataType> iter;
	iter.container = size;
	iter.chunk = 0;
	iter.index = 0;
	iter.map = this;
	return iter;
}

template<class KeyType, class DataType>
inline const ChunkedMapIterator<KeyType, DataType> ChunkedMap<KeyType, DataType>::end() const
{
	if (size == 0)
		return ChunkedMapIterator<KeyType, DataType>();

	ChunkedMapIterator<KeyType, DataType> iter;
	iter.container = size;
	iter.chunk = 0;
	iter.index = 0;
	iter.map = this;
	return iter;
}



/**
* ChunkMapIterator
*/

template<class KeyType, class DataType>
inline ChunkedMapIterator<KeyType, DataType>::ChunkedMapIterator()
	: container(0), chunk(0), index(0), map((ChunkedMap<KeyType, DataType>*)nullptr)
{
}

template<class KeyType, class DataType>
inline ChunkedMapIterator<KeyType, DataType>::ChunkedMapIterator(const ChunkedMapIterator& other)
	: container(other.container), chunk(other.chunk), index(other.index), map(other.map)
{
}

template<class KeyType, class DataType>
inline ChunkedMapIterator<KeyType, DataType>::ChunkedMapIterator(RRef<ChunkedMapIterator>& other) noexcept
	: container(other->container), chunk(other->chunk), index(other->index), map(other->map)
{
	other->container = 0;
	other->chunk = 0;
	other->index = 0;
	other->map = (ChunkedMap<KeyType, DataType>*)nullptr;
}

template<class KeyType, class DataType>
inline ChunkedMapIterator<KeyType, DataType>::~ChunkedMapIterator()
{
}

template<class KeyType, class DataType>
inline ChunkedMapIterator<KeyType, DataType>& ChunkedMapIterator<KeyType, DataType>::operator=(const ChunkedMapIterator& other)
{
	container = other.container;
	chunk = other.chunk;
	index = other.index;
	map = other.map;
	return *this;
}

template<class KeyType, class DataType>
inline ChunkedMapIterator<KeyType, DataType>& ChunkedMapIterator<KeyType, DataType>::operator=(RRef<ChunkedMapIterator>& other) noexcept
{
	if (this == &(*other))
		return *this;

	container = other->container;
	chunk = other->chunk;
	index = other->index;
	map = other->map;

	other->container = 0;
	other->chunk = 0;
	other->index = 0;
	other->map = (ChunkedMap<KeyType, DataType>*)nullptr;
	return *this;
}

template<class KeyType, class DataType>
inline DataType& ChunkedMapIterator<KeyType, DataType>::operator*()
{
	return map->containers[container].chunks[chunk].data[index];
}

template<class KeyType, class DataType>
inline const DataType& ChunkedMapIterator<KeyType, DataType>::operator*() const
{
	return map->containers[container].chunks[chunk].data[index];
}

template<class KeyType, class DataType>
inline DataType* ChunkedMapIterator<KeyType, DataType>::operator->()
{
	return map->containers[container].chunks[chunk].data + index;
}

template<class KeyType, class DataType>
inline const DataType* ChunkedMapIterator<KeyType, DataType>::operator->() const
{
	return map->containers[container].chunks[chunk].data + index;
}

template<class KeyType, class DataType>
inline ChunkedMapIterator<KeyType, DataType>& ChunkedMapIterator<KeyType, DataType>::operator++()
{
	if (container == map->size)
		return *this;

	index++;

	if (index < map->containers[container].chunks[chunk].size)
		return *this;

	index = 0;
	chunk++;

	if (chunk < map->containers[container].size)
		return *this;

	chunk = 0;
	container++;
	return *this;
}

template<class KeyType, class DataType>
inline ChunkedMapIterator<KeyType, DataType> ChunkedMapIterator<KeyType, DataType>::operator++(int)
{
	if (container == map->size)
		return *this;

	ChunkedMapIterator tmp = *this;

	index++;

	if (index < map->containers[container].chunks[chunk].size)
		return tmp;

	index = 0;
	chunk++;

	if (chunk < map->containers[container].size)
		return tmp;

	chunk = 0;
	container++;
	return tmp;
}

template<class KeyType, class DataType>
inline ChunkedMapIterator<KeyType, DataType>& ChunkedMapIterator<KeyType, DataType>::operator--()
{
	if (index > 0)
	{
		--index;
		return *this;
	}
	else if (chunk > 0)
	{
		--chunk;
		index = map->containers[container].chunks[chunk].size - 1;
		return *this;
	}
	else if (container > 0)
	{
		--container;
		chunk = map->containers[container].size - 1;
		index = map->containers[container].chunks[chunk].size - 1;
		return *this;
	}

	return *this;
}

template<class KeyType, class DataType>
inline ChunkedMapIterator<KeyType, DataType> ChunkedMapIterator<KeyType, DataType>::operator--(int)
{
	ChunkedMapIterator tmp = *this;

	if (index > 0)
	{
		--index;
		return tmp;
	}
	else if (chunk > 0)
	{
		--chunk;
		index = map->containers[container].chunks[chunk].size - 1;
		return tmp;
	}
	else if (container > 0)
	{
		--container;
		chunk = map->containers[container].size - 1;
		index = map->containers[container].chunks[chunk].size - 1;
		return tmp;
	}

	return tmp;
}

template<class KeyType, class DataType>
inline Bool ChunkedMapIterator<KeyType, DataType>::operator==(const ChunkedMapIterator& other) const
{
	return container == other.container && chunk == other.chunk && index == other.index && map == other.map;
}

template<class KeyType, class DataType>
inline Bool ChunkedMapIterator<KeyType, DataType>::operator!=(const ChunkedMapIterator& other) const
{
	return index != other.index || chunk != other.chunk || container != other.container || map != other.map;
}
