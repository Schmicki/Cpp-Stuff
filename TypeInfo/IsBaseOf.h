#pragma once

namespace sge
{
	template <class A> static constexpr bool isBaseOf(void*) { return false; }
	template <class A> static constexpr bool isBaseOf(A*) { return true; }

	template <class A, class B> struct IsBaseOf { static constexpr bool value() { return isBaseOf<A>((B*)0); } };
}