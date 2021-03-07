#pragma once
#include "component.h"
#include "rect.h"

/*
Stores all components used to render a rect on the window
@author David Wadsworth
*/

namespace Component
{
	struct Render : public Comp
	{
		Render(Rect src, Rect dest)
			: src(src), dest(dest)
		{}

		Render(Rect src)
			: src(src), dest{}
		{}

		Rect src, // src postion for image
			dest; // actual position on the screen
	};
}

