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

/**
* Compatibility Guide
* ###################
* 
* Do not use the C++ standard library
* 
* Do not use Features of C98++ or higher including dynamic_cast, typeid,
* covariant return types, cast operators, mutable, bool,
* template instantiation, member templates, export, auto - except on variables,
* decltype, defaulted/deleted functions, final, override, rvalue references,
* enum class, constexpr - except on functions, initializer lists,
* delegating constructor,
* using - except for namespaces, member variables/types/functions,
* variadic templates, union with non-POD data members, attributes, lambdas,
* range-for, static_assert, variable templates, inline variables, anonymous
* unions/structs, 
*
* 
* Macros:
* #######
* 
* COMPILER_MSVC - defined as 1 if code is compiled using msvc
* 
* COMPILER_GCC - defined as 1 if code is compiled using gcc
* 
* COMPILER_CLANG - defined as 1 if code is compiled using clang
* 
* ARCHITECTURE_X64 - defined as 1 if code is compiled for x64

* ARCHITECTURE_X86 - defined as 1 if code is compiled for x86

* ARCHITECTURE_ARM64 - defined as 1 if code is compiled for ARM64

* ARCHITECTURE_ARM32 - defined as 1 if code is compiled for ARM32

* SYSTEM_WINDOWS - defined as if code is compiled for windows
* 
* SYSTEM_ANDROID - defined as if code is compiled for android

* SYSTEM_LINUX - defined as if code is compiled for linux

* SYSTEM_MACOS - defined as if code is compiled for macos
* 
* BUILD_DEBUG - defined if code is compiled in debug mode
* 
* BUILD_RELEASE - defined if code is compiled in release mode
* 
* LIB_EXPORT - tell the compiler to export this symbol
* 
* LIB_IMPORT - tell compiler this symbol is part of a lib
* 
* LIB_HIDE - tell the compiler that this symbol should not be exported
* 
* alignas - used on classes/structs to increase the alignment requirement
* 
* alignof - get alignment of class/struct alignof(class/struct-name)
*
* nullptr - defined when compiling in older c++ versions
* 
* constexpr - inline, defined when compiling in older c++ versions
* 
* noexcept - 
* 
* CACHE_LINE - size of a cache line of target architecture
* 
* CACHE_ALIGN - alignas(CACHE_LINE)
* 
* FORCE_INLINE - tells the compiler that this function should really be inlined
* 
* ASSERT - check condition and if false, stop app
* 
* ASSERT_FAST - check condition in debug builds and if false, stop app
* 
* SHFTM
* SHFTMS
* SHFTD
* SHFTDS
* 
* 
* 
* Types:
* 
* Bool
* Byte
* C8
* C16
* C32
* I8
* I16
* I32
* I64
* IPtr
* U8
* U16
* U32
* U64
* UPtr
* F32
* F64
* Flt
*/

#pragma once


/*
* Detect compiler
*/

#if defined(_MSC_VER)

#define COMPILER_MSVC 1
#define LIB_EXPORT __declspec(dllexport)
#define LIB_IMPORT __declspec(dllimport)
#define LIB_HIDE
#define FORCE_INLINE __forceinline


#elif defined(__GNUC__) || defined(__MINGW32__) || defined(__MINGW64__)

#define COMPILER_GCC 1
#define LIB_EXPORT __attribute__((visibility("default")))
#define LIB_IMPORT
#define LIB_HIDE __attribute__((visibility("hidden")))
#define FORCE_INLINE __attribute__((always_inline))


#elif defined(__clang__)

#define COMPILER_CLANG 1
#define LIB_EXPORT __attribute__((visibility("default")))
#define LIB_IMPORT
#define LIB_HIDE __attribute__((visibility("hidden")))
#define FORCE_INLINE __attribute__((always_inline))


#else

#error detection could not find compiler, please implement it

#endif



/*
* Detect architecture
*/

#if defined(_M_AMD64) || defined(_M_X64) || defined(__x86_64__) || defined(__amd64__)

#define ARCHITECTURE_X64 1
#define CACHE_LINE 64


#elif defined(_M_IX86) || defined(__i386__)

#define ARCHITECTURE_X86 1
#define CACHE_LINE 32

#elif  defined(_M_ARM64) || defined(__aarch64__) || defined(_M_ARM64EC)

#define ARCHITECTURE_ARM64 1
#define CACHE_LINE 64


#elif defined(_M_ARM) || defined(__arm__)

#define ARCHITECTURE_ARM32 1
#define CACHE_LINE 32


#else

#error detection could not find architecture, please implement it

#endif



/*
* Detect system
*/

#if defined(_WIN64) || defined(_WIN32) || defined(WIN32)

#define SYSTEM_WINDOWS 1


#elif defined(__ANDROID__) || defined(ANDROID)

#define SYSTEM_ANDROID 1


#elif defined(__linux__)

#define SYSTEM_LINUX 1


#elif defined(__APPLE__)

#define SYSTEM_MACOS 1


#else

#error detection could not find system, please implement it

#endif



/**
* Detect C++ Features
*/

#if defined(__cplusplus)

#if COMPILER_MSVC

#if _MSC_FULL_VER >= 170050727 // VS2015

#define CPP_HAS_ALIGNAS 1
#define CPP_HAS_ALIGNOF 1
#define CPP_HAS_NOEXCEPT 1


#else

#define alignas(alignment) __declspec(align(alignment))
#define alignof(T) __alignof(T)

#endif


#if _MSC_FULL_VER >= 180021114 // VS2013 CTP

#define CPP_HAS_CONSTEXPR 1


#else



#endif


#if _MSC_FULL_VER >= 180021005 // VS2013

#define CPP_HAS_VARIADIC_TEMPLATE 1


#else



#endif


#if _MSC_FULL_VER >= 170050727 // VS2012

#define CPP_HAS_ATOMICS 1


#else



#endif


#if _MSC_FULL_VER >= 160030319 // VS2010

#define CPP_HAS_MOVE 1
#define CPP_HAS_NULLPTR 1
#define CPP_HAS_STATIC_ASSERT 1


#else



#endif


#else

#if __cplusplus >= 201103L

#define CPP_HAS_ATOMICS 1
#define CPP_HAS_VARIADIC_TEMPLATE 1
#define CPP_HAS_MOVE 1
#define CPP_HAS_NULLPTR 1
#define CPP_HAS_ALIGNAS 1
#define CPP_HAS_ALIGNOF 1
#define CPP_HAS_STATIC_ASSERT 1
#define CPP_HAS_CONSTEXPR 1
#define CPP_HAS_NOEXCEPT 1

#else

#define alignas(alignment) __attribute__ ((aligned (alignment)))
#define alignof(T) __alignof__(T)


#endif
#endif
#endif

#if !defined(CPP_HAS_NULLPTR)
#define nullptr 0
#endif

#if !defined(CPP_HAS_CONSTEXPR)
#define constexpr inline
#endif

#if !defined(CPP_HAS_NOEXCEPT)
#define noexcept throw()
#endif



/**
* Detect build mode
*/

#if defined(DEBUG) || defined(_DEBUG) || !defined(NDEBUG) || BUILD_DEBUG

#ifndef BUILD_DEBUG
#define BUILD_DEBUG 1
#endif


#else

#ifndef BUILD_RELEASE
#define BUILD_RELEASE 1
#endif

#ifndef NDEBUG
#define NDEBUG 1
#endif

#endif


#if !defined(BUILD_DEBUG)
#undef NDEBUG
#endif

#include <stddef.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <new>

#if !defined(BUILD_DEBUG)
#define NDEBUG
#endif

#define ASSERT(expr) assert(expr)

#if defined(BUILD_DEBUG)
#define ASSERT_FAST(expr) assert(expr)
#else
#define ASSERT_FAST(expr) ((void)0)
#endif


#define CACHE_ALIGN alignas(CACHE_LINE)

#define SHFTM(var, amount) ((var) << (amount))

#define SHFTMS(var, amount) var <<= amount

#define SHFTD(var, amount) ((var) >> (amount))

#define SHFTDS(var, amount) var >>= amount


// Short type aliases

typedef unsigned char Bool;
typedef unsigned char Byte;
typedef char C8;
typedef uint16_t C16;
typedef uint32_t C32;

typedef int8_t I8;
typedef int16_t I16;
typedef int32_t I32;
typedef int64_t I64;
typedef ptrdiff_t IPtr;

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;
typedef size_t UPtr;

typedef float F32;
typedef double F64;


#if defined FLOAT64

typedef F64 Flt;


#else

typedef F32 Flt;

#endif

#define I8_MIN INT8_MIN
#define I16_MIN INT16_MIN
#define I32_MIN INT32_MIN
#define I64_MIN INT64_MIN
#define IPTR_MIN PTRDIFF_MIN

#define I8_MAX INT8_MAX
#define I16_MAX INT16_MAX
#define I32_MAX INT32_MAX
#define I64_MAX INT64_MAX
#define IPTR_MAX PTRDIFF_MAX

#define U8_MAX UINT8_MAX
#define U16_MAX UINT16_MAX
#define U32_MAX UINT32_MAX
#define U64_MAX UINT64_MAX
#define UPTR_MAX SIZE_MAX


#define ConstexprAlign(size, alignmentBytes) ((size % alignmentBytes) == 0 ? size : size + alignmentBytes - (size % alignmentBytes))

inline U32 Align(U32 size, U32 alignmentBytes)
{
	const U32 pad = size % alignmentBytes;
	return pad == 0 ? size : size + alignmentBytes - pad;
}

inline U64 Align(U64 size, U64 alignmentBytes)
{
	const U64 pad = size % alignmentBytes;
	return pad == 0 ? size : size + alignmentBytes - pad;
}

template <class T>
inline T* Allocate(UPtr count)
{
	return (T*)malloc(count * sizeof(T));
}

inline void* AllocateBytes(UPtr count)
{
	return malloc(count);
}

inline void Free(void* memory)
{
	free(memory);
}