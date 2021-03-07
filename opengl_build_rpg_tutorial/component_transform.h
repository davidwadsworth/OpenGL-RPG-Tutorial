#pragma once
#include "component.h"
#include "rect.h"

/*
Locally bound transformational information. Not where the entity is located but rather located relative to an invisible plane
@author David Wadsworth
*/

namespace Component
{
	struct Transform : public Comp
	{
		Transform(Rect rect, GLfloat scale)
			: rect(rect), scale(scale)
		{}

		Transform(Rect rect)
			: Transform(rect, 1.0f)
		{}

		Transform(GLfloat x, GLfloat y, GLfloat length)
			: Transform(Rect{ x, y, length, length }, 1.0f)
		{}

		Rect rect;
		GLfloat scale;
	};
}


