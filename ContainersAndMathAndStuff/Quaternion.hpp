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
#include "Vector.hpp"

// forward declarations

template <class T, I32 sizeX, I32 sizeY>
struct Mat;

/**
* quaternion declaration
*/

template <class T>
struct Quat
{
	union
	{
		struct { T x, y, z, w; };
		struct { T xyz[3]; T _w; };
		T m[4];
	};

	//Quat();
	Quat(const T m[4]);
	Quat(T x, T y, T z, T w);
	Quat(const Vec<T, 3>& xyz, T w);
	Quat(const Vec<T, 4>& xyzw);
	Quat(const Vec<T, 3>& eulerAngles);
	Quat(T angle, const Vec<T, 3>& axis);

	Quat(const Quat<typename SelectType<I32, F32, IsSame<T, F32>::Value>::Type>& other);
	Quat(const Quat<typename SelectType<I32, F64, IsSame<T, F64>::Value>::Type>& other);

	Quat operator + (const Quat& other) const;
	Quat& operator += (const Quat<T>& other);

	Quat operator - () const;
	Quat operator - (const Quat<T>& other) const;
	Quat& operator -= (const Quat<T>& other);

	Quat operator * (const Quat<T>& other) const;
	Quat& operator *= (const Quat<T>& other);

	Quat operator * (T value) const;
	Quat& operator *= (T value);

	Quat operator / (T value) const;
	Quat& operator /= (T value);

	static Quat FromEulerAngles(const Vec<T, 3>& eulerAngles);
	static Quat FromAxisAngle(const Vec<T, 3>& axis, T angle);
};


template<class T> inline Quat<T> Abs(const Quat<T>& q);
template<class T> inline T Min(const Quat<T>& q);
template<class T> inline T AbsMin(const Quat<T>& q);
template<class T> inline T Max(const Quat<T>& q);
template<class T> inline T AbsMax(const Quat<T>& q);
template<class T> inline T Sum(const Quat<T>& q);
template<class T> inline T AbsSum(const Quat<T>& q);
template<class T> inline T Avg(const Quat<T>& q);
template<class T> inline T AbsAvg(const Quat<T>& q);
template<class T> inline T Near(const Quat<T>& q, const Quat<T>& other, T maxDiff);
template<class T> inline T NearACD(const Quat<T>& q, const Quat<T>& other);
template<class T> inline typename MakeFloat<T>::Type DotProduct(const Quat<T>& q, const Quat<T>& other);
template<class T> inline T LengthSquared(const Quat<T>& q);
template<class T> inline T Length(const Quat<T>& q);
template<class T> inline Bool IsNormal(const Quat<T>& q);
template<class T> inline Quat<T> Normalize(const Quat<T>& q);
template<class T> inline Quat<T> Conjugate(const Quat<T>& q);
template<class T> inline Quat<T> Invert(const Quat<T>& q);
template<class T> inline Quat<T> Lerp(const Quat<T>& q, const Quat<T>& other, typename MakeFloat<T>::Type t);
template<class T> inline Quat<T> Slerp(const Quat<T>& q, const Quat<T>& other, typename MakeFloat<T>::Type t);
template<class T> inline Quat<T> SlerpLongest(const Quat<T>& q, const Quat<T>& other, typename MakeFloat<T>::Type t);
template<class T> inline Quat<T> Nlerp(const Quat<T>& q, const Quat<T>& other, typename MakeFloat<T>::Type t);
