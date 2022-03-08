#pragma once
#include "component_rect_gjk.h"
#include "component_rect_gjk_physics.h"
#include <glm/glm.hpp>
constexpr auto MAX_SIMPLEX = 3;

/*
Calculate collisions by finding a triangle which contains the origin point (0, 0)
between the Minkowski Difference of two colliders

Minkowski Difference:
	plotting all possible combinations of adding points of A with points of (-B).
Then finding the farthest edge points that create a convex polygon (Convex hull)

void collide(Component::Collider::IGJK& col):
	For this particular implementation of GJK we subvert creating the full minkowski difference by instead
using best guess points from A - the best guess points from B and if they are close to forming a triangle
over the origin repeating the guesses until out of possible guess points or enclosing the origin (0, 0) with a triangle

glm::vec2[3] simplex_:
	array of best guess B - A points;

@author David Wadsworth
*/

namespace GJK
{
	bool collide(Component::Rectangle::IGJK& col_a, Component::Rectangle::IGJK& col_b);
	bool collide(glm::vec2 point, Component::Rectangle::IGJK& col_b);
	
	namespace AABB
	{
		bool collide(Component::Rectangle::IGJK& col_a, Component::Rectang& col_b);
	}
}