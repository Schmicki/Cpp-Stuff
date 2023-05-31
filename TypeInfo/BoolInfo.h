#pragma once

namespace sge
{
	template <class T> struct IsBool { static constexpr bool value() { return false; } };
	template <> struct IsBool<bool> { static constexpr bool value() { return true; } };

	template <class T>
	constexpr bool alwaysFalse = false;
}