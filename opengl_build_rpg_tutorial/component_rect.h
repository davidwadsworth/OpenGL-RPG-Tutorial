#pragma once
#include "component.h"
#include "rect.h"

/*
Component implementation for rect data structure

@author David Wadsworth
*/

namespace Component
{
	class Rectang : public Rect, public IComponent
	{
	public:
		Rectang(const Rect& rect)
			: Rect(rect)
		{}
		Rectang(float x, float y, float w, float h)
			: Rect(x,y,w,h)
		{}

		Rectang()
			: Rect()
		{}
	};
#define Transform Rectang
#define Src Rectang
}