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

#include "Matrix.hpp"
#include "Vector.hpp"
#include "Quaternion.hpp"

/**
* Matrix4x4
*/

/*template<class T>
inline Mat<T, 4, 4>::Mat()
	:
	r00((T)1.0), r01((T)0.0), r02((T)0.0), r03((T)0.0),
	r10((T)0.0), r11((T)1.0), r12((T)0.0), r13((T)0.0),
	r20((T)0.0), r21((T)0.0), r22((T)1.0), r23((T)0.0),
	r30((T)0.0), r31((T)0.0), r32((T)0.0), r33((T)1.0)
{
}*/

template<class T>
inline Mat<T, 4, 4>::Mat(T value)
	:
	r00(value), r01((T)0.0), r02((T)0.0), r03((T)0.0),
	r10((T)0.0), r11(value), r12((T)0.0), r13((T)0.0),
	r20((T)0.0), r21((T)0.0), r22(value), r23((T)0.0),
	r30((T)0.0), r31((T)0.0), r32((T)0.0), r33(value)
{
}

template<class T>
inline Mat<T, 4, 4>::Mat(
	T r00, T r01, T r02, T r03,
	T r10, T r11, T r12, T r13,
	T r20, T r21, T r22, T r23,
	T r30, T r31, T r32, T r33
	) :
	r00(r00), r01(r01), r02(r02), r03(r03),
	r10(r10), r11(r11), r12(r12), r13(r13),
	r20(r20), r21(r21), r22(r22), r23(r23),
	r30(r30), r31(r31), r32(r32), r33(r33)
{
}

template<class T>
inline Mat<T, 4, 4>::Mat(const Mat<typename SelectType<I32, F32, IsSame<T, F32>::Value>::Type, 4, 4>& other)
	:
	r00(other.r00), r01(other.r01), r02(other.r02), r03(other.r03),
	r10(other.r10), r11(other.r11), r12(other.r12), r13(other.r13),
	r20(other.r20), r21(other.r21), r22(other.r22), r23(other.r23),
	r30(other.r30), r31(other.r31), r32(other.r32), r33(other.r33)
{
}

template<class T>
inline Mat<T, 4, 4>::Mat(const Mat<typename SelectType<I32, F64, IsSame<T, F64>::Value>::Type, 4, 4>& other)
	:
	r00(other.r00), r01(other.r01), r02(other.r02), r03(other.r03),
	r10(other.r10), r11(other.r11), r12(other.r12), r13(other.r13),
	r20(other.r20), r21(other.r21), r22(other.r22), r23(other.r23),
	r30(other.r30), r31(other.r31), r32(other.r32), r33(other.r33)
{
}


template<class T>
inline Mat<T, 4, 4>::Mat(const Vec<T, 4>& r0, const Vec<T, 4>& r1, const Vec<T, 4>& r2, const Vec<T, 4>& r3)
	:
	r00(r0.x), r01(r0.y), r02(r0.z), r03(r0.w),
	r10(r1.x), r11(r1.y), r12(r1.z), r13(r1.w),
	r20(r2.x), r21(r2.y), r22(r2.z), r23(r2.w),
	r30(r3.x), r31(r3.y), r32(r3.z), r33(r3.w)
{
}

template<class T>
inline Mat<T, 4, 4>::Mat(const Quat<T>& rotation)
{
	*this = FromRotation(rotation);
}

template<class T>
inline Mat<T, 4, 4> Mat<T, 4, 4>::operator+(const Mat& other) const
{
	return Mat<T, 4, 4>(
		r00 + other.r00, r01 + other.r01, r02 + other.r02, r03 + other.r03,
		r10 + other.r10, r11 + other.r11, r12 + other.r12, r13 + other.r13,
		r20 + other.r20, r21 + other.r21, r22 + other.r22, r23 + other.r23,
		r30 + other.r30, r31 + other.r31, r32 + other.r32, r33 + other.r33
	);
}

template<class T>
inline Mat<T, 4, 4>& Mat<T, 4, 4>::operator+=(const Mat& other)
{
	r00 += other.r00; r01 += other.r01; r02 += other.r02; r03 += other.r03;
	r10 += other.r10; r11 += other.r11; r12 += other.r12; r13 += other.r13;
	r20 += other.r20; r21 += other.r21; r22 += other.r22; r23 += other.r23;
	r30 += other.r30; r31 += other.r31; r32 += other.r32; r33 += other.r33;
	return *this;
}

template<class T>
inline Mat<T, 4, 4> Mat<T, 4, 4>::operator-(const Mat& other) const
{
	return Mat<T, 4, 4>(
		r00 - other.r00, r01 - other.r01, r02 - other.r02, r03 - other.r03,
		r10 - other.r10, r11 - other.r11, r12 - other.r12, r13 - other.r13,
		r20 - other.r20, r21 - other.r21, r22 - other.r22, r23 - other.r23,
		r30 - other.r30, r31 - other.r31, r32 - other.r32, r33 - other.r33
	);
}

template<class T>
inline Mat<T, 4, 4>& Mat<T, 4, 4>::operator-=(const Mat& other)
{
	r00 -= other.r00; r01 -= other.r01; r02 -= other.r02; r03 -= other.r03;
	r10 -= other.r10; r11 -= other.r11; r12 -= other.r12; r13 -= other.r13;
	r20 -= other.r20; r21 -= other.r21; r22 -= other.r22; r23 -= other.r23;
	r30 -= other.r30; r31 -= other.r31; r32 -= other.r32; r33 -= other.r33;
	return *this;
}

template<class T>
inline Mat<T, 4, 4> Mat<T, 4, 4>::operator*(const Mat& other) const
{
	return Mat<T, 4, 4>(
		r00 * other.r00 + r01 * other.r10 + r02 * other.r20 + r03 * other.r30,
		r00 * other.r01 + r01 * other.r11 + r02 * other.r21 + r03 * other.r31,
		r00 * other.r02 + r01 * other.r12 + r02 * other.r22 + r03 * other.r32,
		r00 * other.r03 + r01 * other.r13 + r02 * other.r23 + r03 * other.r33,

		r10 * other.r00 + r11 * other.r10 + r12 * other.r20 + r13 * other.r30,
		r10 * other.r01 + r11 * other.r11 + r12 * other.r21 + r13 * other.r31,
		r10 * other.r02 + r11 * other.r12 + r12 * other.r22 + r13 * other.r32,
		r10 * other.r03 + r11 * other.r13 + r12 * other.r23 + r13 * other.r33,

		r20 * other.r00 + r21 * other.r10 + r22 * other.r20 + r23 * other.r30,
		r20 * other.r01 + r21 * other.r11 + r22 * other.r21 + r23 * other.r31,
		r20 * other.r02 + r21 * other.r12 + r22 * other.r22 + r23 * other.r32,
		r20 * other.r03 + r21 * other.r13 + r22 * other.r23 + r23 * other.r33,

		r30 * other.r00 + r31 * other.r10 + r32 * other.r20 + r33 * other.r30,
		r30 * other.r01 + r31 * other.r11 + r32 * other.r21 + r33 * other.r31,
		r30 * other.r02 + r31 * other.r12 + r32 * other.r22 + r33 * other.r32,
		r30 * other.r03 + r31 * other.r13 + r32 * other.r23 + r33 * other.r33
	);
}

template<class T>
inline Mat<T, 4, 4>& Mat<T, 4, 4>::operator*=(const Mat& other)
{
	return *this = *this * other;
}

template<class T>
inline T Min(const Mat<T, 4, 4>& m)
{
	T r0_min = Min(Vec<T, 4>(m.r0)), r1_min = Min(Vec<T, 4>(m.r1)), r2_min = Min(Vec<T, 4>(m.r2)), r3_min = Min(Vec<T, 4>(m.r3));
	return r0_min < r1_min ? (r0_min < r2_min ? (r0_min < r3_min ? r0_min : r3_min) : (r2_min < r3_min ? r2_min : r3_min))
		: (r1_min < r2_min ? (r1_min < r3_min ? r1_min : r3_min) : (r2_min < r3_min ? r2_min : r3_min));
}

template<class T>
inline T AbsMin(const Mat<T, 4, 4>& m)
{
	T r0_min = MinAbs(Vec<T, 4>(m.r0)), r1_min = MinAbs(Vec<T, 4>(m.r1)), r2_min = MinAbs(Vec<T, 4>(m.r2)), r3_min = MinAbs(Vec<T, 4>(m.r3));
	return r0_min < r1_min ? (r0_min < r2_min ? (r0_min < r3_min ? r0_min : r3_min) : (r2_min < r3_min ? r2_min : r3_min))
		: (r1_min < r2_min ? (r1_min < r3_min ? r1_min : r3_min) : (r2_min < r3_min ? r2_min : r3_min));
}

template<class T>
inline T Max(const Mat<T, 4, 4>& m)
{
	T r0_max = Max(Vec<T, 4>(m.r0)), r1_max = Max(Vec<T, 4>(m.r1)), r2_max = Max(Vec<T, 4>(m.r2)), r3_max = Max(Vec<T, 4>(m.r3));
	return r0_max < r1_max ? (r0_max < r2_max ? (r0_max < r3_max ? r0_max : r3_max) : (r2_max < r3_max ? r2_max : r3_max))
		: (r1_max < r2_max ? (r1_max < r3_max ? r1_max : r3_max) : (r2_max < r3_max ? r2_max : r3_max));
}

template<class T>
inline T AbsMax(const Mat<T, 4, 4>& m)
{
	T r0_max = MaxAbs(Vec<T, 4>(m.r0)), r1_max = MaxAbs(Vec<T, 4>(m.r1)), r2_max = MaxAbs(Vec<T, 4>(m.r2)), r3_max = MaxAbs(Vec<T, 4>(m.r3));
	return r0_max < r1_max ? (r0_max < r2_max ? (r0_max < r3_max ? r0_max : r3_max) : (r2_max < r3_max ? r2_max : r3_max))
		: (r1_max < r2_max ? (r1_max < r3_max ? r1_max : r3_max) : (r2_max < r3_max ? r2_max : r3_max));
}

template<class T>
inline T Sum(const Mat<T, 4, 4>& m)
{
	return Sum(Vec<T, 4>(m.r0)) + Sum(Vec<T, 4>(m.r1)) + Sum(Vec<T, 4>(m.r2)) + Sum(Vec<T, 4>(m.r3));
}

template<class T>
inline T AbsSum(const Mat<T, 4, 4>& m)
{
	return SumAbs(Vec<T, 4>(m.r0)) + SumAbs(Vec<T, 4>(m.r1)) + SumAbs(Vec<T, 4>(m.r2)) + SumAbs(Vec<T, 4>(m.r3));
}

template<class T>
inline T Avg(const Mat<T, 4, 4>& m)
{
	return Sum(m) / (T)16.0;
}

template<class T>
inline T AbsAvg(const Mat<T, 4, 4>& m)
{
	return AbsSum(m) / (T)16.0;
}

template<class T>
inline Mat<T, 4, 4> Lerp(const Mat<T, 4, 4>& m, const Mat<T, 4, 4>& other, typename MakeFloat<T>::Type t)
{
	return Mat<T, 4, 4>(
		Lerp(Vec<T, 4>(m.r0, other.r0, t)),
		Lerp(Vec<T, 4>(m.r1, other.r1, t)),
		Lerp(Vec<T, 4>(m.r2, other.r2, t)),
		Lerp(Vec<T, 4>(m.r3, other.r3, t))
	);
}

template<class T>
inline T Near(const Mat<T, 4, 4>& m, const Mat<T, 4, 4>& other, T maxDiff)
{
	U32 i = 0;
	for (; i < 16 && Near(m.m[i], other.m[i], maxDiff); ++i);
	return i == 16;
}

template<class T>
inline T NearACD(const Mat<T, 4, 4>& m, const Mat<T, 4, 4>& other)
{
	U32 i = 0;
	for (; i < 16 && NearACD(m.m[i], other.m[i]); ++i);
	return i == 16;
}

template<class T>
inline Mat<T, 4, 4> Translate(const Mat<T, 4, 4>& m, const Vec<T, 3>& vec)
{
	Mat<T, 4, 4> tmp = m;
	tmp.r03 += vec.x;
	tmp.r13 += vec.y;
	tmp.r23 += vec.z;
	return tmp;
}

template<class T>
inline Mat<T, 4, 4> Rotate(const Mat<T, 4, 4>& m, const Quat<T>& quat)
{
	return m * FromRotation(quat);
}

template<class T>
inline Mat<T, 4, 4> Scale(const Mat<T, 4, 4>& m, const Vec<T, 3>& vec)
{
	return m * FromScale(vec);
}

template<class T>
inline Mat<T, 4, 4> Transpose(const Mat<T, 4, 4>& m)
{
	return Mat<T, 4, 4>(
		m.r00, m.r10, m.r20, m.r30,
		m.r01, m.r11, m.r21, m.r31,
		m.r02, m.r12, m.r22, m.r32,
		m.r03, m.r13, m.r23, m.r33
	);
}

template<class T>
inline Mat<T, 4, 4> Mat<T, 4, 4>::FromLocation(const Vec<T, 3>& vec)
{
	return Mat<T, 4, 4>(
		1, 0, 0, vec.x,
		0, 1, 0, vec.y,
		0, 0, 1, vec.z,
		0, 0, 0, 1
		);
}

template<class T>
inline Mat<T, 4, 4> Mat<T, 4, 4>::FromRotation(const Quat<T>& quat)
{
	return Mat<T, 4, 4>(
		1 - 2 * quat.y * quat.y - 2 * quat.z * quat.z, 2 * quat.x * quat.y - 2 * quat.w * quat.z, 2 * quat.x * quat.z + 2 * quat.w * quat.y, 0,
		2 * quat.x * quat.y + 2 * quat.w * quat.z, 1 - 2 * quat.x * quat.x - 2 * quat.z * quat.z, 2 * quat.y * quat.z + 2 * quat.w * quat.x, 0,
		2 * quat.x * quat.z - 2 * quat.w * quat.y, 2 * quat.y * quat.z - 2 * quat.w * quat.x, 1 - 2 * quat.x * quat.x - 2 * quat.y * quat.y, 0,
		0, 0, 0, 1
	);
}

template<class T>
inline Mat<T, 4, 4> Mat<T, 4, 4>::FromScale(const Vec<T, 3>& vec)
{
	return Mat<T, 4, 4>(
		vec.x, 0,     0, 0,
		0, vec.y,     0, 0,
		0,     0, vec.z, 0,
		0,     0,     0, 1
	);
}

template<class T>
inline Mat<T, 4, 4> Mat<T, 4, 4>::FromTransformation(const Vec<T, 3>& location, const Quat<T>& rotation, const Vec<T, 3>& scale)
{
	return FromLocation(location).RotateThis(rotation).ScaleThis(scale);
}

template<class T>
inline Mat<T, 4, 4> Mat<T, 4, 4>::Perspective(const T fov, const Vec<T, 2>& res, const T nearZ, const T farZ)
{
	F32 ar = res.x / res.y;
	F32 _fov = tand((fov / 2));
	F32 a = (farZ / (nearZ - farZ));
	F32 b = (farZ * nearZ / (nearZ - farZ));
	return Mat<T, 4, 4>(
		1 / ar * _fov, 0, 0,        0,
		0,             0, 1 / _fov, 0,
		0,            -a, 0,        b,
		0,             1, 0,        0
	);
}

template<class T>
inline Mat<T, 4, 4> Mat<T, 4, 4>::PerspectiveRZ(const T fov, const Vec<T, 2>& res, const T nearZ, const T farZ)
{
	F32 ar = res.x / res.y;
	F32 _fov = tand((fov / 2));
	F32 a = (farZ / (nearZ - farZ)) + 1;
	F32 b = (farZ * nearZ / (nearZ - farZ));
	return Mat<T, 4, 4>(
		1 / ar * _fov, 0,        0,  0,
		0,             0, 1 / _fov,  0,
		0,             a,        0, -b,
		0,             1,        0,  0
	);
}

template<class T>
inline Mat<T, 4, 4> Mat<T, 4, 4>::PerspectiveI(const T fov, const Vec<T, 2>& res, const T nearZ)
{
	F32 ar = res.x / res.y;
	F32 _fov = tand((fov / 2));
	return Mat<T, 4, 4>(
		1 / ar * _fov, 0,        0,      0,
		0,             0, 1 / _fov,      0,
		0,             1,        0, -nearZ,
		0,             1,        0,      0
	);
}

template<class T>
inline Mat<T, 4, 4> Mat<T, 4, 4>::PerspectiveIRZ(const T fov, const Vec<T, 2>& res, const T nearZ)
{
	F32 ar = res.x / res.y;
	F32 _fov = tand(fov / 2);
	return Mat<T, 4, 4>(
		1 / ar * _fov, 0, 0,        0,
		0,             0, 1 / _fov, 0,
		0,             0, 0,    nearZ,
		0,             1, 0,        0
	);
}


template <class T>
inline Bool operator == (const Mat<T, 4, 4>& a, const Mat<T, 4, 4>& b)
{
	U32 i = 0;
	for (; i < 16 && a.m[i] == b.m[i]; ++i);
	return i == 16;
}

template <class T>
inline Bool operator != (const Mat<T, 4, 4>& a, const Mat<T, 4, 4>& b)
{
	U32 i = 0;
	for (; i < 16 && a.m[i] == b.m[i]; ++i);
	return i != 16;
}

template <class T>
inline Bool operator > (const Mat<T, 4, 4>& a, const Mat<T, 4, 4>& b)
{
	U32 i = 0;
	for (; i < 16 && a.m[i] == b.m[i]; ++i);
	return i != 16 && a.m[i] > b.m[i];
}

template <class T>
inline Bool operator >= (const Mat<T, 4, 4>& a, const Mat<T, 4, 4>& b)
{
	U32 i = 0;
	for (; i < 16 && a.m[i] == b.m[i]; ++i);
	return i == 16 || a.m[i] > b.m[i];
}

template <class T>
inline Bool operator < (const Mat<T, 4, 4>& a, const Mat<T, 4, 4>& b)
{
	U32 i = 0;
	for (; i < 16 && a.m[i] == b.m[i]; ++i);
	return i != 16 && a.m[i] < b.m[i];
}

template <class T>
inline Bool operator <= (const Mat<T, 4, 4>& a, const Mat<T, 4, 4>& b)
{
	U32 i = 0;
	for (; i < 16 && a.m[i] == b.m[i]; ++i);
	return i == 16 || a.m[i] < b.m[i];
}
