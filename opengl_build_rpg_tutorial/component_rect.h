#pragma once
#include "component.h"
#include "rect.h"

namespace Component
{
	class Rectangle : public Rect, public IComponent
	{
	public:
		Rectangle()
			: Rect()
		{}

		Rectangle(Rect rect)
			: Rect(rect)
		{}

		Rectangle(GLfloat x, GLfloat y, GLfloat length)
			: Rectangle(Rect{ x, y, length, length })
		{}

		void set(float x, float y, float w, float h)
		{
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}
	};
#define Transform Rectangle
#define Src Rectangle
}