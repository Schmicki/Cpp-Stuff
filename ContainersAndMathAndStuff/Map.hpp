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
* Map declaration
*/

template <class KeyType, class DataType>
class Map
{
public:

	U32 size, capacity;
	KeyType* keys;
	DataType* data;

	Map();
	Map(U32 capacity);

	Map(const Map& other);
	Map(RRef<Map>& other) noexcept;
	Map(const Pair<KeyType, DataType>* data, U32 size) noexcept;

	~Map();

	Map& operator=(const Map& other);
	Map& operator=(RRef<Map>& other) noexcept;

	DataType& operator [] (U32 index);
	const DataType& operator [] (U32 index) const;

	DataType& Get(U32 index);
	const DataType& Get(U32 index) const;

	void Reserve(U32 size);
	void Release();
	void Clear();

	Bool Find(const KeyType& key, U32& outIndex);

	void Add(const KeyType& key, const DataType& value);
	void Add(RRef<KeyType>& key, RRef<DataType>& value);

	// Only insert elements into an index which was acquired by a call to "Find" which returned false!
	void Insert(const KeyType& key, const DataType& value, U32 index);

	// Only insert elements into an index which was acquired by a call to "Find" which returned false!
	void Insert(RRef<KeyType>& key, RRef<DataType>& value, U32 index);

	void Remove(U32 index);

	DataType* begin();
	const DataType* begin() const;

	DataType* end();
	const DataType* end() const;
};
