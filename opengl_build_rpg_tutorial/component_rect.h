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

		
	};
#define Transform Rectangle
#define Src Rectangle
}