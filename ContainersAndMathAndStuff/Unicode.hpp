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

#include "Platform.hpp"

// UTF-8
	// 01111111                             7-bit ASCII characters
	// 110xxxxx 10xxxxxx                    2-byte sequence 11-bit characters
	// 1110xxxx 10xxxxxx 10xxxxxx           3-byte sequence 16-bit characters
	// 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx  4-byte sequence 21-bit characters
	//
	// ASCII: 0 - 127
	//
	// Start: 192u - 253u		||	-64 - -3
	// Following: 128u - 191u	||	-128 - -65
	//
	// Start 2: 192u - 223u		||	-64 - -33
	// Start 3: 224u - 239u		||	-32 - -17
	// Start 4: 240u - 247u		||	-16 - -9
	// Start 5: 248u - 251u		||	-8 - -5
	// Start 6: 252u - 253u		||	-4 - -3
	//
	//

inline U32 UTF8SymbolSize(const char c)
{
	return ((Byte)c > 0) + ((Byte)c > 0xBF) + ((Byte)c > 0xDF) + ((Byte)c > 0xEF);
}

// count number of array elements (excluding 0)
inline U32 UTF8CountArray(const char* str)
{
	if (str == nullptr)
		return 0;

	U32 i = 0;
	for (; str[i] != 0; i += UTF8SymbolSize(str[i]));
	return i;
}

inline U32 UTF8CountSymbols(const char* str)
{
	if (str == nullptr)
		return 0;

	U32 count = 0;
	for (U32 i = 0; str[i] != 0; i += UTF8SymbolSize(str[i]), count++);
	return count;
}

// count number of array elements a UTF16 string needs (excluding 0)
inline U32 UTF8CountUTF16(const char* str)
{
	U32 count = 0;
	for (U32 i = 0; str[i] != 0; )
	{
		U32 size = UTF8SymbolSize(str[i]);
		count += size > 3 ? 2 : 1;
		i += size;
	}
	return count;
}

// return value: number of array elements the symbol used
inline U32 UTF8EncodeSymbol(char* str, U32 symbol)
{
	if (symbol < 0x80)
	{
		str[0] = symbol;
		return 1;
	}
	else if (symbol < 0x800)
	{
		str[0] = 0xC0 | static_cast<char>(symbol >> 6);
		str[1] = 0x80 | static_cast<char>(symbol & 0x3F);
		return 2;
	}
	else if (symbol < 0x10000)
	{
		str[0] = 0xE0 | static_cast<char>(symbol >> 12);
		str[1] = 0x80 | static_cast<char>(symbol >> 6 & 0x3F);
		str[2] = 0x80 | static_cast<char>(symbol & 0x3F);
		return 3;
	}
	else
	{
		str[0] = 0xF0 | static_cast<char>(symbol >> 18);
		str[1] = 0x80 | static_cast<char>(symbol >> 12) & 0x3F;
		str[2] = 0x80 | static_cast<char>(symbol >> 6) & 0x3F;
		str[3] = 0x80 | static_cast<char>(symbol & 0x3F);
		return 4;
	}
}

// return value: number of array elements the symbol used
inline U32 UTF8DecodeSymbol(const char* str, U32& outSymbol)
{
	switch (UTF8SymbolSize(str[0]))
	{
	default:
		return 0;
	case 0:
		outSymbol = 0;
		return 0;
	case 1:
		outSymbol = str[0];
		return 1;
	case 2:
		outSymbol = ((0x1F & str[0]) << 6) | (0x1F & str[1]);
		return 2;
	case 3:
		outSymbol = ((0xF & str[0]) << 12) | ((0x1F & str[1]) << 6) | (0x1F & str[2]);
		return 3;
	case 4:
		outSymbol = ((0x7 & str[0]) << 18) | ((0x1F & str[1]) << 12) | ((0x1F & str[2]) << 6) | (0x1F & str[3]);
		return 4;
	}
}



// UTF-16 Little Endian & Big Endian (standard x86 is little endian, the byte order is mirrored on big endian)
// xxxxxxxx xxxxxxxx                    16-bit Unicode characters
// 110110xx xxxxxxxx                    High
// 110111xx xxxxxxxx                    Low
// 110110xx xxxxxxxx 110110xx xxxxxxxx  4-byte sequence 20-bit charcaters
// Everything between 11011000 00000000 - 11011011 11111111 (0xD800 - 0xDBFF) and 11011100 00000000 - 11011111 11111111 (0xDC00 - 0xDFFF)
// is reserved space in unicode so you can identify UTF-16 "Surrogate Pairs"
//
//

inline U32 UTF16SymbolSize(const U16 c)
{
	return (c < 0xD800 || c > 0xDFFF) ? 1 : 2;
}

// count number of array elements (excluding 0)
inline U32 UTF16CountArray(const U16* str)
{
	if (str == nullptr)
		return 0;

	U32 i = 0;
	for (; str[i] != 0; i += UTF16SymbolSize(str[i]));
	return i;
}

inline U32 UTF16CountSymbols(const U16* str)
{
	if (str == nullptr)
		return 0;

	U32 count = 0;
	for (U32 i = 0; str[i] != 0; i += UTF16SymbolSize(str[i]), count++);
	return count;
}

// count number of array elements a UTF8 string needs (excluding 0)
inline U32 UTF16CountUTF8(const U16* str)
{
	if (str == nullptr)
		return 0;

	U32 count = 0;
	for (U32 i = 0; str[i] != 0;)
	{
		if (UTF16SymbolSize(str[i]) == 2)
		{
			i += 2;
			count += 4;
		}
		else
		{
			count += 1 + (str[i] > 0x7F) + (str[i] > 0x7FF);
			i++;
		}
	}

	return count;
}

// return value: number of array elements the symbol used
inline U32 UTF16EncodeSymbol(U16* str, U32 symbol)
{
	if (symbol < 0x10000)
	{
		str[0] = (U16)symbol;
		return 1;
	}
	else
	{
		symbol -= 0x10000;
		str[0] = 0xD800 | ((symbol >> 10) & 0x3FF);
		str[1] = 0xDC00 | (symbol & 0x3FF);
		return 2;
	}
}

// return value: number of array elements the symbol used
inline U32 UTF16DecodeSymbol(const U16* str, U32& outSymbol)
{
	if (UTF16SymbolSize(str[0]) == 1)
	{
		outSymbol = (U32)str[0];
		return 1;
	}
	else
	{
		outSymbol = (((U32)str[0] & 1023) << 10) | ((U32)str[1] & 1023) + 65536;
		return 2;
	}
}

// UTF-32
//
//

inline U32 UTF32CountUTF8(const U32* str)
{
	U32 count = 0;
	for (U32 i = 0; str[i] != 0; count += 1 + (str[i] > 0x7F) + (str[i] > 0x7FF) + (str[i] > 0xFFFF), i++);
	return count;
}

inline U32 UTF32CountSymbols(const U32* str)
{
	if (str == nullptr)
		return 0;

	U32 i = 0;
	for (; str[i] != 0; i++);
	return i;
}