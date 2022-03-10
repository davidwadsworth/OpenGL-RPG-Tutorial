#pragma once
#include "component.h"
#include <glm/vec2.hpp>

/*
implementation for 2d positions

@author David Wadsworth
*/

namespace Component
{
	class Position : public IComponent, public glm::vec2 {
	public:
		Position(glm::vec2 vec)
			: glm::vec2(vec)
		{}

		Position(float x, float y)
			: glm::vec2(x,y)
		{}
	};
}