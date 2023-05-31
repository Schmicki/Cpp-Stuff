#pragma once

namespace sge
{
	template<typename T> struct IsLReference { static constexpr bool value() { return false; } };
	template<typename T> struct IsLReference<T&> { static constexpr bool value() { return true; } };

#define IS_L_REF(type) IsLReference<type>::value()

	template<typename T> struct IsRReference { static constexpr bool value() { return false; } };
	template<typename T> struct IsRReference<T&&> { static constexpr bool value() { return true; } };

#define IS_R_REF(type) IsRReference<type>::value()

	template<typename T> struct RemoveReference { using Type = T; };
	template<typename T> struct RemoveReference<T&> { using Type = T; };
	template<typename T> struct RemoveReference<T&&> { using Type = T; };
}