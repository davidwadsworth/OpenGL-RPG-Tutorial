#pragma once
#include "component.h"
#include "rect.h"

namespace Component
{
	class Rectangle : public Rect, public IComponent
	{
	public:
		using Rect::Rect;
	};
#define Transform Rectangle
#define Src Rectangle
}