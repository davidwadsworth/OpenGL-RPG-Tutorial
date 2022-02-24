#pragma once
#include "component.h"
#include "rect.h"

/*
Rect storing coordinates with respect to the origin

@author David Wadsworth
*/

namespace Component
{
	struct Transform : public IComponent, public Rect
	{
		Transform()
			: Rect{}
		{}

		Transform(const Rect& rect)
			: Rect(rect)
		{}

		Transform(GLfloat x, GLfloat y, GLfloat length)
			: Transform(Rect{ x, y, length, length })
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


