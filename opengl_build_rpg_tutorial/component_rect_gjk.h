#pragma once
#include "component_rect_action.h"
#include <array>


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
	array of best guess A - B points;

glm::vec2 support(glm::vec2 direction): 
	calculate the closest edge point of the collider within the direction provided

@author David Wadsworth
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

