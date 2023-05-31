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

template <class T>
constexpr bool alwaysFalse = false;

template <class ...Types>
struct TypeList
{
	constexpr int size()
	{
		return sizeof...(Types);
	}

private:
	template <class T, class ... Ts>
	struct _First
	{
		using Type = T;
		using Rest = TypeList<Ts...>;
	};

public:
	using First = typename _First<Types...>::Type;
	using Tail = typename _First<Types...>::Rest;

	template <class ... Ts>
	using Merge = TypeList<Types..., Ts...>;

	template <class ... Ts>
	using ReverseMerge = TypeList<Ts..., Types...>;

	constexpr First first()
	{
		return First();
	}

	constexpr Tail tail()
	{
		return Tail();
	}

	template<class ... Ts>
	constexpr TypeList<Types..., Ts...> merge()
	{
		return TypeList<Types..., Ts...>();
	}

	template<class ... Ts>
	constexpr TypeList<Types..., Ts...> merge(TypeList<Ts...>)
	{
		return TypeList<Types..., Ts...>();
	}

	template<class ... Ts>
	constexpr TypeList<Ts..., Types...> reverseMerge()
	{
		return TypeList<Ts..., Types...>();
	}

	template<class ... Ts>
	constexpr TypeList<Ts..., Types...> reverseMerge(TypeList<Ts...>)
	{
		return TypeList<Ts..., Types...>();
	}
	/*
private:
	template <class T, class Ty>
	struct _Reverse;

	template <class T>
	struct _Reverse<T, TypeList<>>
	{
		using Type = T;
	};

	template <class T, class Ty>
	struct _Reverse : _Reverse<typename T::template ReverseMerge<typename Ty::First>, typename Ty::Tail>
	{

	};

	template <class T, class Ty>
	struct _Last;

	template <class T, class Ty>
	struct _Last<T, TypeList<Ty>>
	{
		using Type = Ty;
		using Rest = T;
	};

	template <class T, class Ty>
	struct _Last : _Last<typename T::template Merge<typename Ty::First>, typename Ty::Tail>
	{

	};

public:
	using Head = typename _Last<TypeList<>, TypeList<Types...>>::Rest;
	using Last = typename _Last<TypeList<>, TypeList<Types...>>::Type;
	using Reverse = typename _Reverse<TypeList<>, TypeList<Types...>>::Type;

	constexpr Head head()
	{
		return Head();
	}

	constexpr Last last()
	{
		return last();
	}

	constexpr Reverse reverse()
	{
		return Reverse();
	}*/
};

template<>
struct TypeList<>
{
	template <class ... Ts>
	using Merge = TypeList<Ts...>;

	template <class ... Ts>
	using ReverseMerge = TypeList<Ts...>;

	template<class ... T>
	constexpr TypeList<T...> merge()
	{
		return TypeList<T...>();
	}

	template<class ... T>
	constexpr TypeList<T...> merge(TypeList<T...>)
	{
		return TypeList<T...>();
	}

	template<class ... T>
	constexpr ReverseMerge<T...> reverseMerge()
	{
		return ReverseMerge<T...>();
	}

	template<class ... T>
	constexpr ReverseMerge<T...> reverseMerge(TypeList<T...>)
	{
		return ReverseMerge<T...>();
	}

	constexpr int size()
	{
		return 0;
	}
};

/*

*/
template<int I, class PreSeq, class PostSeq>
struct _TypeListElement;

template<class PreSeq>
struct _TypeListElement<0, PreSeq, TypeList<>>
{
	static_assert(alwaysFalse<_TypeListElement<0, PreSeq, TypeList<>>>, "Type sequence out of bounds!");
};

template<int I, class PreSeq>
struct _TypeListElement<I, PreSeq, TypeList<>>
{
	static_assert(alwaysFalse<_TypeListElement<I, PreSeq, TypeList<>>>, "Type sequence out of bounds!");
};

template<class PreSeq, class PostSeq>
struct _TypeListElement<0, PreSeq, PostSeq>
{
	using Type = typename PostSeq::First;
	using Left = PreSeq;
	using Right = PostSeq;
};

template<int I, class PreSeq, class PostSeq>
struct _TypeListElement : _TypeListElement<I - 1, decltype(PreSeq().template merge<typename PostSeq::First>()), typename PostSeq::Tail>
{
	
};

template<int I, class Seq>
using TypeListElement = _TypeListElement<I, TypeList<>, Seq>;
