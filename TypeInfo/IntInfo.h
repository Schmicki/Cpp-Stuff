#pragma once

namespace sge
{
	template <class T> struct IsSignedInt { static constexpr bool value() { return false; } };
	template <> struct IsSignedInt<char> { static constexpr bool value() { return true; } };
	template <> struct IsSignedInt<short> { static constexpr bool value() { return true; } };
	template <> struct IsSignedInt<int> { static constexpr bool value() { return true; } };
	template <> struct IsSignedInt<long> { static constexpr bool value() { return true; } };
	template <> struct IsSignedInt<long long> { static constexpr bool value() { return true; } };

	template <class T> struct IsUnsginedInt { static constexpr bool value() { return false; } };
	template <> struct IsUnsginedInt<unsigned char> { static constexpr bool value() { return true; } };
	template <> struct IsUnsginedInt<unsigned short> { static constexpr bool value() { return true; } };
	template <> struct IsUnsginedInt<unsigned int> { static constexpr bool value() { return true; } };
	template <> struct IsUnsginedInt<unsigned long> { static constexpr bool value() { return true; } };
	template <> struct IsUnsginedInt<unsigned long long> { static constexpr bool value() { return true; } };

	template <class T> struct IsInteger { static constexpr bool value() { return false; } };
	template <> struct IsInteger<char> { static constexpr bool value() { return true; } };
	template <> struct IsInteger<short> { static constexpr bool value() { return true; } };
	template <> struct IsInteger<int> { static constexpr bool value() { return true; } };
	template <> struct IsInteger<long> { static constexpr bool value() { return true; } };
	template <> struct IsInteger<long long> { static constexpr bool value() { return true; } };
	template <> struct IsInteger<unsigned char> { static constexpr bool value() { return true; } };
	template <> struct IsInteger<unsigned short> { static constexpr bool value() { return true; } };
	template <> struct IsInteger<unsigned int> { static constexpr bool value() { return true; } };
	template <> struct IsInteger<unsigned long> { static constexpr bool value() { return true; } };
	template <> struct IsInteger<unsigned long long> { static constexpr bool value() { return true; } };
}