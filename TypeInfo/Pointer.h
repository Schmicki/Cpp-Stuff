#pragma once

namespace sge
{
	template<class T> struct IsPointer { static constexpr bool value() { return false; } };
	template<class T> struct IsPointer<T*> { static constexpr bool value() { return true; } };

	template<class T> struct IsPointer2D { static constexpr bool value() { return false; } };
	template<class T> struct IsPointer2D<T**> { static constexpr bool value() { return true; } };

	template<class T> struct RemovePointer1D { using Type = T; };
	template<class T> struct RemovePointer1D<T*> { using Type = T; };

	template<class T> struct RemovePointer { using Type = T; };
	template<class T> struct RemovePointer<T*> { using Type = T; };
	template<class T> struct RemovePointer<T**> { using Type = T; };
}