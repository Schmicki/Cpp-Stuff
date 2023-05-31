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
#include "Templates.hpp"
#include <cmath>

#define F32_PI 3.141592f
#define F64_PI 3.141592653589793

template <class T>
struct MakeFloat { typedef typename SelectType<T, F32, IsFloat<T>::Value>::Type Type; };


template <class T>
constexpr T Select(T a, T b, Bool useA)
{
	return useA ? a : b;
}


template <class T>
inline T Flip(T value)
{
	return (T)1.0 - value;
}


template <class T>
inline T Abs(T value)
{
	return abs(value);
}


template <class T>
inline T Min(T a, T b)
{
	return a < b ? a : b;
}


template <class T>
inline T AbsMin(T a, T b)
{
	a = Abs(a);
	b = Abs(b);
	return a < b ? a : b;
}


template <class T>
inline T Max(T a, T b)
{
	return a > b ? a : b;
}


template <class T>
inline T AbsMax(T a, T b)
{
	a = Abs(a);
	b = Abs(b);
	return a > b ? a : b;
}


template <class T>
inline T Sum(T a, T b)
{
	return a + b;
}


template <class T>
inline T AbsSum(T a, T b)
{
	return Abs(a) + Abs(b);
}


template <class T>
inline T Avg(T a, T b)
{
	return Sum(a + b) / (T)2.0;
}


template <class T>
inline Bool InRange(T min, T max, T value)
{
	return value >= min && value <= max;
}

template <class T>
inline Bool InRangeEx(T min, T max, T value)
{
	return value > min && value < max;
}


template <class T>
inline T Clamp(T min, T max, T value)
{
	return value < min ? min : value > max ? max : value;
}


inline Bool Near(const F32 a, const F32 b, F32 maxDiff)
{
	return Abs(a - b) <= maxDiff;
}

inline Bool Near(const F64 a, const F64 b, F64 maxDiff)
{
	return Abs(a - b) <= maxDiff;
}

template <class T>
inline Bool Near(const T a, const T b, T maxDiff)
{
	return Abs(a - b) <= maxDiff;
}


inline Bool NearACD(const F32 a, const F32 b)
{
	F32 maxDiff = 0.0000002f * Max(1.0f, Max(Abs(a), Abs(b)));
	return Abs(a - b) <= maxDiff;
}

inline Bool NearACD(const F64 a, const F64 b)
{
	F64 maxDiff = 0.0000000000002 * Max(1.0, Max(Abs(a), Abs(b)));
	return Abs(a - b) <= maxDiff;
}

template <class T>
inline Bool NearACD(const T a, const T b)
{
	F32 maxDiff = 0.0000002f * Max(1.0f, (F32)Max(Abs(a), Abs(b)));
	return Abs(a - b) <= (T)maxDiff;
}


inline F32 Round(F32 value)
{
	return round(value);
}

inline F64 Round(F64 value)
{
	return round(value);
}


inline F32 Trunc(F32 value)
{
	return trunc(value);
}

inline F64 Trunc(F64 value)
{
	return trunc(value);
}


template <class T>
inline T Mod(T val, T mod)
{
	return fmod(val, mod);
}


inline F32 Lerp(F32 a, F32 b, F32 alpha)
{
	return a + alpha * (b - a);
}

inline F64 Lerp(F64 a, F64 b, F64 alpha)
{
	return a + alpha * (b - a);
}

template <class T>
inline T Lerp(T a, T b, F32 alpha)
{
	return a + CastVV<T>(alpha * CastVV<F32>(b - a));
}


constexpr F32 DegToRad(const F32 degrees)
{
	return degrees * (F32_PI / 180.0f);
}

constexpr F64 DegToRad(const F64 degrees)
{
	return degrees * (F64_PI / 180.0);
}

template <class T>
constexpr T DegToRad(const T degrees)
{
	return (T)((F32)degrees * (F32_PI / 180.0));
}


constexpr F32 RadToDeg(const F32 radians)
{
	return radians / F32_PI * 180.0f;
}

constexpr F64 RadToDeg(const F64 radians)
{
	return radians / F64_PI * 180.0;
}

template <class T>
constexpr T RadToDeg(const T radians)
{
	return (T)((F32)radians / F32_PI * 180.0);
}


template <class T>
inline T Cos(const T radians)
{
	return cos(radians);
}

template <class T>
inline T CosD(const T degrees)
{
	return cos(DegToRad(degrees));
}


template <class T>
inline T Acos(const T radians)
{
	return acos(radians);
}

template <class T>
inline T AcosD(const T degrees)
{
	return acos(DegToRad(degrees));
}


template <class T>
inline T Sin(const T radians)
{
	return sin(radians);
}

template <class T>
inline T SinD(const T degrees)
{
	return sin(DegToRad(degrees));
}


template <class T>
inline T Asin(const T radians)
{
	return asin(radians);
}

template <class T>
inline T AsinD(const T degrees)
{
	return asin(DegToRad(degrees));
}


template <class T>
inline T Tan(const T radians)
{
	return tan(radians);
}

template <class T>
inline T TanD(const T degrees)
{
	return tan(DegToRad(degrees));
}


template <class T>
inline T Atan(const T radians)
{
	return atan(radians);
}

template <class T>
inline T AtanD(const T degrees)
{
	return atan(DegToRad(degrees));
}


template <class T>
inline T Pow(T value, T exponent)
{
	return pow(value, exponent);
}

template <class T>
inline T Pow2(T value)
{
	return value * value;
}


template <class T>
inline T Sqrt(T value)
{
	return sqrt(value);
}


template <class T>
inline T Log(T value)
{
	return log(value);
}
