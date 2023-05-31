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

#include "Unicode.hpp"

// forward declarations

template <class T>
class Array;

/**
* string declaration
*/

#define SMALL_STRING_SIZE sizeof(U32) + sizeof(char*)

class String
{
public:

	union
	{
		struct
		{
			U32 size, capacity;
			C8* data;
		};

		struct
		{
			U32 _size;
			C8 _data[SMALL_STRING_SIZE];
		};
	};

	String();
	String(const char* str);
	String(const wchar_t* str);
	String(const U16* str);
	String(const U32* str);

	String(const String& other);
	String(RRef<String>& other) noexcept;

	~String();

	String& operator=(const String& other);
	String& operator=(RRef<String>& other) noexcept;

	String operator+(const String& other);
	String& operator+=(const String& other);

	String operator+(const char* other);
	String& operator+=(const char* other);

	char& operator [] (U32 index);
	const char& operator [] (U32 index) const;

	void Reserve(U32 size);
	void Release();
	void Resize(U32 size, char defaultChar = ' ');

	char& Get(U32 index);
	const char& Get(U32 index) const;

	void Add(char c);
	void Add(const char* str, U32 len);
	void Add(const char* str);
	void Add(const String& str);

	void Insert(char c, U32 index);
	void Insert(const char* str, U32 len, U32 index);
	void Insert(const char* str, U32 index);
	void Insert(const String& str, U32 index);

	void Remove(U32 index);
	void Remove(U32 first, U32 last);
	void Remove(char c, U32 offset, U32 count);
	void Remove(const char* c, U32 offset, U32 len, U32 count);
	void Remove(const char* str, U32 offset, U32 count);
	void Remove(const String& str, U32 offset, U32 count);

	void SubString(U32 start, U32 end, String& outString) const;
	void Split(U32 firstRight, String& outLeft, String& outRight) const;
	void Split(C8 Seperator, Array<String>& OutTokens) const;

	U16* ToUTF16() const;
	U32* ToUTF32() const;
	wchar_t* ToWString() const;

	Bool BeginsWith(const char* str, U32 len) const;
	Bool BeginsWith(const char* str) const;
	Bool BeginsWith(const String& str) const;

	Bool EndsWith(const char* str, U32 len) const;
	Bool EndsWith(const char* str) const;
	Bool EndsWith(const String& str) const;

	Bool Find(char c, U32 offset, U32& outIndex) const;
	Bool Find(const char* str, U32 len, U32 offset, U32& outIndex) const;
	Bool Find(const char* str, U32 offset, U32& outIndex) const;
	Bool Find(const String& str, U32 offset, U32& outIndex) const;

	Bool FindLast(char c, U32 offset, U32& outIndex) const;
	Bool FindLast(const char* str, U32 len, U32 offset, U32& outIndex) const;
	Bool FindLast(const char* str, U32 offset, U32& outIndex) const;
	Bool FindLast(const String& str, U32 offset, U32& outIndex) const;

	Bool Find(char c, U32 offset, U32 count, Array<U32>& outIndexArray) const;
	Bool Find(const char* str, U32 len, U32 offset, U32 count, Array<U32>& outIndexArray) const;
	Bool Find(const char* str, U32 offset, U32 count, Array<U32>& outIndexArray) const;
	Bool Find(const String& str, U32 offset, U32 count, Array<U32>& outIndexArray) const;

	C8* begin();
	const C8* begin() const;

	C8* end();
	const C8* end() const;
};
