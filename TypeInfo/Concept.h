#pragma once

#include "IsBaseOf.h"

namespace sge
{
	template <bool B, class T, class ... Ts>
	struct _Concept;

	template <class T, class ... Ts>
	struct _Concept<false, T, Ts...>
	{
		static constexpr bool value() { return false; };
	};

	template <class T>
	struct _Concept<true, T>
	{
		static constexpr bool value() { return true; }
	};

	template <bool B, class T, class Ty, class ... Ts>
	struct _Concept<B, T, Ty, Ts...> : _Concept<IsBaseOf<T, Ty>::value(), T, Ts...>
	{

	};

	template <class T, class ... Ts>
	using Concept = _Concept<true, T, Ts...>;
}