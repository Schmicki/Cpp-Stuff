#pragma once

namespace sge
{
	template <bool B> struct EnableIf {};
	template <> struct EnableIf<true> { using Type = void; };
}