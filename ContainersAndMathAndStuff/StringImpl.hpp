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

#include "String.hpp"
#include "Array.hpp"
#include "Templates.hpp"

inline String::String()
	: size(0), capacity(0), data((C8*)nullptr)
{
}

inline String::String(const char* str)
	: size(0), capacity(0), data((C8*)nullptr)
{
	U32 _size = UTF8CountArray(str);
	Reserve(_size);
	memcpy(data, str, _size + 1);
	this->size = _size;
}

inline String::String(const wchar_t* str)
	: size(0), capacity(0), data((C8*)nullptr)
{
	String tmp((SelectType<C16*, C32*, sizeof(wchar_t) == 2>::Type)str);
	*this = Mov(tmp);
}

inline String::String(const U16* str)
	: size(0), capacity(0), data((C8*)nullptr)
{
	size = UTF16CountUTF8(str);
	capacity = CalculateContainerCapacity(size);
	data = (char*)malloc(capacity);
	data[size] = 0;

	for (U32 i = 0, ix = 0, symbol = 0; str[i] != 0;)
	{
		i += UTF16DecodeSymbol(str + i, symbol);
		ix += UTF8EncodeSymbol(data + ix, symbol);
	}
}

inline String::String(const U32* str)
	: size(0), capacity(0), data((C8*)nullptr)
{
	size = UTF32CountUTF8(str);
	capacity = CalculateContainerCapacity(size);
	data = (char*)malloc(capacity);
	data[size] = 0;

	for (U32 i = 0, ix = 0; str[i] != 0; i++)
	{
		ix += UTF8EncodeSymbol(data + ix, str[i]);
	}
}

inline String::String(const String& other)
	: size(0), capacity(0), data((C8*)nullptr)
{
	if (other.size > 0)
	{
		size = other.size;
		capacity = other.capacity;
		data = (char*)malloc(capacity);
		memcpy(data, other.data, size + 1);
	}
}

inline String::String(RRef<String>& other) noexcept
	: size(other->size), capacity(other->capacity), data(other->data)
{
	other->size = 0;
	other->capacity = 0;
	other->data = (C8*)nullptr;
}

inline String::~String()
{
	if (data)
		Free(data);
}

inline String& String::operator=(const String& other)
{
	if (this != &other)
	{
		if (data)
			Free(data);
		
		if (other.size == 0)
		{
			size = 0;
			capacity = 0;
			data = (C8*)nullptr;
			return *this;
		}

		size = other.size;
		capacity = other.capacity;
		data = (char*)malloc(capacity);
		memcpy(data, other.data, size + 1);
	}
	return *this;
}

inline String& String::operator=(RRef<String>& other) noexcept
{
	if (this != &(*other))
	{
		if (data)
			Free(data);

		size = other->size;
		capacity = other->capacity;
		data = other->data;

		other->data = (C8*)nullptr;
		other->size = 0;
		other->capacity = 0;
	}
	return *this;
}

inline String String::operator+(const String& other)
{
	if ((size + other.size) > 0)
	{
		String tmp;
		tmp.Reserve(size + other.size);
		tmp.size = size + other.size;
		tmp.data[tmp.size] = 0;
		memcpy(tmp.data, data, size);
		memcpy(tmp.data + size, other.data, other.size);
		return tmp;
	}

	return String();
}

inline String& String::operator+=(const String& other)
{
	if ((size + other.size) > 0)
	{
		Reserve(size + other.size);
		memcpy(data + size, other.data, other.size);
		size += other.size;
		data[size] = 0;
	}
	return *this;
}

inline String String::operator+(const char* other)
{
	String tmp;
	U32 otherSize = UTF8CountArray(other);
	U32 combinedSize = size + otherSize;

	if (combinedSize > 0)
	{
		tmp.Reserve(combinedSize);
		memcpy(tmp.data, data, size);
		memcpy(tmp.data + size, other, otherSize);
		tmp.data[combinedSize] = 0;
		tmp.size = combinedSize;
	}
	return tmp;
}

inline String& String::operator+=(const char* other)
{
	U32 otherSize = UTF8CountArray(other);
	U32 combinedSize = size + otherSize;

	if (combinedSize > 0)
	{
		Reserve(combinedSize);
		memcpy(data + size, other, otherSize);
		data[combinedSize] = 0;
		size = combinedSize;
	}
	return *this;
}

inline char& String::operator[](U32 index)
{
	return data[index];
}

inline const char& String::operator[](U32 index) const
{
	return data[index];
}

inline void String::Reserve(U32 size)
{
	if (size <= capacity) return;

	capacity = CalculateContainerCapacity(size);
	char* oldData = data;
	data = (char*)malloc(capacity);
	data[this->size] = 0;
	memcpy(data, oldData, (UPtr)this->size);

	if (oldData)
		Free(oldData);
}

inline void String::Release()
{
	if ((capacity / 4) > size)
	{
		capacity /= 2;
		char* oldData = data;
		data = (char*)malloc(capacity);
		data[size] = 0;
		memcpy(data, oldData, size);

		if (oldData)
			Free(oldData);
	}
}

inline void String::Resize(U32 size, char defaultChar)
{
	if (size == this->size)
		return;

	if (size == 0)
	{
		if (data)
			Free(data);

		size = 0;
		capacity = 0;
		data = (C8*)nullptr;
		return;
	}

	if (size > capacity || size < SHFTD(this->capacity, 1))
	{
		capacity = CalculateContainerCapacity(size);
		char* oldData = data;
		data = (char*)malloc(capacity);
		data[size] = 0;

		if (size < this->size)
		{
			memcpy(data, oldData, size);
		}
		else
		{
			memcpy(data, oldData, this->size);
			for (U32 i = this->size; i < size; data[i] = defaultChar, i++);
		}

		this->size = size;

		if (oldData)
			Free(oldData);

		return;
	}
	else
	{
		data[size] = 0;

		if (size > this->size)
		{
			for (U32 i = this->size; i < size; data[i] = defaultChar, i++);
		}

		this->size = size;
	}
}

inline char& String::Get(U32 index)
{
	return data[index];
}

inline const char& String::Get(U32 index) const
{
	return data[index];
}

inline void String::Add(char c)
{
	Reserve(size + 1);
	data[size] = c;

	size++;
	data[size] = 0;
}

inline void String::Add(const char* str, U32 len)
{
	Reserve(size + len);
	memcpy(data + size, str, len);
	size += len;
	data[size] = 0;
}

inline void String::Add(const char* str)
{
	Add(str, UTF8CountArray(str));
}

inline void String::Add(const String& str)
{
	Add(str.data, str.size);
}

inline void String::Insert(char c, U32 index)
{
	if (index >= size)
		return Add(c);

	Reserve(size + 1);
	
	for (U32 i = size; i > index; data[i] = data[i - 1], --i);

	size++;
	data[index] = c;
	data[size] = 0;
}

inline void String::Insert(const char* str, U32 len, U32 index)
{
	if (index >= size)
		return Add(str, len);

	Reserve(size + len);
	
	U32 end = index + len;
	for (U32 i = size + len - 1; i > end; data[i] = data[i - len], --i);

	memcpy(data + index, str, len);
	size += len;
	data[size] = 0;
}

inline void String::Insert(const char* str, U32 index)
{
	Insert(str, UTF8CountArray(str), index);
}

inline void String::Insert(const String& str, U32 index)
{
	Insert(str.data, str.size, index);
}

inline void String::Remove(U32 index)
{
	if (index >= size)
		return;

	memcpy(data + index, data + index + 1, size - index - 1);
	size--;
	data[size] = 0;

	Release();
}

inline void String::Remove(U32 first, U32 last)
{
	if (first >= size || first > last)
		return;

	if (last >= size)
		last = size - 1;

	memcpy(data + first, data + last + 1, size - last - 1);
	size -= last - first + 1;
	Release();
}

inline void String::Remove(char c, U32 offset, U32 count)
{
	U32 index = offset;
	for (U32 i = 0; i < count && Find(c, index, index); i++)
	{
		Remove(index);
	}
}

inline void String::Remove(const char* str, U32 len, U32 offset, U32 count)
{
	U32 index = offset;
	for (U32 i = 0; i < count && Find(str, index, index); i++)
	{
		Remove(index, index + len - 1);
	}
}

inline void String::Remove(const char* str, U32 offset, U32 count)
{
	Remove(str, UTF8CountArray(str), offset, count);
}

inline void String::Remove(const String& str, U32 offset, U32 count)
{
	Remove(str.data, str.size, offset, count);
}

inline void String::SubString(U32 start, U32 end, String& outString) const
{
	if (start > end || start >= size)
		return;

	end = (end >= size) ? size : end + 1;
	end = end - start;

	outString.Reserve(end);
	outString.size = end;
	outString[end] = 0;
	memcpy(outString.data, data + start, end);
}

inline void String::Split(U32 firstRight, String& outLeft, String& outRight) const
{
	SubString(0, firstRight - 1, outLeft);
	SubString(firstRight, size - 1, outRight);
}

inline void String::Split(C8 InSeperator, Array<String>& OutTokens) const
{
	if (size == 0) return;

	U32 StartIndex = 0, Index;
	while (Find(InSeperator, StartIndex, Index))
	{
		if (Index == StartIndex)
		{
			StartIndex = ++Index;
			continue;
		}

		OutTokens.Add(String());
		SubString(StartIndex, Index - 1, OutTokens.Get(OutTokens.size - 1));
		StartIndex = ++Index;
	}
	
	if (StartIndex == size) return;

	OutTokens.Add(String());
	SubString(StartIndex, size - 1, OutTokens.Get(OutTokens.size - 1));
}

inline U16* String::ToUTF16() const
{
	U32 convertedSize = UTF8CountUTF16(data);

	C16* string = Allocate<C16>((UPtr)convertedSize + 1);
	string[convertedSize] = 0;

	for (U32 i = 0, ix = 0, symbol = 0; i < size;)
	{
		i += UTF8DecodeSymbol(data + i, symbol);
		ix += UTF16EncodeSymbol(string + ix, symbol);
	}

	return string;
}

inline U32* String::ToUTF32() const
{
	U32 convertedSize = UTF8CountSymbols(data);

	C32* string = Allocate<C32>((UPtr)convertedSize + 1);
	string[convertedSize] = 0;

	for (U32 i = 0, ix = 0, symbol = 0; i < size; ix++)
	{
		i += UTF8DecodeSymbol(data + i, symbol);
		string[ix] = symbol;
	}

	return string;
}

inline wchar_t* String::ToWString() const
{
	if (sizeof(wchar_t) == 4)
	{
		(wchar_t*)ToUTF32();
	}
	else
	{
		return (wchar_t*)ToUTF16();
	}
}

inline Bool String::BeginsWith(const char* str, U32 len) const
{
	if (len > size)
		return false;

	U32 i = 0;
	for (; data[i] == str[i] && str[i] != 0; i++);
	return str[i] == 0;
}

inline Bool String::BeginsWith(const char* str) const
{
	return BeginsWith(str, UTF8CountArray(str));
}

inline Bool String::BeginsWith(const String& str) const
{
	return BeginsWith(str.data, str.size);
}

inline Bool String::EndsWith(const char* str, U32 len) const
{
	if (len > size)
		return false;

	U32 i = 0;
	for (; i < len && data[size - 1 - i] == str[len - 1 - i]; i++);
	return i == len;
}

inline Bool String::EndsWith(const char* str) const
{
	return EndsWith(str, UTF8CountArray(str));
}

inline Bool String::EndsWith(const String& str) const
{
	return EndsWith(str.data, str.size);
}

inline Bool String::Find(char c, U32 offset, U32& outIndex) const
{
	outIndex = offset;
	for (; outIndex < size && data[outIndex] != c; outIndex++);
	return outIndex < size && data[outIndex] == c;
}

inline Bool String::Find(const char* str, U32 len, U32 offset, U32& outIndex) const
{
	if (offset >= size || len > (size - offset))
		return false;

	outIndex = offset;
	for (;outIndex < size; outIndex++)
	{
		for (; outIndex < size && data[outIndex] != str[0]; outIndex++);

		U32 i = 0;
		for (; i < len && data[outIndex + i] == str[i]; i++);

		if (i == len)
			return true;
		if (len > (size - outIndex))
			return false;
	}

	return false;
}

inline Bool String::Find(const char* str, U32 offset, U32& outIndex) const
{
	return Find(str, UTF8CountArray(str), offset, outIndex);
}

inline Bool String::Find(const String& str, U32 offset, U32& outIndex) const
{
	return Find(str.data, str.size, offset, outIndex);
}

inline Bool String::FindLast(char c, U32 offset, U32& outIndex) const
{
	if (size == 0)
		return false;

	outIndex = size - 1;
	for (; outIndex >= offset && data[outIndex] != c; --outIndex);
	return data[outIndex] == c;
}

inline Bool String::FindLast(const char* str, U32 len, U32 offset, U32& outIndex) const
{
	if (offset >= size || len > (size - offset))
		return false;

	outIndex = size - 1 - offset;
	for (; outIndex > 0; --outIndex)
	{
		for (; outIndex > 0 && data[outIndex] != str[len - 1]; --outIndex);

		U32 i = 0;
		for (; i < len && data[outIndex - i] == str[len - i - 1]; i++);

		if (i == len)
			return true;
		if (len > outIndex)
			return false;
	}

	return len == 1 && data[outIndex] == str[0];
}

inline Bool String::FindLast(const char* str, U32 offset, U32& outIndex) const
{
	return FindLast(str, UTF8CountArray(str), offset, outIndex);
}

inline Bool String::FindLast(const String& str, U32 offset, U32& outIndex) const
{
	return FindLast(str.data, str.size, offset, outIndex);
}

inline Bool String::Find(char c, U32 offset, U32 count, Array<U32>& outIndexArray) const
{
	outIndexArray.Resize(0);
	U32 index = offset;
	for (U32 i = 0; i < count && Find(c, index, index); i++)
	{
		outIndexArray.Add(index);
		index++;
	}
	return outIndexArray.size > 0;
}

inline Bool String::Find(const char* str, U32 len, U32 offset, U32 count, Array<U32>& outIndexArray) const
{
	outIndexArray.Resize(0);
	U32 index = offset;
	for (U32 i = 0; i < count && Find(str, len, index, index); i++)
	{
		outIndexArray.Add(index);
		index += len;
	}
	return outIndexArray.size > 0;
}

inline Bool String::Find(const char* str, U32 offset, U32 count, Array<U32>& outIndexArray) const
{
	return Find(str, UTF8CountArray(str), offset, count, outIndexArray);
}

inline Bool String::Find(const String& str, U32 offset, U32 count, Array<U32>& outIndexArray) const
{
	return Find(str.data, str.size, offset, count, outIndexArray);
}

inline C8* String::begin()
{
	return data;
}

inline const C8* String::begin() const
{
	return data;
}

inline C8* String::end()
{
	return data + size;
}

inline const C8* String::end() const
{
	return data + size;
}

inline Bool operator==(const String& Str, const char* Other)
{
	U32 Size = UTF8CountArray(Other);

	if (Str.size != Size) return 0;

	U32 Index = 0;
	for (; Str.data[Index] == Other[Index]; Index++);

	return Index == Size;
}

inline Bool operator==(const String& Str, const String& Other)
{
	if (Str.size != Other.size) return 0;

	U32 Index = 0;
	for (; Str.data[Index] == Other.data[Index]; Index++);

	return Index == Other.size;
}