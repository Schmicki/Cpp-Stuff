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

#include "ChunkedSet.hpp"



/**
* ChunkedSet
* 
* get index: x = SHFTD(val, 20); y = SHFTD(val & 0xFFF00, 8); z = (U8)val
* set index: val = SHFTM(x, 20) | SHFTM(y, 8) | z
*/

template<class T>
inline ChunkedSet<T>::Chunk::Chunk(U32 size, U32 capacity, T* data)
	: size(size), capacity(capacity), data(data)
{
}

template<class T>
inline ChunkedSet<T>::ChunkContainer::ChunkContainer(U32 size, U32 capacity, Chunk* chunks)
	: size(size), capacity(capacity), chunks(chunks)
{
}


template<class T>
inline ChunkedSet<T>::ChunkedSet()
	: size(0), capacity(0), containers((ChunkContainer*)nullptr)
{
}

template<class T>
inline ChunkedSet<T>::ChunkedSet(const ChunkedSet& other)
	: size(0), capacity(0), containers((ChunkContainer*)nullptr)
{
	if (other.size == 0)
		return;

	size = other.size;
	capacity = other.capacity;
	containers = Allocate<ChunkContainer>(capacity);

	for (U32 x = 0; x < other.size; x++)
	{
		ChunkContainer* container = containers + x;
		new (container) ChunkContainer(other.containers[x].size, 0x1000, Allocate<Chunk>(0x1000));

		for (U32 y = 0; y < container->size; y++)
		{
			Chunk* chunk = container->chunks + y;
			new (chunk) Chunk( other.containers[x].chunks[y].size, 0x100, Allocate<T>(0x100));
			CopyToNew(chunk->data, other.containers[x].chunks[y].data, chunk->size);
		}
	}
}

template<class T>
inline ChunkedSet<T>::ChunkedSet(RRef<ChunkedSet>& other) noexcept
	: size(other->size), capacity(other->capacity), containers(other->containers)
{
	other->size = 0;
	other->capacity = 0;
	other->containers = (ChunkContainer*)nullptr;
}

template<class T>
inline ChunkedSet<T>::~ChunkedSet()
{
	Clear();
}

template<class T>
inline ChunkedSet<T>& ChunkedSet<T>::operator=(const ChunkedSet& other)
{
	if (this == &other)
		return *this;

	Clear();

	if (other.size == 0)
		return *this;

	ChunkedSet s(other);
	return (*this = Mov(s));
}

template<class T>
inline ChunkedSet<T>& ChunkedSet<T>::operator=(RRef<ChunkedSet>& other) noexcept
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

template<class T>
inline T& ChunkedSet<T>::operator[](U32 index)
{
	U16 x = SHFTD(index, 20), y = SHFTD(index & 0xFFF00, 8), z = (U8)index;
	return containers[x].chunks[y].data[z];
}

template<class T>
inline const T& ChunkedSet<T>::operator[](U32 index) const
{
	U16 x = SHFTD(index, 20), y = SHFTD(index & 0xFFF00, 8), z = (U8)index;
	return containers[x].chunks[y].data[z];
}

template<class T>
inline T& ChunkedSet<T>::Get(U32 index)
{
	U16 x = SHFTD(index, 20), y = SHFTD(index & 0xFFF00, 8), z = (U8)index;
	return containers[x].chunks[y].data[z];
}

template<class T>
inline const T& ChunkedSet<T>::Get(U32 index) const
{
	U16 x = SHFTD(index, 20), y = SHFTD(index & 0xFFF00, 8), z = (U8)index;
	return containers[x].chunks[y].data[z];
}

template<class T>
inline void ChunkedSet<T>::Clear()
{
	if (containers)
	{
		for (U32 x = 0; x < size; x++)
		{
			for (U32 y = 0; y < containers[x].size; y++)
			{
				DestroyArrayElements(containers[x].chunks[y].data, containers[x].chunks[y].size);
				Free(containers[x].chunks[y].data);
			}
			Free(containers[x].chunks);
		}
		Free(containers);
	}

	size = 0;
	capacity = 0;
	containers = (ChunkContainer*)nullptr;
}

template<class T>
inline typename ChunkedSet<T>::ChunkContainer* ChunkedSet<T>::_InternalGetOrCreatePreviousContainer(ChunkContainer*& container)
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

template<class T>
inline typename ChunkedSet<T>::Chunk* ChunkedSet<T>::_InternalGetOrCreatePreviousChunk(ChunkContainer* container, Chunk*& chunk)
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
		container->size++;
		new (chunk) Chunk( 0, 0x100, Allocate<T>(0x100) );
		chunk++;
		return (chunk - 1);
	}

	ChunkContainer* previousContainer = _InternalGetOrCreatePreviousContainer(container);

	// place new chunk into previous container
	if (chunk == container->chunks)
	{
		Chunk* newChunk = previousContainer->chunks + previousContainer->size;
		previousContainer->size++;
		new (newChunk) Chunk( 0, 0x100, Allocate<T>(0x100) );
		return newChunk;
	}

	// place first chunk into previous container
	new (previousContainer->chunks + previousContainer->size) Chunk(Mov(*container->chunks));
	container->chunks->~Chunk();
	previousContainer->size++;

	Chunk* newChunk = chunk - 1;
	MoveToNew(container->chunks, container->chunks + 1, (U32)(newChunk - container->chunks));
	new (newChunk) Chunk( 0, 0x100, Allocate<T>(0x100) );
	return newChunk;
}

template<class T>
inline Bool ChunkedSet<T>::Find(const T& element, U32& outIndex)
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
			&& containers[x].chunks[0].data[0] <= element);

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
			&& container.chunks[y].data[0] <= element);

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
		} while (z < chunk.size && chunk.data[z] < element);

		z -= stepSize;
		SHFTDS(stepSize, 2);
	}

	while (z < chunk.size && chunk.data[z] < element) z++;

	if (z < chunk.size)
	{
		outIndex = SHFTM(x, 20) | SHFTM(y, 8) | z;
		return chunk.data[z] == element;
	}

	if (z == 0x100)
		z = 0xFF;

	outIndex = SHFTM(x, 20) | SHFTM(y, 8) | z;
	return false;
}

template<class T>
inline void ChunkedSet<T>::Add(const T& element)
{
	U32 index;
	if (!Find(element, index))
	{
		Insert(element, index);
	}
}

template<class T>
inline void ChunkedSet<T>::Add(RRef<T>& element)
{
	U32 index;
	if (!Find(*element, index))
	{
		Insert(Mov(element), index);
	}
}

template<class T>
inline void ChunkedSet<T>::Add(const ChunkedSet& other)
{
	if (size == 0)
	{
		*this = other;
		return;
	}

	for (U32 x = 0; x < other.size; x++)
		for (U32 y = 0; y < other.containers[x].size; y++)
			for (U32 z = 0; z < other.containers[x].chunks[y].size; z++)
				Add(other.containers[x].chunks[y].data[z]);
}

template<class T>
inline void ChunkedSet<T>::Add(RRef<ChunkedSet>& other)
{
	if (size == 0)
	{
		*this = Mov(other);
		return;
	}

	for (U32 x = 0; x < other->size; x++)
		for (U32 y = 0; y < other->containers[x].size; y++)
			for (U32 z = 0; z < other->containers[x].chunks[y].size; z++)
				Add(Mov(other->containers[x].chunks[y].data[z]));

	other->Clear();
}

template<class T>
inline void ChunkedSet<T>::Insert(const T& element, U32 index)
{
	if (size == 0)
	{
		containers = Allocate<ChunkContainer>(3);
		size = 1;
		capacity = 3;

		new (containers) ChunkContainer( 1, 0x1000, Allocate<Chunk>(0x1000) );
		new (containers[0].chunks) Chunk( 1, 0x100, Allocate<T>(0x100) );
		new (containers[0].chunks[0].data) T(element);
		return;
	}

	U16 x = SHFTD(index, 20), y = SHFTD(index & 0xFFF00, 8), z = (U8)index;
	Chunk* chunk = containers[x].chunks + y;

	// place element in chunk
	if (chunk->size < 0x100)
	{
		T* e = chunk->data + z;
		for (T* i = chunk->data + chunk->size; i != e; --i)
		{
			new (i) T(Mov(*(i - 1)));
			(i - 1)->~T();
		}
		chunk->size++;
		new (e) T(element);
		return;
	}

	// get or create previous chunk
	Chunk* previousChunk = _InternalGetOrCreatePreviousChunk(containers + x, chunk);

	// place element in previous chunk
	if (z == 0)
	{
		new (previousChunk->data + previousChunk->size) T(element);
		previousChunk->size++;
		return;
	}

	// move first to previous chunk
	new (previousChunk->data + previousChunk->size) T(Mov(*chunk->data));
	chunk->data->~T();
	previousChunk->size++;

	if (z != 0xFF || element < chunk->data[z]) --z;
	MoveToNew(chunk->data, chunk->data + 1, (U32)z);
	new (chunk->data + z) T(element);
}

template<class T>
inline void ChunkedSet<T>::Insert(RRef<T>& element, U32 index)
{
	if (size == 0)
	{
		containers = Allocate<ChunkContainer>(3);
		size = 1;
		capacity = 3;

		new (containers) ChunkContainer( 1, 0x1000, Allocate<Chunk>(0x1000) );
		new (containers[0].chunks) Chunk( 1, 0x100, Allocate<T>(0x100) );
		new (containers[0].chunks[0].data) T(Mov(element));
		return;
	}

	U16 x = SHFTD(index, 20), y = SHFTD(index & 0xFFF00, 8), z = (U8)index;
	Chunk* chunk = containers[x].chunks + y;

	// place element in chunk
	if (chunk->size < 0x100)
	{
		T* e = chunk->data + z;
		for (T* i = chunk->data + chunk->size; i != e; --i)
		{
			new (i) T(Mov(*(i - 1)));
			(i - 1)->~T();
		}
		chunk->size++;
		new (e) T(Mov(element));
		return;
	}

	// get or create previous chunk
	Chunk* previousChunk = _InternalGetOrCreatePreviousChunk(containers + x, chunk);

	// place element in previous chunk
	if (z == 0)
	{
		new (previousChunk->data + previousChunk->size) T(Mov(element));
		previousChunk->size++;
		return;
	}

	// move first to previous chunk
	new (previousChunk->data + previousChunk->size) T(Mov(*chunk->data));
	chunk->data->~T();
	previousChunk->size++;

	if (z != 0xFF || (*element) < chunk->data[z]) --z;
	MoveToNew(chunk->data, chunk->data + 1, (U32)z);
	new (chunk->data + z) T(Mov(element));
}

template<class T>
inline void ChunkedSet<T>::Remove(U32 index)
{
	U16 x = SHFTD(index, 20), y = SHFTD(index & 0xFFF00, 8), z = (U8)index;
	ChunkContainer* container = containers + x;
	Chunk* chunk = container->chunks + y;
	T* element = chunk->data + z;

	// remove element
	--chunk->size;
	element->~T();
	MoveToNew(element, element + 1, chunk->size - z);

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

template<class T>
inline ChunkedSetIterator<T> ChunkedSet<T>::begin()
{
	if (size == 0)
		return ChunkedSetIterator<T>();
	
	ChunkedSetIterator<T> iter;
	iter.container = 0;
	iter.chunk = 0;
	iter.index = 0;
	iter.set = this;
	return iter;
}

template<class T>
inline const ChunkedSetIterator<T> ChunkedSet<T>::begin() const
{
	if (size == 0)
		return ChunkedSetIterator<T>();

	ChunkedSetIterator<T> iter;
	iter.container = 0;
	iter.chunk = 0;
	iter.index = 0;
	iter.set = this;
	return iter;
}

template<class T>
inline ChunkedSetIterator<T> ChunkedSet<T>::end()
{
	if (size == 0)
		return ChunkedSetIterator<T>();

	ChunkedSetIterator<T> iter;
	iter.container = size;
	iter.chunk = 0;
	iter.index = 0;
	iter.set = this;
	return iter;
}

template<class T>
inline const ChunkedSetIterator<T> ChunkedSet<T>::end() const
{
	if (size == 0)
		return ChunkedSetIterator<T>();

	ChunkedSetIterator<T> iter;
	iter.container = size;
	iter.chunk = 0;
	iter.index = 0;
	iter.set = this;
	return iter;
}



/**
* ChunkedSetIterator
*/

template<class T>
inline ChunkedSetIterator<T>::ChunkedSetIterator()
	: container(0), chunk(0), index(0), set((ChunkedSet<T>*)nullptr)
{
}

template<class T>
inline ChunkedSetIterator<T>::ChunkedSetIterator(const ChunkedSetIterator& other)
	: container(other.container), chunk(other.chunk), index(other.index), set(other.set)
{
}

template<class T>
inline ChunkedSetIterator<T>::ChunkedSetIterator(RRef<ChunkedSetIterator>& other) noexcept
	: container(other->container), chunk(other->chunk), index(other->index), set(other->set)
{
	other->container = 0;
	other->chunk = 0;
	other->index = 0;
	other->set = (ChunkedSet<T>*)nullptr;
}

template<class T>
inline ChunkedSetIterator<T>::~ChunkedSetIterator()
{
}

template<class T>
inline ChunkedSetIterator<T>& ChunkedSetIterator<T>::operator=(const ChunkedSetIterator& other)
{
	container = other.container;
	chunk = other.chunk;
	index = other.index;
	set = other.set;
	return *this;
}

template<class T>
inline ChunkedSetIterator<T>& ChunkedSetIterator<T>::operator=(RRef<ChunkedSetIterator>& other) noexcept
{
	if (this == &(*other))
		return *this;

	container = other->container;
	chunk = other->chunk;
	index = other->index;
	set = other->set;

	other->container = 0;
	other->chunk = 0;
	other->index = 0;
	other->set = (ChunkedSet<T>*)nullptr;
	return *this;
}

template<class T>
inline T& ChunkedSetIterator<T>::operator*()
{
	return set->containers[container].chunks[chunk].data[index];
}

template<class T>
inline const T& ChunkedSetIterator<T>::operator*() const
{
	return set->containers[container].chunks[chunk].data[index];
}

template<class T>
inline T* ChunkedSetIterator<T>::operator->()
{
	return set->containers[container].chunks[chunk].data + index;
}

template<class T>
inline const T* ChunkedSetIterator<T>::operator->() const
{
	return set->containers[container].chunks[chunk].data + index;
}

template<class T>
inline ChunkedSetIterator<T>& ChunkedSetIterator<T>::operator++()
{
	if (container == set->size)
		return *this;

	index++;

	if (index < set->containers[container].chunks[chunk].size)
		return *this;

	index = 0;
	chunk++;

	if (chunk < set->containers[container].size)
		return *this;

	chunk = 0;
	container++;
	return *this;
}

template<class T>
inline ChunkedSetIterator<T> ChunkedSetIterator<T>::operator++(int)
{
	if (container == set->size)
		return *this;

	ChunkedSetIterator tmp = *this;

	index++;

	if (index < set->containers[container].chunks[chunk].size)
		return tmp;

	index = 0;
	chunk++;

	if (chunk < set->containers[container].size)
		return tmp;

	chunk = 0;
	container++;
	return tmp;
}

template<class T>
inline ChunkedSetIterator<T>& ChunkedSetIterator<T>::operator--()
{
	if (index > 0)
	{
		--index;
		return *this;
	}
	else if (chunk > 0)
	{
		--chunk;
		index = set->containers[container].chunks[chunk].size - 1;
		return *this;
	}
	else if (container > 0)
	{
		--container;
		chunk = set->containers[container].size - 1;
		index = set->containers[container].chunks[chunk].size - 1;
		return *this;
	}

	return *this;
}

template<class T>
inline ChunkedSetIterator<T> ChunkedSetIterator<T>::operator--(int)
{
	ChunkedSetIterator tmp = *this;

	if (index > 0)
	{
		--index;
		return tmp;
	}
	else if (chunk > 0)
	{
		--chunk;
		index = set->containers[container].chunks[chunk].size - 1;
		return tmp;
	}
	else if (container > 0)
	{
		--container;
		chunk = set->containers[container].size - 1;
		index = set->containers[container].chunks[chunk].size - 1;
		return tmp;
	}

	return tmp;
}

template<class T>
inline Bool ChunkedSetIterator<T>::operator==(const ChunkedSetIterator& other) const
{
	return container == other.container && chunk == other.chunk && index == other.index && set == other.set;
}

template<class T>
inline Bool ChunkedSetIterator<T>::operator!=(const ChunkedSetIterator& other) const
{
	return index != other.index || chunk != other.chunk || container != other.container || set != other.set;
}
