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

template <class T>
struct Quat;

/**
* matrix declaration
*/

template <class T, I32 sizeX, I32 sizeY>
struct Mat
{
	/*union
	{
		T r[sizeX][sizeY];
		T m[sizeX * sizeY];
	};

	Mat();
	Mat(T value);

	Mat operator + (const Mat& other);
	Mat& operator += (const Mat& other);

	Mat operator - (const Mat& other);
	Mat& operator -= (const Mat& other);

	Mat operator * (const Mat& other);
	Mat& operator *= (const Mat& other);

	Mat transposed();
	Mat& transpose();*/
};

template <class T>
struct Mat<T, 4, 4>
{
	typedef typename SelectType<T, F32, IsFloat<T>::Value>::Type _Float;

	union
	{
		struct { T r00, r01, r02, r03, r10, r11, r12, r13, r20, r21, r22, r23, r30, r31, r32, r33; };
		struct { T r0[4], r1[4], r2[4], r3[4]; };
		T r[4][4];
		T m[16];
	};

	//Mat();
	Mat(T value);
	Mat(
		T r00, T r01, T r02, T r03,
		T r10, T r11, T r12, T r13,
		T r20, T r21, T r22, T r23,
		T r30, T r31, T r32, T r33
	);
	Mat(const Vec<T, 4>& r0, const Vec<T, 4>& r1, const Vec<T, 4>& r2, const Vec<T, 4>& r3);
	Mat(const Quat<T>& rotation);

	Mat(const Mat<typename SelectType<I32, F32, IsSame<T, F32>::Value>::Type, 4, 4>& other);
	Mat(const Mat<typename SelectType<I32, F64, IsSame<T, F64>::Value>::Type, 4, 4>& other);


	Mat operator + (const Mat& other) const;
	Mat& operator += (const Mat& other);

	Mat operator - (const Mat& other) const;
	Mat& operator -= (const Mat& other);

	Mat operator * (const Mat& other) const;
	Mat& operator *= (const Mat& other);

	static Mat FromLocation(const Vec<T, 3>& vec);
	static Mat FromRotation(const Quat<T>& quat);
	static Mat FromScale(const Vec<T, 3>& vec);
	static Mat FromTransformation(const Vec<T, 3>& location, const Quat<T>& rotation, const Vec<T, 3>& scale);

	static Mat Perspective(const T fov, const Vec<T, 2>& res, const T nearZ, const T farZ);
	static Mat PerspectiveRZ(const T fov, const Vec<T, 2>& res, const T nearZ, const T farZ);
	static Mat PerspectiveI(const T fov, const Vec<T, 2>& res, const T nearZ);
	static Mat PerspectiveIRZ(const T fov, const Vec<T, 2>& res, const T nearZ);
};


template<class T> inline T Min(const Mat<T, 4, 4>& m);
template<class T> inline T AbsMin(const Mat<T, 4, 4>& m);
template<class T> inline T Max(const Mat<T, 4, 4>& m);
template<class T> inline T AbsMax(const Mat<T, 4, 4>& m);
template<class T> inline T Sum(const Mat<T, 4, 4>& m);
template<class T> inline T AbsSum(const Mat<T, 4, 4>& m);
template<class T> inline T Avg(const Mat<T, 4, 4>& m);
template<class T> inline T AbsAvg(const Mat<T, 4, 4>& m);
template<class T> inline Mat<T, 4, 4> Lerp(const Mat<T, 4, 4>& m, const Mat<T, 4, 4>& other, typename MakeFloat<T>::Type t);
template<class T> inline T Near(const Mat<T, 4, 4>& m, const Mat<T, 4, 4>& other, T maxDiff);
template<class T> inline T NearACD(const Mat<T, 4, 4>& m, const Mat<T, 4, 4>& other);
template<class T> inline Mat<T, 4, 4> Translate(const Mat<T, 4, 4>& m, const Vec<T, 3>& vec);
template<class T> inline Mat<T, 4, 4> Rotate(const Mat<T, 4, 4>& m, const Quat<T>& quat);
template<class T> inline Mat<T, 4, 4> Scale(const Mat<T, 4, 4>& m, const Vec<T, 3>& vec);
template<class T> inline Mat<T, 4, 4> Transpose(const Mat<T, 4, 4>& m);