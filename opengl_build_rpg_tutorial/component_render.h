#pragma once
#include "component.h"
#include "rect.h"

/*
Stores all components used to render a rect on the window
@author David Wadsworth
*/

namespace Component
{
	struct Src : public Comp, public Rect {
		Src() = default;
		Src(const Rect& rect)
			: Rect(rect)
		{}
	};

	struct Dest : public Comp, public Rect {
		Dest() = default;
		Dest(const Rect& rect)
			: Rect(rect)
		{}
	};
}

