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

#include "MathBase.hpp"

// forward declarations

template <class T, I32 sizeX, I32 sizeY>
struct Mat;

template <class T>
struct Quat;

/**
* vector declaration
*/

template <class T, I32 size>
struct Vec;

template <class T>
struct Vec<T, 2>
{
	union
	{
		struct { T x, y; };
		struct { T r, g; };
		T m[2];
	};

	//Vec();
	Vec(T val);
	Vec(const T m[2]);
	Vec(T x, T y);
	Vec(const Vec<T, 3>& xy);
	Vec(const Vec<T, 4>& xy);

	Vec(const Vec<typename SelectType<I32, F32, IsSame<T, F32>::Value>::Type, 2>& other);
	Vec(const Vec<typename SelectType<I32, F64, IsSame<T, F64>::Value>::Type, 2>& other);

	Vec operator + (const Vec& other) const;
	Vec& operator += (const Vec& other);

	Vec operator - () const;
	Vec operator - (const Vec& other) const;
	Vec& operator -= (const Vec& other);

	Vec operator * (T value) const;
	Vec& operator *= (T value);

	Vec operator / (T value) const;
	Vec& operator /= (T value);

	static Vec Zero;
	static Vec Left;
	static Vec Right;
	static Vec Up;
	static Vec Down;
};

template <class T>
struct Vec<T, 3>
{
	union
	{
		struct { T x, y, z; };
		struct { T r, g, b; };
		struct { T xy[2]; T _z; };
		struct { T _x; T yz[2]; };
		struct { T rg[2]; T _b; };
		struct { T _r; T gb[2]; };
		T m[3];
	};

	//Vec();
	Vec(T val);
	Vec(const T m[3]);
	Vec(T x, T y, T z);

	Vec(const Vec<T, 2>& xy, T z);
	Vec(T x, const Vec<T, 2>& yz);
	Vec(const Vec<T, 4>& xyz);
	Vec(const Quat<T>& xyz);

	Vec(const Vec<typename SelectType<I32, F32, IsSame<T, F32>::Value>::Type, 3>& other);
	Vec(const Vec<typename SelectType<I32, F64, IsSame<T, F64>::Value>::Type, 3>& other);

	Vec operator + (const Vec& other) const;
	Vec& operator += (const Vec& other);

	Vec operator - () const;
	Vec operator - (const Vec& other) const;
	Vec& operator -= (const Vec& other);

	Vec operator * (T value) const;
	Vec& operator *= (T value);

	Vec operator / (T value) const;
	Vec& operator /= (T value);

	static Vec Zero;
	static Vec Forward;
	static Vec Backward;
	static Vec Left;
	static Vec Right;
	static Vec Up;
	static Vec Down;
};

template <class T>
struct Vec<T, 4>
{
	union
	{
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };
		struct { T xyz[3]; T _w; };
		struct { T rgb[3]; T _a; };
		struct { T _x; T yzw[3]; };
		struct { T _r; T gba[3]; };
		struct { T xy[2]; T zw[2]; };
		struct { T rg[2]; T ba[2]; };
		struct { T _X; T yz[2]; T _W; };
		struct { T _R; T gb[2]; T _A; };
		T m[4];
	};

	//Vec();
	Vec(T val);
	Vec(const T m[4]);
	Vec(T x, T y, T z, T w);
	Vec(const Vec<T, 2>& xy, T z, T w);
	Vec(T x, T y, const Vec<T, 2>& zw);
	Vec(T x, const Vec<T, 2>& yz, T w);
	Vec(const Vec<T, 2>& xy, const Vec<T, 2>& zw);
	Vec(const Vec<T, 3>& xyz, T w);
	Vec(T x, const Vec<T, 3>& yzw);
	Vec(const Quat<T>& quaternion);

	Vec(const Vec<typename SelectType<I32, F32, IsSame<T, F32>::Value>::Type, 4>& other);
	Vec(const Vec<typename SelectType<I32, F64, IsSame<T, F64>::Value>::Type, 4>& other);

	Vec operator + (const Vec& other) const;
	Vec& operator += (const Vec& other);

	Vec operator - () const;
	Vec operator - (const Vec& other) const;
	Vec& operator -= (const Vec& other);

	Vec operator * (T value) const;
	Vec& operator *= (T value);

	Vec operator / (T value) const;
	Vec& operator /= (T value);
};


template<class T> inline Vec<T, 2> Abs(const Vec<T, 2>& v);
template<class T> inline T Min(const Vec<T, 2>& v);
template<class T> inline T AbsMin(const Vec<T, 2>& v);
template<class T> inline T Max(const Vec<T, 2>& v);
template<class T> inline T AbsMax(const Vec<T, 2>& v);
template<class T> inline T Sum(const Vec<T, 2>& v);
template<class T> inline T AbsSum(const Vec<T, 2>& v);
template<class T> inline T Avg(const Vec<T, 2>& v);
template<class T> inline T AbsAvg(const Vec<T, 2>& v);
template<class T> inline T Near(const Vec<T, 2>& v, const Vec<T, 2>& other, T maxDiff);
template<class T> inline T NearACD(const Vec<T, 2>& v, const Vec<T, 2>& other);
template<class T> inline T LengthSquared(const Vec<T, 2>& v);
template<class T> inline T Length(const Vec<T, 2>& v);
template<class T> inline Bool IsNormal(const Vec<T, 2>& v);
template<class T> inline Vec<T, 2> Normalize(const Vec<T, 2>& v);
template<class T> inline T DistanceSquared(const Vec<T, 2>& v, const Vec<T, 2>& other);
template<class T> inline T Distance(const Vec<T, 2>& v, const Vec<T, 2>& other);
template<class T> inline typename MakeFloat<T>::Type DotProduct(const Vec<T, 2>& v, const Vec<T, 2>& other);
template<class T> inline Vec<T, 2> Lerp(const Vec<T, 2>& v, const Vec<T, 2>& other, typename MakeFloat<T>::Type t);
template<class T> inline Vec<T, 2> Slerp(const Vec<T, 2>& v, const Vec<T, 2>& other, typename MakeFloat<T>::Type t);


template<class T> inline Vec<T, 3> Abs(const Vec<T, 3>& v);
template<class T> inline T Min(const Vec<T, 3>& v);
template<class T> inline T AbsMin(const Vec<T, 3>& v);
template<class T> inline T Max(const Vec<T, 3>& v);
template<class T> inline T AbsMax(const Vec<T, 3>& v);
template<class T> inline T Sum(const Vec<T, 3>& v);
template<class T> inline T AbsSum(const Vec<T, 3>& v);
template<class T> inline T Avg(const Vec<T, 3>& v);
template<class T> inline T AbsAvg(const Vec<T, 3>& v);
template<class T> inline T Near(const Vec<T, 3>& v, const Vec<T, 3>& other, T maxDiff);
template<class T> inline T NearACD(const Vec<T, 3>& v, const Vec<T, 3>& other);
template<class T> inline Bool IsNormal(const Vec<T, 3>& v);
template<class T> inline typename MakeFloat<T>::Type DotProduct(const Vec<T, 3>& v, const Vec<T, 3>& other);
template<class T> inline Vec<T, 3> CrossProduct(const Vec<T, 3>& v, const Vec<T, 3>& other);
template<class T> inline T LengthSquared(const Vec<T, 3>& v);
template<class T> inline T Length(const Vec<T, 3>& v);
template<class T> inline T DistanceSquared(const Vec<T, 3>& v, const Vec<T, 3>& other);
template<class T> inline T Distance(const Vec<T, 3>& v, const Vec<T, 3>& other);
template<class T> inline Vec<T, 3> Normalize(const Vec<T, 3>& v);
template<class T> inline Vec<T, 3> Lerp(const Vec<T, 3>& v, const Vec<T, 3>& other, typename MakeFloat<T>::Type t);
template<class T> inline Vec<T, 3> Slerp(const Vec<T, 3>& v, const Vec<T, 3>& other, typename MakeFloat<T>::Type t);
template<class T> inline Vec<T, 3> Rotate(const Vec<T, 3>& v, const Quat<T>& rotation);
template<class T> inline Vec<T, 3> Rotate(const Vec<T, 3>& v, const Vec<T, 3>& eulerAngles);
template<class T> inline Vec<T, 3> Rotate(const Vec<T, 3>& v, const Vec<T, 3>& axis, T angle);
template<class T> inline Vec<T, 3> Transform(const Vec<T, 3>& v, const Mat<T, 4, 4>& m);


template<class T> inline Vec<T, 4> Abs(const Vec<T, 4>& v);
template<class T> inline T Min(const Vec<T, 4>& v);
template<class T> inline T AbsMin(const Vec<T, 4>& v);
template<class T> inline T Max(const Vec<T, 4>& v);
template<class T> inline T AbsMax(const Vec<T, 4>& v);
template<class T> inline T Sum(const Vec<T, 4>& v);
template<class T> inline T AbsSum(const Vec<T, 4>& v);
template<class T> inline T Avg(const Vec<T, 4>& v);
template<class T> inline T AbsAvg(const Vec<T, 4>& v);
template<class T> inline T Near(const Vec<T, 4>& v, const Vec<T, 4>& other, T maxDiff);
template<class T> inline T NearACD(const Vec<T, 4>& v, const Vec<T, 4>& other);
template<class T> inline typename MakeFloat<T>::Type DotProduct(const Vec<T, 4>& v, const Vec<T, 4>& other);
template<class T> inline T LengthSquared(const Vec<T, 4>& v);
template<class T> inline T Length(const Vec<T, 4>& v);
template<class T> inline Bool IsNormal(const Vec<T, 4>& v);
template<class T> inline Vec<T, 4> Normalize(const Vec<T, 4>& v);
template<class T> inline Vec<T, 4> Lerp(const Vec<T, 4>& v, const Vec<T, 4>& other, typename MakeFloat<T>::Type t);
