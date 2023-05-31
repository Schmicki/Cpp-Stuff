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
* reinterpret functions
*/

// Get reference as reference
template <class T, class Ty>
inline T& GetRR(Ty& var)
{
    ASSERT(alignof(Ty) >= alignof(T) && sizeof(Ty) >= sizeof(T));
    return reinterpret_cast<T&>(var);
}

// Get reference as pointer
template <class T, class Ty>
inline T* GetRP(Ty& var)
{
    ASSERT(alignof(Ty) >= alignof(T) && sizeof(Ty) >= sizeof(T));
    return reinterpret_cast<T*>(&var);
}

// Get reference as value
template <class T, class Ty>
inline T GetRV(Ty& var)
{
    ASSERT(alignof(Ty) >= alignof(T) && sizeof(Ty) >= sizeof(T));
    return reinterpret_cast<T&>(var);
}

// Get pointer as pointer
template <class T, class Ty>
inline T* GetPP(Ty* var)
{
    ASSERT(alignof(Ty) >= alignof(T) && sizeof(Ty) >= sizeof(T));
    return reinterpret_cast<T*>(var);
}

// Get pointer as reference
template <class T, class Ty>
inline T& GetPR(Ty* var)
{
    ASSERT(alignof(Ty) >= alignof(T) && sizeof(Ty) >= sizeof(T));
    return reinterpret_cast<T&>(*var);
}

// Get pointer as value
template <class T, class Ty>
inline T GetPV(Ty* var)
{
    ASSERT(alignof(Ty) >= alignof(T) && sizeof(Ty) >= sizeof(T));
    return reinterpret_cast<T&>(*var);
}

// Get pointer as value
template <class T, class Ty>
inline T GetVV(const Ty& var)
{
    ASSERT(alignof(Ty) >= alignof(T) && sizeof(Ty) >= sizeof(T));
    return reinterpret_cast<const T&>(var);
}



/**
* Cast functions
*/

// Cast reference to reference
template <class T, class Ty>
inline T& CastRR(Ty& var)
{
    return static_cast<T&>(var);
}

// Cast reference to pointer
template <class T, class Ty>
inline T* CastRP(Ty& var)
{
    return static_cast<T*>(&var);
}

// Cast reference to value
template <class T, class Ty>
inline T CastRV(Ty& var)
{
    return static_cast<T>(var);
}

// Cast pointer to pointer
template <class T, class Ty>
inline T* CastPP(Ty* var)
{
    return static_cast<T*>(var);
}

// Cast pointer to reference
template <class T, class Ty>
inline T& CastPR(Ty* var)
{
    return static_cast<T&>(*var);
}

// Cast pointer to value
template <class T, class Ty>
inline T CastPV(Ty* var)
{
    return static_cast<T>(*var);
}

// Cast value as value
template <class T, class Ty>
inline T CastVV(const Ty& var)
{
    return static_cast<T>(var);
}
