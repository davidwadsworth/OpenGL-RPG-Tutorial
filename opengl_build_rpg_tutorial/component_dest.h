#pragma once
#include "component.h"
#include "rect.h"

/*
Rect storing screen coordinates

@author David Wadsworth
*/

namespace Component
{
	struct Dest : public Comp, public Rect {
		Dest() = default;
		Dest(const Rect& rect)
			: Rect(rect)
		{}
	};
}

