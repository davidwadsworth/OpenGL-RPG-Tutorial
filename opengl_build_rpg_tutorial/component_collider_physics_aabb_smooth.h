#pragma once
#include "physics.h"
#include "component_collider_aabb.h"
#include <glm\geometric.hpp>
#include "game.h"

/*
Calculates the shortest distance between rect side and the difference in centers of the colliders
displaced by the perpendicular bisector of the edge.
 
@author David Wadsworth
*/

namespace Component {
	namespace Collider {
		namespace Physics {
			namespace AABB
			{
				class Smooth : public Component::Collider::AABB, public IPhysics
				{
				public:
					using Component::Collider::AABB::AABB;

					/* eg.
											*col_center (displacement * {1,0})

								  e1
						 p1	_______________ p2
						   |               |
						   |               |
						   |               |
						e4 |               | e2
						   |               |
						   |               |
						 p4 ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾ p3
								   e3
					*/

					void resolve(Component::ICollider &col) override
					{
						auto aabb_1 = static_cast<AABB*>(&col);
						auto aabb_2 = this;

						auto col_a_center = this->get_center() - col.get_center();

						// calculate point set up
						glm::vec2 p1{ aabb_2->x, aabb_2->y };
						glm::vec2 p2{ aabb_2->x + aabb_2->w, aabb_2->y };
						glm::vec2 p3{ aabb_2->x + aabb_2->w, aabb_2->y + aabb_2->h };
						glm::vec2 p4{ aabb_2->x, aabb_2->y + aabb_2->h };

						// calculate the distance between the center of the collder and the edge
						auto e1_distance = glm::dot(p2, col_a_center) + glm::dot(p1, col_a_center);
						auto e2_distance = glm::dot(p3, col_a_center) + glm::dot(p2, col_a_center);
						auto e3_distance = glm::dot(p4, col_a_center) + glm::dot(p3, col_a_center);
						auto e4_distance = glm::dot(p1, col_a_center) + glm::dot(p4, col_a_center);

						auto min_distance = e1_distance;
						auto bisector = glm::vec2(0.0f, -1.0f);
						auto displacement = aabb_1->y + aabb_1->h - aabb_2->y;

						if (e2_distance < min_distance)
						{
							min_distance = e2_distance;
							bisector = glm::vec2(1.0f, 0.0f);
							displacement = (aabb_1->x + aabb_1->w) - aabb_2->x;
						}
						if (e3_distance < min_distance)
						{
							min_distance = e3_distance;
							bisector = glm::vec2(0.0f, 1.0f);
							displacement = aabb_1->y - (aabb_2->y + aabb_2->h);
						}
						if (e4_distance < min_distance)
						{
							min_distance = e4_distance;
							bisector = glm::vec2(-1.0f, 0.0f);
							displacement = aabb_1->x - (aabb_2->x + aabb_2->w);
						}

						aabb_1->x += bisector.x * displacement;
						aabb_1->y += bisector.y * displacement;
					}
				};
			}
		}
	}
}