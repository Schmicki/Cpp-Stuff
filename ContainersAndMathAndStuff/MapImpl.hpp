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

template<class KeyType, class DataType>
inline Map<KeyType, DataType>::Map()
	: size(0), capacity(0), keys((KeyType*)nullptr), data((DataType*)nullptr)
{
}

template<class KeyType, class DataType>
inline Map<KeyType, DataType>::Map(U32 capacity)
	: size(0), capacity(0), keys((KeyType*)nullptr), data((DataType*)nullptr)
{
	Reserve(capacity);
}

template<class KeyType, class DataType>
inline Map<KeyType, DataType>::Map(const Map& other)
	: size(0), capacity(0), keys((KeyType*)nullptr), data((DataType*)nullptr)
{
	if (other.size == 0)
	{
		return;
	}

	Reserve(other.size);
	size = other.size;
	CopyToNew(keys, other.keys, other.size);
	CopyToNew(data, other.data, other.size);
}

template<class KeyType, class DataType>
inline Map<KeyType, DataType>::Map(RRef<Map>& other) noexcept
	: size(other->size), capacity(other->capacity), keys(other->keys), data(other->data)
{
	other->size = 0;
	other->capacity = 0;
	other->keys = (KeyType*)nullptr;
	other->data = (DataType*)nullptr;
}

template<class KeyType, class DataType>
inline Map<KeyType, DataType>::Map(const Pair<KeyType, DataType>* data, U32 size) noexcept
	: size(0), capacity(0), keys((KeyType*)nullptr), data((DataType*)nullptr)
{
	Reserve(size);
	for (U32 i = 0; i < size; i++)
		Add(data[i].a, data[i].b);
}

template<class KeyType, class DataType>
inline Map<KeyType, DataType>::~Map()
{
	if (keys)
	{
		DestroyArrayElements(keys, size);
		DestroyArrayElements(data, size);
		Free(keys);
	}
}

template<class KeyType, class DataType>
inline Map<KeyType, DataType>& Map<KeyType, DataType>::operator=(const Map& other)
{
	if (this != &other)
	{
		Clear();

		if (other.size == 0)
			return *this;


		Reserve(other.size);
		CopyToNew(keys, other.keys, other.size);
		CopyToNew(data, other.data, other.size);

		size = other.size;
	}
	return *this;
}

template<class KeyType, class DataType>
inline Map<KeyType, DataType>& Map<KeyType, DataType>::operator=(RRef<Map>& other) noexcept
{
	if (this != &(*other))
	{
		Clear();

		size = other->size;
		capacity = other->capacity;
		keys = other->keys;
		data = other->data;

		other->size = 0;
		other->capacity = 0;
		other->keys = (KeyType*)nullptr;
		other->data = (DataType*)nullptr;
	}
	return *this;
}

template<class KeyType, class DataType>
inline DataType& Map<KeyType, DataType>::operator[](U32 index)
{
	return data[index];
}

template<class KeyType, class DataType>
inline const DataType& Map<KeyType, DataType>::operator[](U32 index) const
{
	return data[index];
}

template<class KeyType, class DataType>
inline DataType& Map<KeyType, DataType>::Get(U32 index)
{
	return data[index];
}

template<class KeyType, class DataType>
inline const DataType& Map<KeyType, DataType>::Get(U32 index) const
{
	return data[index];
}

template<class KeyType, class DataType>
inline void Map<KeyType, DataType>::Reserve(U32 size)
{

	if (size <= capacity) return;

	capacity = CalculateContainerCapacity(size);

	KeyType* oldKeys = keys;
	DataType* oldData = data;

	U32 offset = Align((U32)sizeof(KeyType) * capacity, (U32)alignof(DataType));
	U32 combinedSize = offset + (sizeof(DataType) * capacity);

	keys = (KeyType*)AllocateBytes(combinedSize);
	data = (DataType*)((char*)keys + offset);

	MoveToNew(keys, oldKeys, this->size);
	MoveToNew(data, oldData, this->size);

	if (oldKeys) Free(oldKeys);
}

template<class KeyType, class DataType>
inline void Map<KeyType, DataType>::Release()
{
	if ((capacity / 4) > size)
	{
		capacity /= 2;
		KeyType* oldKeys = keys;
		DataType* oldData = data;

		U32 offset = Align((U32)sizeof(KeyType) * capacity, (U32)alignof(DataType));
		U32 combinedSize = offset + (sizeof(DataType) * capacity);

		keys = (KeyType*)AllocateBytes(combinedSize);
		data = (DataType*)((char*)keys + offset);

		MoveToNew(keys, oldKeys, size);
		MoveToNew(data, oldData, size);

		if (oldData) Free(oldData);
	}
}

template<class KeyType, class DataType>
inline void Map<KeyType, DataType>::Clear()
{
	if (keys)
	{
		DestroyArrayElements(keys, size);
		DestroyArrayElements(data, size);
		Free(keys);

		size = 0;
		capacity = 0;
		keys = (KeyType*)nullptr;
		data = (DataType*)nullptr;
	}
}

template<class KeyType, class DataType>
inline Bool Map<KeyType, DataType>::Find(const KeyType& key, U32& outIndex)
{
	outIndex = 0;
	U32 stepSize = 0x400;
	while (stepSize > 1)
	{
		do
		{
			outIndex += stepSize;
		} while (outIndex < size && keys[outIndex] < key);
		outIndex -= stepSize;
		SHFTDS(stepSize, 2);
	}

	for (; outIndex < size && keys[outIndex] < key; outIndex++);
	return outIndex < size && !(key < keys[outIndex]);
}

template<class KeyType, class DataType>
inline void Map<KeyType, DataType>::Add(const KeyType& key, const DataType& value)
{
	U32 index;
	if (!Find(key, index))
	{
		Reserve(size + 1);

		for (U32 i = size; i > index; --i)
		{
			new (keys + i) KeyType(Mov(keys[i - 1]));
			keys[i - 1].~KeyType();
			new (data + i) DataType(Mov(data[i - 1]));
			data[i - 1].~DataType();
		}

		new (keys + index) KeyType(key);
		new (data + index) DataType(value);
		size++;
	}
}

template<class KeyType, class DataType>
inline void Map<KeyType, DataType>::Add(RRef<KeyType>& key, RRef<DataType>& value)
{
	U32 index;
	if (!Find(*key, index))
	{
		Reserve(size + 1);

		for (U32 i = size; i > index; --i)
		{
			new (keys + i) KeyType(Mov(keys[i - 1]));
			keys[i - 1].~KeyType();
			new (data + i) DataType(Mov(data[i - 1]));
			data[i - 1].~DataType();
		}

		new (keys + index) KeyType(Mov(key));
		new (data + index) DataType(Mov(value));
		size++;
	}
}

template<class KeyType, class DataType>
inline void Map<KeyType, DataType>::Insert(const KeyType& key, const DataType& value, U32 index)
{
	Reserve(size + 1);

	for (U32 i = size; i > index; --i)
	{
		new (keys + i) KeyType(Mov(keys[i - 1]));
		keys[i - 1].~KeyType();
		new (data + i) DataType(Mov(data[i - 1]));
		data[i - 1].~DataType();
	}

	new (keys + index) KeyType(key);
	new (data + index) DataType(value);
	size++;
}

template<class KeyType, class DataType>
inline void Map<KeyType, DataType>::Insert(RRef<KeyType>& key, RRef<DataType>& value, U32 index)
{
	Reserve(size + 1);

	for (U32 i = size; i > index; --i)
	{
		new (keys + i) KeyType(Mov(keys[i - 1]));
		keys[i - 1].~KeyType();
		new (data + i) DataType(Mov(data[i - 1]));
		data[i - 1].~DataType();
	}

	new (keys + index) KeyType(Mov(key));
	new (data + index) DataType(Mov(value));
	size++;
}

template<class KeyType, class DataType>
inline void Map<KeyType, DataType>::Remove(U32 index)
{
	keys[index].~KeyType();
	data[index].~DataType();
	--size;
	MoveToNew(keys + index, keys + index + 1, size - index);
	MoveToNew(data + index, data + index + 1, size - index);
	Release();
}

template<class KeyType, class DataType>
inline DataType* Map<KeyType, DataType>::begin()
{
	return data;
}

template<class KeyType, class DataType>
inline const DataType* Map<KeyType, DataType>::begin() const
{
	return data;
}

template<class KeyType, class DataType>
inline DataType* Map<KeyType, DataType>::end()
{
	return data + size;
}

template<class KeyType, class DataType>
inline const DataType* Map<KeyType, DataType>::end() const
{
	return data + size;
}
