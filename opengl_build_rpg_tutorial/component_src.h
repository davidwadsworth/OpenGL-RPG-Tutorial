#pragma once
#include "component.h"
#include "rect.h"
/*
Rect storing image coordinates

@author David Wadsworth
*/

namespace Component
{
	struct Src : public IComponent, public Rect {
		Src() = default;
		Src(const Rect& rect)
			: Rect(rect)
		{}
	};
}