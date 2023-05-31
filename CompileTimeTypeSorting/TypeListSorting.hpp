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

#include "TypeList.hpp"

template <class ... Ts>
constexpr int _smallestType(TypeList<Ts...> ts)
{
	int uids[ts.size()]{ Ts::uid... };
	int s = 0;

	for (int i = 0; i < ts.size(); i++)
	{
		if (uids[i] < uids[s])
		{
			s = i;
		}
	}

	return s;
}

template <class ... Ts>
constexpr auto smallestType(TypeList<Ts...> ts)
{
	constexpr int i = _smallestType(ts);
	return TypeListElement<i, decltype(ts)>::Type();
}

template <class ... Ts>
constexpr TypeList<> sortTypeList(TypeList<> ts)
{
	return TypeList<>();
}

template <class ... Ts>
constexpr auto sortTypeList(TypeList<Ts...> ts)
{
	constexpr int i = _smallestType(ts);
	auto left = typename TypeListElement<i, decltype(ts)>::Left();
	auto right = typename TypeListElement<i, decltype(ts)>::Right().tail();
	auto rest = left.merge(right);

	return TypeList<>().merge<typename TypeListElement<i, decltype(ts)>::Type>().merge(sortTypeList(rest));
}