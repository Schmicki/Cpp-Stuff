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

#include "Vector.hpp"
#include "Matrix.hpp"
#include "Quaternion.hpp"

/**
* Vec 2d
*/

/*template<class T>
inline Vec<T, 2>::Vec()
	: x((T)0.0), y((T)0.0)
{
}*/

template<class T>
inline Vec<T, 2>::Vec(T val)
	: x(val), y(val)
{
}

template<class T>
inline Vec<T, 2>::Vec(const T m[2])
	: x(m[0]), y(m[1])
{
}

template<class T>
inline Vec<T, 2>::Vec(T x, T y)
	: x(x), y(y)
{
}

template<class T>
inline Vec<T, 2>::Vec(const Vec<T, 3>& xy)
	: x(xy.x), y(xy.y)
{
}

template<class T>
inline Vec<T, 2>::Vec(const Vec<T, 4>& xy)
	: x(xy.x), y(xy.y)
{
}

template<class T>
inline Vec<T, 2>::Vec(const Vec<typename SelectType<I32, F32, IsSame<T, F32>::Value>::Type, 2>& other)
	: x((T)other.x), y((T)other.y)
{
}

template<class T>
inline Vec<T, 2>::Vec(const Vec<typename SelectType<I32, F64, IsSame<T, F64>::Value>::Type, 2>& other)
	: x((T)other.x), y((T)other.y)
{
}

template<class T>
inline Vec<T, 2> Vec<T, 2>::operator+(const Vec& other) const
{
	return Vec<T, 2>(x + other.x, y + other.y);
}

template<class T>
inline Vec<T, 2>& Vec<T, 2>::operator+=(const Vec& other)
{
	x += other.x; y += other.y;
	return *this;
}

template<class T>
inline Vec<T, 2> Vec<T, 2>::operator-() const
{
	return Vec<T, 2>(-x, -y);
}

template<class T>
inline Vec<T, 2> Vec<T, 2>::operator-(const Vec& other) const
{
	return Vec<T, 2>(x - other.x, y - other.y);
}

template<class T>
inline Vec<T, 2>& Vec<T, 2>::operator-=(const Vec& other)
{
	x -= other.x; y -= other.y;
	return *this;
}

template<class T>
inline Vec<T, 2> Vec<T, 2>::operator*(T value) const
{
	return Vec<T, 2>(x * value, y * value);
}

template<class T>
inline Vec<T, 2>& Vec<T, 2>::operator*=(T value)
{
	x *= value; y *= value;
	return *this;
}

template<class T>
inline Vec<T, 2> Vec<T, 2>::operator/(T value) const
{
	return Vec<T, 2>(x / value, y / value);
}

template<class T>
inline Vec<T, 2>& Vec<T, 2>::operator/=(T value)
{
	x /= value; y /= value;
	return *this;
}


template<class T>
inline Vec<T, 2> Abs(const Vec<T, 2>& v)
{
	return Vec<T, 2>(Abs(v.x), Abs(v.y));
}

template<class T>
inline T Min(const Vec<T, 2>& v)
{
	return v.x < v.y ? v.x : v.y;
}

template<class T>
inline T AbsMin(const Vec<T, 2>& v)
{
	T abs_x = Abs(v.x), abs_y = Abs(v.y);
	return abs_x < abs_y ? abs_x : abs_y;
}

template<class T>
inline T Max(const Vec<T, 2>& v)
{
	return v.x > v.y ? v.x : v.y;
}

template<class T>
inline T AbsMax(const Vec<T, 2>& v)
{
	T abs_x = Abs(v.x), abs_y = Abs(v.y);
	return abs_x > abs_y ? abs_x : abs_y;
}

template<class T>
inline T Sum(const Vec<T, 2>& v)
{
	return v.x + v.y;
}

template<class T>
inline T AbsSum(const Vec<T, 2>& v)
{
	return Abs(v.x) + Abs(v.y);
}

template<class T>
inline T Avg(const Vec<T, 2>& v)
{
	return Sum(v) / (T)2.0;
}

template<class T>
inline T AbsAvg(const Vec<T, 2>& v)
{
	return AbsSum(v) / (T)2.0;
}

template<class T>
inline T Near(const Vec<T, 2>& v, const Vec<T, 2>& other, T maxDiff)
{
	return Near(v.x, other.x, maxDiff) && Near(v.y, other.y, maxDiff);
}

template<class T>
inline T NearACD(const Vec<T, 2>& v, const Vec<T, 2>& other)
{
	return NearACD(v.x, other.x) && NearACD(v.y, other.y);
}

template<class T>
inline T LengthSquared(const Vec<T, 2>& v)
{
	return v.x * v.x + v.y * v.y;
}

template<class T>
inline T Length(const Vec<T, 2>& v)
{
	return Sqrt(LengthSquared(v));
}

template<class T>
inline Bool IsNormal(const Vec<T, 2>& v)
{
	return Near(LengthSquared(v), (T)1.0, (T)0.000001);
}

template<class T>
inline Vec<T, 2> Normalize(const Vec<T, 2>& v)
{
	T l = Length(v);
	return v / l;
}

template<class T>
inline T DistanceSquared(const Vec<T, 2>& v, const Vec<T, 2>& other)
{
	return LengthSquared(other - v);
}

template<class T>
inline T Distance(const Vec<T, 2>& v, const Vec<T, 2>& other)
{
	return Length(other - v);
}

template<class T>
inline typename MakeFloat<T>::Type DotProduct(const Vec<T, 2>& v, const Vec<T, 2>& other)
{
	typedef typename MakeFloat<T>::Type _Float;
	return (_Float)v.x * (_Float)other.x + (_Float)v.y * (_Float)other.y;
}

template<class T>
inline Vec<T, 2> Lerp(const Vec<T, 2>& v, const Vec<T, 2>& other, typename MakeFloat<T>::Type t)
{
	return v + Vec<T, 2>(Vec<typename MakeFloat<T>::Type, 2>(other - v) * t);
}

template<class T>
inline Vec<T, 2> Slerp(const Vec<T, 2>& v, const Vec<T, 2>& other, typename MakeFloat<T>::Type t)
{
	return Vec<T, 2>();
}

template <class T> Vec<T, 2> Vec<T, 2>::Zero = Vec<T, 3>((T)0.0, (T)0.0);
template <class T> Vec<T, 2> Vec<T, 2>::Right = Vec<T, 3>((T)0.0, (T)1.0);
template <class T> Vec<T, 2> Vec<T, 2>::Left = Vec<T, 3>((T)0.0, (T)-1.0);
template <class T> Vec<T, 2> Vec<T, 2>::Up = Vec<T, 3>((T)0.0, (T)1.0);
template <class T> Vec<T, 2> Vec<T, 2>::Down = Vec<T, 3>((T)0.0, (T)-1.0);


template <class T>
inline Bool operator == (const Vec<T, 2>& a, const Vec<T, 2>& b)
{
	return a.x == b.x && a.y == b.y;
}

template <class T>
inline Bool operator != (const Vec<T, 2>& a, const Vec<T, 2>& b)
{
	return a.x != b.x || a.y != b.y;
}

template <class T>
inline Bool operator > (const Vec<T, 2>& a, const Vec<T, 2>& b)
{
	return (a.x > b.x ? true : (a.x == b.x ? a.y > b.y : false));
}

template <class T>
inline Bool operator >= (const Vec<T, 2>& a, const Vec<T, 2>& b)
{
	return (a.x > b.x ? true : (a.x == b.x ? a.y >= b.y : false));
}

template <class T>
inline Bool operator < (const Vec<T, 2>& a, const Vec<T, 2>& b)
{
	return (a.x < b.x ? true : (a.x == b.x ? a.y < b.y : false));
}

template <class T>
inline Bool operator <= (const Vec<T, 2>& a, const Vec<T, 2>& b)
{
	return (a.x < b.x ? true : (a.x == b.x ? a.y <= b.y : false));
}



/**
* vector 3d
*/

/*template<class T>
inline Vec<T, 3>::Vec()
	: x((T)0.0), y((T)0.0), z((T)0.0)
{
}*/

template<class T>
inline Vec<T, 3>::Vec(T val)
	: x(val), y(val), z(val)
{
}

template<class T>
inline Vec<T, 3>::Vec(const T m[3])
	: x(m[0]), y(m[1]), z(m[2])
{
}

template<class T>
inline Vec<T, 3>::Vec(T x, T y, T z)
	: x(x), y(y), z(z)
{
}

template<class T>
inline Vec<T, 3>::Vec(const Vec<T, 2>& xy, T z)
	: x(xy.x), y(xy.y), z(z)
{
}

template<class T>
inline Vec<T, 3>::Vec(T x, const Vec<T, 2>& yz)
	: x(x), y(yz.x), z(yz.y)
{
}

template<class T>
inline Vec<T, 3>::Vec(const Vec<T, 4>& xyz)
	: x(xyz.x), y(xyz.y), z(xyz.z)
{
}

template<class T>
inline Vec<T, 3>::Vec(const Quat<T>& xyz)
	: x(xyz.x), y(xyz.y), z(xyz.z)
{
}

template<class T>
inline Vec<T, 3>::Vec(const Vec<typename SelectType<I32, F32, IsSame<T, F32>::Value>::Type, 3>& other)
	: x((T)other.x), y((T)other.y), z((T)other.z)
{
}

template<class T>
inline Vec<T, 3>::Vec(const Vec<typename SelectType<I32, F64, IsSame<T, F64>::Value>::Type, 3>& other)
	: x((T)other.x), y((T)other.y), z((T)other.z)
{
}

template<class T>
inline Vec<T, 3> Vec<T, 3>::operator+(const Vec& other) const
{
	return Vec<T, 3>(x + other.x, y + other.y, z + other.z);
}

template<class T>
inline Vec<T, 3>& Vec<T, 3>::operator+=(const Vec& other)
{
	x += other.x; y += other.y; z += other.z;
	return *this;
}

template<class T>
inline Vec<T, 3> Vec<T, 3>::operator-() const
{
	return Vec<T, 3>(-x, -y, -z);
}

template<class T>
inline Vec<T, 3> Vec<T, 3>::operator-(const Vec& other) const
{
	return Vec<T, 3>(x - other.x, y - other.y, z - other.z);
}

template<class T>
inline Vec<T, 3>& Vec<T, 3>::operator-=(const Vec& other)
{
	x -= other.x; y -= other.y; z -= other.z;
	return *this;
}

template<class T>
inline Vec<T, 3> Vec<T, 3>::operator*(T value) const
{
	return Vec<T, 3>(x * value, y * value, z * value);
}

template<class T>
inline Vec<T, 3>& Vec<T, 3>::operator*=(T value)
{
	x *= value; y *= value; z *= value;
	return *this;
}

template<class T>
inline Vec<T, 3> Vec<T, 3>::operator/(T value) const
{
	return Vec<T, 3>(x / value, y / value, z / value);
}

template<class T>
inline Vec<T, 3>& Vec<T, 3>::operator/=(T value)
{
	x /= value; y /= value; z /= value;
	return *this;
}

template<class T>
inline Vec<T, 3> Abs(const Vec<T, 3>& v)
{
	return Vec<T, 3>(Abs(v.x), Abs(v.y), Abs(v.z));
}

template<class T>
inline T Min(const Vec<T, 3>& v)
{
	return v.x < v.y ? (v.x < v.z ? v.x : v.z) : (v.y < v.z ? v.y : v.z);
}

template<class T>
inline T AbsMin(const Vec<T, 3>& v)
{
	T abs_x = Abs(v.x), abs_y = Abs(v.y), abs_z = Abs(v.z);
	return abs_x < abs_y ? (abs_x < abs_z ? abs_x : abs_z)
		: (abs_y < abs_z ? abs_y : abs_z);
}

template<class T>
inline T Max(const Vec<T, 3>& v)
{
	return v.x > v.y ? (v.x > v.z ? v.x : v.z) : (v.y > v.z ? v.y : v.z);
}

template<class T>
inline T AbsMax(const Vec<T, 3>& v)
{
	T abs_x = Abs(v.x), abs_y = Abs(v.y), abs_z = Abs(v.z);
	return abs_x > abs_y ? (abs_x > abs_z ? abs_x : abs_z)
		: (abs_y > abs_z ? abs_y : abs_z);
}

template<class T>
inline T Sum(const Vec<T, 3>& v)
{
	return v.x + v.y + v.z;
}

template<class T>
inline T AbsSum(const Vec<T, 3>& v)
{
	return Abs(v.x) + Abs(v.y) + Abs(v.z);
}

template<class T>
inline T Avg(const Vec<T, 3>& v)
{
	return Sum(v) / (T)3.0;
}

template<class T>
inline T AbsAvg(const Vec<T, 3>& v)
{
	return AbsSum(v) / (T)3.0;
}

template<class T>
inline T Near(const Vec<T, 3>& v, const Vec<T, 3>& other, T maxDiff)
{
	return Near(v.x, other.x, maxDiff) && Near(v.y, other.y, maxDiff) && Near(v.z, other.z, maxDiff);
}

template<class T>
inline T NearACD(const Vec<T, 3>& v, const Vec<T, 3>& other)
{
	return NearACD(v.x, other.x) && NearACD(v.y, other.y) && NearACD(v.z, other.z);
}

template<class T>
inline Bool IsNormal(const Vec<T, 3>& v)
{
	return Near(LengthSquared(v), (T)1.0, (T)0.000001);
}

template<class T>
inline typename MakeFloat<T>::Type DotProduct(const Vec<T, 3>& v, const Vec<T, 3>& other)
{
	typedef typename MakeFloat<T>::Type _Float;
	return (_Float)v.x * (_Float)other.x + (_Float)v.y * (_Float)other.y + (_Float)v.z * (_Float)other.z;
}

template<class T>
inline Vec<T, 3> CrossProduct(const Vec<T, 3>& v, const Vec<T, 3>& other)
{
	return Vec<T, 3> (
		v.y * other.z - v.z * other.y,
		v.z * other.x - v.x * other.z,
		v.x * other.y - v.y * other.x
	);
}

template<class T>
inline T LengthSquared(const Vec<T, 3>& v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

template<class T>
inline T Length(const Vec<T, 3>& v)
{
	return Sqrt(LengthSquared(v));
}

template<class T>
inline T DistanceSquared(const Vec<T, 3>& v, const Vec<T, 3>& other)
{
	return LengthSquared(other - v);
}

template<class T>
inline T Distance(const Vec<T, 3>& v, const Vec<T, 3>& other)
{
	return Length(other - v);
}

template<class T>
inline Vec<T, 3> Normalize(const Vec<T, 3>& v)
{
	T l = Length(v);
	return Vec<T, 3>(v.x / l, v.y / l, v.z / l);
}

template<class T>
inline Vec<T, 3> Lerp(const Vec<T, 3>& v, const Vec<T, 3>& other, typename MakeFloat<T>::Type t)
{
	return v + Vec<T, 3>(Vec<typename MakeFloat<T>::Type, 3>(other - v) * t);
}

template<class T>
inline Vec<T, 3> Slerp(const Vec<T, 3>& v, const Vec<T, 3>& other, typename MakeFloat<T>::Type t)
{
	return Vec<T, 3>();
}

template<class T>
inline Vec<T, 3> Rotate(const Vec<T, 3>& v, const Quat<T>& rotation)
{
	return rotation * Quat<T>(v, (T)0.0) * Conjugate(rotation);
}

template<class T>
inline Vec<T, 3> Rotate(const Vec<T, 3>& v, const Vec<T, 3>& eulerAngles)
{
	return Rotate(v, Quat<T>::FromEulerAngles(eulerAngles));
}

template<class T>
inline Vec<T, 3> Rotate(const Vec<T, 3>& v, const Vec<T, 3>& axis, T angle)
{
	return Rotate(v, Quat<T>::FromAxisAngle(axis, angle));
}

template<class T>
inline Vec<T, 3> Transform(const Vec<T, 3>& v, const Mat<T, 4, 4>& m)
{
	return Vec<T, 3>(
		v.x * m.r00 + v.y * m.r01 + v.z * m.r02 + m.r03,
		v.x * m.r10 + v.y * m.r11 + v.z * m.r12 + m.r13,
		v.x * m.r20 + v.y * m.r21 + v.z * m.r22 + m.r23
		);
}

template <class T> Vec<T, 3> Vec<T, 3>::Zero = Vec<T, 3>((T)0.0, (T)0.0, (T)0.0);
template <class T> Vec<T, 3> Vec<T, 3>::Forward = Vec<T, 3>((T)1.0, (T)0.0, (T)0.0);
template <class T> Vec<T, 3> Vec<T, 3>::Backward = Vec<T, 3>((T)-1.0, (T)0.0, (T)0.0);
template <class T> Vec<T, 3> Vec<T, 3>::Right = Vec<T, 3>((T)0.0, (T)1.0, (T)0.0);
template <class T> Vec<T, 3> Vec<T, 3>::Left = Vec<T, 3>((T)0.0, (T)-1.0, (T)0.0);
template <class T> Vec<T, 3> Vec<T, 3>::Up = Vec<T, 3>((T)0.0, (T)0.0, (T)1.0);
template <class T> Vec<T, 3> Vec<T, 3>::Down = Vec<T, 3>((T)0.0, (T)0.0, (T)-1.0);


template <class T>
inline Bool operator == (const Vec<T, 3>& a, const Vec<T, 3>& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

template <class T>
inline Bool operator != (const Vec<T, 3>& a, const Vec<T, 3>& b)
{
	return a.x != b.x || a.y != b.y || a.z != b.z;
}

template <class T>
inline Bool operator > (const Vec<T, 3>& a, const Vec<T, 3>& b)
{
	return (a.x == b.x ? (a.y == b.y ? a.z > b.z : a.y > b.y) : a.x > b.x);
}

template <class T>
inline Bool operator >= (const Vec<T, 3>& a, const Vec<T, 3>& b)
{
	return (a.x == b.x ? (a.y == b.y ? a.z >= b.z : a.y > b.y) : a.x > b.x);
}

template <class T>
inline Bool operator < (const Vec<T, 3>& a, const Vec<T, 3>& b)
{
	return (a.x == b.x ? (a.y == b.y ? a.z < b.z : a.y < b.y) : a.x < b.x);
}

template <class T>
inline Bool operator <= (const Vec<T, 3>& a, const Vec<T, 3>& b)
{
	return (a.x == b.x ? (a.y == b.y ? a.z <= b.z : a.y < b.y) : a.x < b.x);
}



/**
* vector 4d
*/

/*template<class T>
inline Vec<T, 4>::Vec()
	: x((T)0.0), y((T)0.0), z((T)0.0), w((T)0.0)
{
}*/

template<class T>
inline Vec<T, 4>::Vec(T val)
	: x(val), y(val), z(val), w(val)
{
}

template<class T>
inline Vec<T, 4>::Vec(const T m[4])
	: x(m[0]), y(m[1]), z(m[2]), w(m[3])
{
}

template<class T>
inline Vec<T, 4>::Vec(T x, T y, T z, T w)
	: x(x), y(y), z(z), w(w)
{
}

template<class T>
inline Vec<T, 4>::Vec(const Vec<T, 2>& xy, T z, T w)
	: x(xy.x), y(xy.y), z(z), w(w)
{
}

template<class T>
inline Vec<T, 4>::Vec(T x, T y, const Vec<T, 2>& zw)
	: x(x), y(y), z(zw.x), w(zw.y)
{
}

template<class T>
inline Vec<T, 4>::Vec(T x, const Vec<T, 2>& yz, T w)
	: x(x), y(yz.x), z(yz.y), w(w)
{
}

template<class T>
inline Vec<T, 4>::Vec(const Vec<T, 2>& xy, const Vec<T, 2>& zw)
	: x(xy.x), y(xy.y), z(zw.x), w(zw.y)
{
}

template<class T>
inline Vec<T, 4>::Vec(const Vec<T, 3>& xyz, T w)
	: x(xyz.x), y(xyz.y), z(xyz.z), w(w)
{
}

template<class T>
inline Vec<T, 4>::Vec(T x, const Vec<T, 3>& yzw)
	: x(x), y(yzw.x), z(yzw.y), w(yzw.z)
{
}

template<class T>
inline Vec<T, 4>::Vec(const Quat<T>& quaternion)
	: x(quaternion.x), y(quaternion.y), z(quaternion.z), w(quaternion.w)
{
}

template<class T>
inline Vec<T, 4>::Vec(const Vec<typename SelectType<I32, F32, IsSame<T, F32>::Value>::Type, 4>& other)
	: x((T)other.x), y((T)other.y), z((T)other.z), w((T)other.w)
{
}

template<class T>
inline Vec<T, 4>::Vec(const Vec<typename SelectType<I32, F64, IsSame<T, F64>::Value>::Type, 4>& other)
	: x((T)other.x), y((T)other.y), z((T)other.z), w((T)other.w)
{
}

template<class T>
inline Vec<T, 4> Vec<T, 4>::operator+(const Vec& other) const
{
	return Vec<T, 4>(x + other.x, y + other.y, z + other.z, w + other.w);
}

template<class T>
inline Vec<T, 4>& Vec<T, 4>::operator+=(const Vec& other)
{
	x += other.x; y += other.y; z += other.z; w += other.w;
	return *this;
}

template<class T>
inline Vec<T, 4> Vec<T, 4>::operator-() const
{
	return Vec<T, 4>(-x, -y, -z, -w);
}

template<class T>
inline Vec<T, 4> Vec<T, 4>::operator-(const Vec& other) const
{
	return Vec<T, 4>(x - other.x, y - other.y, z - other.z, w - other.w);
}

template<class T>
inline Vec<T, 4>& Vec<T, 4>::operator-=(const Vec& other)
{
	x -= other.x; y -= other.y; z -= other.z; w -= other.w;
	return *this;
}

template<class T>
inline Vec<T, 4> Vec<T, 4>::operator*(T value) const
{
	return Vec<T, 4>(x * value, y * value, z * value, w * value);
}

template<class T>
inline Vec<T, 4>& Vec<T, 4>::operator*=(T value)
{
	x *= value; y *= value; z *= value; w *= value;
	return *this;
}

template<class T>
inline Vec<T, 4> Vec<T, 4>::operator/(T value) const
{
	return Vec<T, 4>(x / value, y / value, z / value, w / value);
}

template<class T>
inline Vec<T, 4>& Vec<T, 4>::operator/=(T value)
{
	x /= value; y /= value; z /= value; w /= value;
	return *this;
}

template<class T>
inline Vec<T, 4> Abs(const Vec<T, 4>& v)
{
	return Vec<T, 4>(Abs(v.x), Abs(v.y), Abs(v.z), Abs(v.w));
}

template<class T>
inline T Min(const Vec<T, 4>& v)
{
	return v.x < v.y ? (v.x < v.z ? (v.x < v.w ? v.x : v.w) : (v.z < v.w ? v.z : v.w))
		: (v.y < v.z ? (v.y < v.w ? v.y : v.w) : (v.z < v.w ? v.z : v.w));
}

template<class T>
inline T AbsMin(const Vec<T, 4>& v)
{
	T abs_x = Abs(v.x), abs_y = Abs(v.y), abs_z = Abs(v.z), abs_w = Abs(v.w);
	return abs_x < abs_y ? (abs_x < abs_z ? (abs_x < abs_w ? abs_x : abs_w) : (abs_z < abs_w ? abs_z : abs_w))
		: (abs_y < abs_z ? (abs_y < abs_w ? abs_y : abs_w) : (abs_z < abs_w ? abs_z : abs_w));
}

template<class T>
inline T Max(const Vec<T, 4>& v)
{
	return v.x > v.y ? (v.x > v.z ? (v.x > v.w ? v.x : v.w) : (v.z > v.w ? v.z : v.w))
		: (v.y > v.z ? (v.y > v.w ? v.y : v.w) : (v.z > v.w ? v.z : v.w));
}

template<class T>
inline T AbsMax(const Vec<T, 4>& v)
{
	T abs_x = Abs(v.x), abs_y = Abs(v.y), abs_z = Abs(v.z), abs_w = Abs(v.w);
	return abs_x > abs_y ? (abs_x > abs_z ? (abs_x > abs_w ? abs_x : abs_w) : (abs_z > abs_w ? abs_z : abs_w))
		: (abs_y > abs_z ? (abs_y > abs_w ? abs_y : abs_w) : (abs_z > abs_w ? abs_z : abs_w));
}

template<class T>
inline T Sum(const Vec<T, 4>& v)
{
	return v.x + v.y + v.z + v.w;
}

template<class T>
inline T AbsSum(const Vec<T, 4>& v)
{
	return Abs(v.x) + Abs(v.y) + Abs(v.z) + Abs(v.w);
}

template<class T>
inline T Avg(const Vec<T, 4>& v)
{
	return Sum(v) / (T)4.0;
}

template<class T>
inline T AbsAvg(const Vec<T, 4>& v)
{
	return AbsSum(v) / (T)4.0;
}

template<class T>
inline T Near(const Vec<T, 4>& v, const Vec<T, 4>& other, T maxDiff)
{
	return Near(v.x, other.x, maxDiff) && Near(v.y, other.y, maxDiff) && Near(v.z, other.z, maxDiff) && Near(v.w, other.w, maxDiff);
}

template<class T>
inline T NearACD(const Vec<T, 4>& v, const Vec<T, 4>& other)
{
	return NearACD(v.x, other.x) && NearACD(v.y, other.y) && NearACD(v.z, other.z) && NearACD(v.w, other.w);
}

template<class T>
inline typename MakeFloat<T>::Type DotProduct(const Vec<T, 4>& v, const Vec<T, 4>& other)
{
	typedef typename MakeFloat<T>::Type _Float;
	return (_Float)v.x * (_Float)other.x + (_Float)v.y * (_Float)other.y + (_Float)v.z * (_Float)other.z + (_Float)v.w * (_Float)other.w;
}

template<class T>
inline T LengthSquared(const Vec<T, 4>& v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

template<class T>
inline T Length(const Vec<T, 4>& v)
{
	return Sqrt(LengthSquared(v));
}

template<class T>
inline Bool IsNormal(const Vec<T, 4>& v)
{
	return Near(LengthSquared(v), (T)1.0, (T)0.000001);
}

template<class T>
inline Vec<T, 4> Normalize(const Vec<T, 4>& v)
{
	T l = Length(v);
	return Vec<T, 4>(v.x / l, v.y / l, v.z / l, v.w / l);
}

template<class T>
inline Vec<T, 4> Lerp(const Vec<T, 4>& v, const Vec<T, 4>& other, typename MakeFloat<T>::Type t)
{
	return v + Vec<T, 4>(Vec<typename MakeFloat<T>::Type, 4>(other - v) * t);
}

template <class T>
inline Bool operator == (const Vec<T, 4>& a, const Vec<T, 4>& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

template <class T>
inline Bool operator != (const Vec<T, 4>& a, const Vec<T, 4>& b)
{
	return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
}

template <class T>
inline Bool operator > (const Vec<T, 4>& a, const Vec<T, 4>& b)
{
	return (a.x == b.x ? (a.y == b.y ? (a.z == b.z ? a.w > b.w : a.z > b.z) : a.y > b.y) : a.x > b.x);
}

template <class T>
inline Bool operator >= (const Vec<T, 4>& a, const Vec<T, 4>& b)
{
	return (a.x == b.x ? (a.y == b.y ? (a.z == b.z ? a.w >= b.w : a.z > b.z) : a.y > b.y) : a.x > b.x);
}

template <class T>
inline Bool operator < (const Vec<T, 4>& a, const Vec<T, 4>& b)
{
	return (a.x == b.x ? (a.y == b.y ? (a.z == b.z ? a.w < b.w : a.z < b.z) : a.y < b.y) : a.x < b.x);
}

template <class T>
inline Bool operator <= (const Vec<T, 4>& a, const Vec<T, 4>& b)
{
	return (a.x == b.x ? (a.y == b.y ? (a.z == b.z ? a.w <= b.w : a.z < b.z) : a.y < b.y) : a.x < b.x);
}
