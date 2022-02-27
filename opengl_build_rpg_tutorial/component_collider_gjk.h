#pragma once
#include "component_collider.h"
#include <array>

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
	array of best guess A - B points;

glm::vec2 support(glm::vec2 direction): 
	calculate the closest edge point of the collider within the direction provided

@author David Wadsworth
*/
namespace Component {
	namespace Collider
	{
		class IGJK : public Component::ICollider
		{
		protected:
			std::array<glm::vec2, MAX_SIMPLEX> simplex_;
		public:
			using Component::ICollider::ICollider;
			virtual glm::vec2 support(glm::vec2 direction) = 0;
			virtual bool collide(Component::Collider::IGJK& col);
		};
	}
}

