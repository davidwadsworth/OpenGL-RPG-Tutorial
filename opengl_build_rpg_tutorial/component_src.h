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

		void set(float x, float y, float w, float h)
		{
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}
	};
}