#pragma once
#include "component.h"
#include "rect.h"

/*
Rect storing coordinates with respect to the origin

@author David Wadsworth
*/

namespace Component
{
	struct Transform : public Comp, public Rect
	{
		Transform(const Rect& rect, GLfloat sc)
			: Rect(rect), sc(sc)
		{}

		Transform(const Rect& rect)
			: Transform(rect, 1.0f)
		{}

		Transform(GLfloat x, GLfloat y, GLfloat length)
			: Transform(Rect{ x, y, length, length }, 1.0f)
		{}

		Transform(GLfloat x, GLfloat y, GLfloat length, GLfloat sc)
			: Transform(Rect{ x, y, length, length }, sc)
		{}

		GLfloat sc;
	};
}


