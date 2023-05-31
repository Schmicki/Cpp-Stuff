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
#include "VectorImpl.hpp"
#include "MatrixImpl.hpp"
#include "QuaternionImpl.hpp"

// short type aliases

typedef Vec<Flt, 2> Vec2Flt;
typedef Vec<F32, 2> Vec2F32;
typedef Vec<F64, 2> Vec2F64;

typedef Vec<Flt, 3> Vec3Flt;
typedef Vec<F32, 3> Vec3F32;
typedef Vec<F64, 3> Vec3F64;

typedef Vec<Flt, 4> Vec4Flt;
typedef Vec<F32, 4> Vec4F32;
typedef Vec<F64, 4> Vec4F64;

typedef Mat<Flt, 4, 4> Mat4Flt;
typedef Mat<F32, 4, 4> Mat4F32;
typedef Mat<F64, 4, 4> Mat4F64;

typedef Quat<Flt> QuatFlt;
typedef Quat<F32> QuatF32;
typedef Quat<F64> QuatF64;