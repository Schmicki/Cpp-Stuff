#pragma once

namespace sge
{
	template<typename A, typename B> constexpr bool typesEqual(const A&, const B&) { return false; }
	template<typename A> constexpr bool typesEqual(const A&, const A&) { return true; }

	template<typename A, typename B> struct TypesEqual { static constexpr bool value() { return false; } };
	template<typename A> struct TypesEqual<A, A> { static constexpr bool value() { return true; } };

#define TYPES_EQUAL(a, b) TypesEqual<a, b>::value()
}