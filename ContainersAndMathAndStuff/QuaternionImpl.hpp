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

#include "Quaternion.hpp"
#include "Vector.hpp"
#include "Matrix.hpp"

/**
* quaternion
*/

/*template<class T>
inline Quat<T>::Quat()
	: x((T)0.0), y((T)0.0), z((T)0.0), w((T)1.0)
{
}*/

template<class T>
inline Quat<T>::Quat(const T m[4])
	: x(m[0]), y(m[1]), z(m[2]), w(m[3])
{
}

template<class T>
inline Quat<T>::Quat(T x, T y, T z, T w)
	: x(x), y(y), z(z), w(w)
{
}

template<class T>
inline Quat<T>::Quat(const Vec<T, 3>& xyz, T w)
	: x(xyz.x), y(xyz.y), z(xyz.z), w(w)
{
}

template<class T>
inline Quat<T>::Quat(const Vec<T, 4>& xyzw)
	: x(xyzw.x), y(xyzw.y), z(xyzw.z), w(xyzw.w)
{
}

template<class T>
inline Quat<T>::Quat(const Vec<T, 3>& eulerAngles)
	: x((T)0.0), y((T)0.0), z((T)0.0), w((T)1.0)
{
	*this = FromEulerAngles(eulerAngles);
}

template<class T>
inline Quat<T>::Quat(T angle, const Vec<T, 3>& axis)
	: x((T)0.0), y((T)0.0), z((T)0.0), w((T)1.0)
{
	*this = FromAxisAngle(axis, angle);
}

template<class T>
inline Quat<T>::Quat(const Quat<typename SelectType<I32, F32, IsSame<T, F32>::Value>::Type>& other)
	: x((T)other.x), y((T)other.y), z((T)other.z), w((T)other.w)
{
}

template<class T>
inline Quat<T>::Quat(const Quat<typename SelectType<I32, F64, IsSame<T, F64>::Value>::Type>& other)
	: x((T)other.x), y((T)other.y), z((T)other.z), w((T)other.w)
{
}

template<class T>
inline Quat<T> Quat<T>::operator+(const Quat<T>& other) const
{
	return Quat<T>(x + other.x, y + other.y, z + other.z, w + other.w);
}

template<class T>
inline Quat<T>& Quat<T>::operator+=(const Quat<T>& other)
{
	x += other.x; y += other.y; z += other.z; w += other.w;
	return *this;
}

template<class T>
inline Quat<T> Quat<T>::operator-() const
{
	return Quat<T>(-x, -y, -z, -w);
}

template<class T>
inline Quat<T> Quat<T>::operator-(const Quat<T>& other) const
{
	return Quat<T>(x - other.x, y - other.y, z - other.z, w - other.w);
}

template<class T>
inline Quat<T>& Quat<T>::operator-=(const Quat<T>& other)
{
	x -= other.x; y -= other.y; z -= other.z; w -= other.w;
	return *this;
}

template<class T>
inline Quat<T> Quat<T>::operator*(const Quat<T>& other) const
{
	Quat<T> tmp;
	tmp.x = w * other.x + x * other.w + y * other.z - z * other.y;
	tmp.y = w* other.y - x * other.z + y * other.w + z * other.x;
	tmp.z = w* other.z + x * other.y - y * other.x + z * other.w;
	tmp.w = w* other.w - x * other.x - y * other.y - z * other.z;
	return tmp;
}

template<class T>
inline Quat<T>& Quat<T>::operator*=(const Quat<T>& other)
{
	return *this = *this * other;
}

template<class T>
inline Quat<T> Quat<T>::operator*(T value) const
{
	return Quat<T>(x * value, y * value, z * value, w * value);
}

template<class T>
inline Quat<T>& Quat<T>::operator*=(T value)
{
	x *= value; y *= value; z *= value; w *= value;
	return *this;
}

template<class T>
inline Quat<T> Quat<T>::operator/(T value) const
{
	return Quat<T>(x / value, y / value, z / value, w / value);
}

template<class T>
inline Quat<T>& Quat<T>::operator/=(T value)
{
	x *= value; y *= value; z *= value; w *= value;
	return *this;
}

template<class T>
inline Quat<T> Abs(const Quat<T>& q)
{
	return Quat<T>(Abs(q.x), Abs(q.y), Abs(q.z), Abs(q.w));
}

template<class T>
inline T Min(const Quat<T>& q)
{
	return q.x < q.y ? (q.x < q.z ? (q.x < q.w ? q.x : q.w) : (q.z < q.w ? q.z : q.w))
		: (q.y < q.z ? (q.y < q.w ? q.y : q.w) : (q.z < q.w ? q.z : q.w));
}

template<class T>
inline T AbsMin(const Quat<T>& q)
{
	T abs_x = Abs(q.x), abs_y = Abs(q.y), abs_z = Abs(q.z), abs_w = Abs(q.w);
	return abs_x < abs_y ? (abs_x < abs_z ? (abs_x < abs_w ? abs_x : abs_w) : (abs_z < abs_w ? abs_z : abs_w))
		: (abs_y < abs_z ? (abs_y < abs_w ? abs_y : abs_w) : (abs_z < abs_w ? abs_z : abs_w));
}

template<class T>
inline T Max(const Quat<T>& q)
{
	return q.x > q.y ? (q.x > q.z ? (q.x > q.w ? q.x : q.w) : (q.z > q.w ? q.z : q.w))
		: (q.y > q.z ? (q.y > q.w ? q.y : q.w) : (q.z > q.w ? q.z : q.w));
}

template<class T>
inline T AbsMax(const Quat<T>& q)
{
	T abs_x = Abs(q.x), abs_y = Abs(q.y), abs_z = Abs(q.z), abs_w = Abs(q.w);
	return abs_x > abs_y ? (abs_x > abs_z ? (abs_x > abs_w ? abs_x : abs_w) : (abs_z > abs_w ? abs_z : abs_w))
		: (abs_y > abs_z ? (abs_y > abs_w ? abs_y : abs_w) : (abs_z > abs_w ? abs_z : abs_w));
}

template<class T>
inline T Sum(const Quat<T>& q)
{
	return q.x + q.y + q.z + q.w;
}

template<class T>
inline T AbsSum(const Quat<T>& q)
{
	return Abs(q.x) + Abs(q.y) + Abs(q.z) + Abs(q.w);
}

template<class T>
inline T Avg(const Quat<T>& q)
{
	return Sum(q) / (T)4.0;
}

template<class T>
inline T AbsAvg(const Quat<T>& q)
{
	return AbsSum(q) / (T)4.0;
}

template<class T>
inline T Near(const Quat<T>& q, const Quat<T>& other, T maxDiff)
{
	return Near(q.x, other.x, maxDiff) && Near(q.y, other.y, maxDiff) && Near(q.z, other.z, maxDiff) && Near(q.w, other.w, maxDiff);
}

template<class T>
inline T NearACD(const Quat<T>& q, const Quat<T>& other)
{
	return NearACD(q.x, other.x) && NearACD(q.y, other.y) && NearACD(q.z, other.z) && NearACD(q.w, other.w);
}

template<class T>
inline typename MakeFloat<T>::Type DotProduct(const Quat<T>& q, const Quat<T>& other)
{
	typedef typename MakeFloat<T>::Type _Float;
	return (_Float)q.x * (_Float)other.x + (_Float)q.y * (_Float)other.y + (_Float)q.z * (_Float)other.z + (_Float)q.w * (_Float)other.w;;
}

template<class T>
inline T LengthSquared(const Quat<T>& q)
{
	return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
}

template<class T>
inline T Length(const Quat<T>& q)
{
	return Sqrt(LengthSquared(q));
}

template<class T>
inline Bool IsNormal(const Quat<T>& q)
{
	return Near(LengthSquared(q), (T)1.0, (T)0.000001);
}

template<class T>
inline Quat<T> Normalize(const Quat<T>& q)
{
	T l = Length(q);
	return Quat<T>(q / l);
}

template<class T>
inline Quat<T> Conjugate(const Quat<T>& q)
{
	return Quat<T>(-q.x, -q.y, -q.z, q.w);
}

template<class T>
inline Quat<T> Invert(const Quat<T>& q)
{
	return Quat<T>(Conjugate(q) / LengthSquared(q));
}

template<class T>
inline Quat<T> Lerp(const Quat<T>& q, const Quat<T>& other, typename MakeFloat<T>::Type t)
{
	return q + Quat<T>(Quat<typename MakeFloat<T>::Type>(other - q) * t);
}

template<class T>
inline Quat<T> Slerp(const Quat<T>& q, const Quat<T>& other, typename MakeFloat<T>::Type t)
{
	typedef typename MakeFloat<T>::Type _Float;
	_Float dot = DotProduct(q, other);
	_Float angle = Acos(dot);
	Quat<T> tmp = dot < 0 ? -(q) : q;

	return (tmp * Sin(((_Float)1.0 - t) * angle) + other * sin(t * angle)) / Sin(angle);
}

template<class T>
inline Quat<T> SlerpLongest(const Quat<T>& q, const Quat<T>& other, typename MakeFloat<T>::Type t)
{
	typedef typename MakeFloat<T>::Type _Float;
	_Float dot = DotProduct(q, other);
	_Float angle = Acos(dot);
	Quat<T> tmp = dot < 0 ? q : -(q);

	return (tmp * Sin(((_Float)1.0 - t) * angle) + other * sin(t * angle)) / Sin(angle);
}

template<class T>
inline Quat<T> Nlerp(const Quat<T>& q, const Quat<T>& other, typename MakeFloat<T>::Type t)
{
	return Normalize(Lerp(q, other.t));
}

template<class T>
inline Quat<T> Quat<T>::FromEulerAngles(const Vec<T, 3>& eulerAngles)
{
	T cx = CosD(eulerAngles.x * 0.5f), sx = SinD(eulerAngles.x * 0.5f);
	T cy = CosD(eulerAngles.y * 0.5f), sy = SinD(eulerAngles.y * 0.5f);
	T cz = CosD(eulerAngles.z * 0.5f), sz = SinD(eulerAngles.z * 0.5f);

	return Quat<T>(
		cx * sy * sz - sx * cy * cz,
		-cx * sy * cz - sx * cy * sz,
		cx * cy * sz - sx * sy * cz,
		cx * cy * cz + sx * sy * sz
		);
}

template<class T>
inline Quat<T> Quat<T>::FromAxisAngle(const Vec<T, 3>& axis, T angle)
{
	T cos = CosD(angle * (T)0.5), sin = SinD(angle * (T)0.5);
	return Quat<T>(axis.x * sin, axis.y * sin, axis.z * sin, cos);
}


template <class T>
inline Bool operator == (const Quat<T>& a, const Quat<T>& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

template <class T>
inline Bool operator != (const Quat<T>& a, const Quat<T>& b)
{
	return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
}

template <class T>
inline Bool operator > (const Quat<T>& a, const Quat<T>& b)
{
	return (a.x == b.x ? (a.y == b.y ? (a.z == b.z ? a.w > b.w : a.z > b.z) : a.y > b.y) : a.x > b.x);
}

template <class T>
inline Bool operator >= (const Quat<T>& a, const Quat<T>& b)
{
	return (a.x == b.x ? (a.y == b.y ? (a.z == b.z ? a.w >= b.w : a.z > b.z) : a.y > b.y) : a.x > b.x);
}

template <class T>
inline Bool operator < (const Quat<T>& a, const Quat<T>& b)
{
	return (a.x == b.x ? (a.y == b.y ? (a.z == b.z ? a.w < b.w : a.z < b.z) : a.y < b.y) : a.x < b.x);
}

template <class T>
inline Bool operator <= (const Quat<T>& a, const Quat<T>& b)
{
	return (a.x == b.x ? (a.y == b.y ? (a.z == b.z ? a.w <= b.w : a.z < b.z) : a.y < b.y) : a.x < b.x);
}
