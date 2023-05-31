#pragma once

namespace sge
{
	template <class A> static constexpr bool isConvertibleTo(...) { return false; }
	template <class A> static constexpr bool isConvertibleTo(A) { return true; }

	template <class A, class B> struct IsConvertibleTo { static constexpr bool value() { return isConvertibleTo<A>(B()); } };
}