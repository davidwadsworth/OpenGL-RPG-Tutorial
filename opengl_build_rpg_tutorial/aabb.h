#pragma once
#include "component_rect.h"

/*
Axis-Aligned Bounding Box collision detection between two rectangles and a point and a rectangle

"One of the simpler forms of collision detection is between two rectangles that are axis aligned — meaning no rotation. 
The algorithm works by ensuring there is no gap between any of the 4 sides of the rectangles. Any gap means a collision does not exist."

- MDN contributers
https://developer.mozilla.org/en-US/docs/Games/Techniques/2D_collision_detection

@author David Wadsworth
*/

namespace AABB
{
	bool collide(Component::Rectang& rect_a, Component::Rectang& rect_b);
	bool collide(glm::vec2 point, Component::Rectang& rect_b);
}