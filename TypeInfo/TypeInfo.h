#pragma once

#include "Reference.h"
#include "Pointer.h"
#include "TypesEqual.h"
#include "IsConvertibleTo.h"
#include "IsBaseOf.h"
#include "Concept.h"
#include "BoolInfo.h"
#include "IntInfo.h"
#include "EnableIf.h"

namespace sge
{
	template <typename T>
	constexpr typename RemoveReference<T>::Type &&mov(T &&a)
	{
		using MoveType = typename RemoveReference<T>::Type;
		return static_cast<MoveType &&>(a);
	}
}

#define sizeOf(x) sizeof(x)
#define alignOf(x) alignof(x)