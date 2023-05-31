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

#include "Templates.hpp"

template <class T>
class AutoPtr
{
private:

	AutoPtr(const AutoPtr&);
	AutoPtr& operator=(const AutoPtr&);

public:

	T* data;

	AutoPtr()
		: data((T*)nullptr) {}

	AutoPtr(T* data)
		: data(data) {}

	AutoPtr(RRef<AutoPtr>& other)
		: data(other->data)
	{
		other->data = (T*)nullptr;
	}

	~AutoPtr()
	{
		if(data) delete data;
	}

	AutoPtr& operator=(RRef<AutoPtr>& other)
	{
		if (this == (T*)&other) return *this;
		data = other->data;
		other->data = (T*)nullptr;
	}

	T* operator->() { return data; }
	const T* operator->() const { return data; }

	T& operator*() { return *data; }
	const T& operator*() const { return *data; }

	T* Get() { return data; }
	Bool IsNull() { return data == (T*)nullptr; }
	Bool IsValid() { return data != (T*)nullptr; }
};