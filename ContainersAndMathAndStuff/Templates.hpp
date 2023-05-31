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

/**
* Type traits
*/

template <class T, class Ty> struct IsSame { enum { Value = 0 }; };
template <class T> struct IsSame<T, T> { enum { Value = 1 }; };

template <class T, class From>
struct IsConstructable
{
	typedef struct { U8 val[1]; } True;
	typedef struct { U8 val[2]; } False;
	static True Test(T);
	static False Test(...);
	static From Create();

	enum { Value = (sizeof(Test(Create())) == sizeof(True)) };
};

template <class T> struct IsIntegral { enum { Value = 0 }; };
template <> struct IsIntegral<I8> { enum { Value = 1 }; };
template <> struct IsIntegral<I16> { enum { Value = 1 }; };
template <> struct IsIntegral<I32> { enum { Value = 1 }; };
template <> struct IsIntegral<I64> { enum { Value = 1 }; };
template <> struct IsIntegral<U8> { enum { Value = 1 }; };
template <> struct IsIntegral<U16> { enum { Value = 1 }; };
template <> struct IsIntegral<U32> { enum { Value = 1 }; };
template <> struct IsIntegral<U64> { enum { Value = 1 }; };

template <class T> struct IsFloat { enum { Value = 0 }; };
template <> struct IsFloat<F32> { enum { Value = 1 }; };
template <> struct IsFloat<F64> { enum { Value = 1 }; };

template <class T> struct IsPointer { enum { Value = 0 }; };
template <class T> struct IsPointer<T*> { enum { Value = 1 }; };

template <class T> struct RemovePointer{ typedef T Type; };
template <class T> struct RemovePointer<T*>{ typedef T Type; };

template <class T> struct RemoveReference { typedef T Type; };
template <class T> struct RemoveReference<T&> { typedef T Type; };

#if defined(CPP_HAS_MOVE)
template <class T> struct RemoveReference<T&&> { typedef T Type; };
#endif

template <class T, class Ty, Bool B> struct SelectType { typedef Ty Type; };
template <class T, class Ty> struct SelectType<T, Ty, true> { typedef T Type; };

template <I32 A, I32 B, Bool V> struct SelectI32 { enum { Value = B }; };
template <I32 A, I32 B> struct SelectI32<A, B, true> { enum { Value = A }; };

template <class Tx, class Ty> struct Pair { Tx a; Ty b; Pair(Tx a, Ty b) : a(a), b(b) {} };

/**
* Mov functions
*/

template <class T>
struct RRef {
	T data;
	T& operator*() noexcept { return *((T*)this); }
	T* operator->() noexcept { return ((T*)this); }
};

template <class T, Bool B = IsConstructable<T, RRef<T>&>::Value> struct IsMoveable { typedef T& Type; };
template <class T> struct IsMoveable<T, 1> { typedef RRef<T>& Type; };

template<class T> RRef<T>& Mov(RRef<T>& var)
{
	return var;
}

template<class T>
typename IsMoveable<typename RemoveReference<T>::Type>::Type Mov(T& var)
{
	return (typename IsMoveable<typename RemoveReference<T>::Type>::Type)var;
}
