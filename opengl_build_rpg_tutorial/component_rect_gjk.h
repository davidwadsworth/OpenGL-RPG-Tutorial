#pragma once
#include "component_rect_action.h"
#include <array>

/*
glm::vec2 support(glm::vec2 direction):
	calculate the closest edge point of the collider within the normalized direction provided with respect to the center of the collider

	normalized: x^2 + y+2 = 1; 

eg. 
	vec2(0, 1) = 0^2 + 1^2 = 1. This vec2 is normalized.

eg. 
	support implementation for a rectangle with four points (a, b, c, d)

	support( vec2(0,1) ) -> return b-a;

	y, x-->
	|
	v

	a)             b)
	----------------
	|       ^      |
	|       |      |
	|       .      |
	|              |
	|              |
	----------------
	c)             d)
*/

namespace Component {
	namespace Rectangle
	{
		class IGJK : public Component::Rectang
		{
		public:
			using Component::Rectang::Rectang;
			virtual glm::vec2 get_center() = 0;
			virtual glm::vec2 support(glm::vec2 direction) = 0;
		};
	}
}

