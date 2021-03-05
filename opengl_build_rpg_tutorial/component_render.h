#pragma once
#include "component.h"
#include <glad/glad.h>

/*
Stores all components used to render a rect on the window
@author David Wadsworth
*/

struct Rect
{
	GLfloat x, y, w, h;
};

namespace Component
{
	struct Render : Comp
	{
		Render(Rect src, Rect dest)
			: src(src), dest(dest)
		{}

		Rect src, dest;
	};
}

