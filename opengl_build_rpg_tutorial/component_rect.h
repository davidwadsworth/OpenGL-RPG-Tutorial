#pragma once
#include "component.h"
#include "rect.h"

namespace Component
{
	class Rectangle : public Rect, public IComponent
	{
	public:
		Rectangle(const Rect& rect)
			: Rect(rect)
		{}
		Rectangle(float x, float y, float w, float h)
			: Rect(x,y,w,h)
		{}

		Rectangle()
			: Rect()
		{}
	};
#define Transform Rectangle
#define Src Rectangle
}