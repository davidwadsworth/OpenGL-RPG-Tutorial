#pragma once
#include "component_collider_physics.h"
#include "component_collider_aabb.h"
#include <glm\geometric.hpp>

/*
Calculates the shortest distance between rect side and the difference in centers of the colliders
displaces the perpendicular bisector of the edge multiplied by object speed.
 
@author David Wadsworth
*/

namespace Component {
	namespace Collider {
		namespace Physics
		{
			class AABBSmooth : public Component::Collider::AABB, public Phys
			{
			public:
				using Component::Collider::AABB::AABB;
				
				/* eg.
				                        *col_center (displaces speed * {1,0})

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

				void resolve(Component::Col& col, Component::Movement& movement) override
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

					if (e2_distance < min_distance)
					{
						min_distance = e2_distance;
						bisector = glm::vec2(1.0f, 0.0f);
					}
					if (e3_distance < min_distance)
					{
						min_distance = e3_distance;
						bisector = glm::vec2(0.0f, 1.0f);
					}
					if (e4_distance < min_distance)
					{
						min_distance = e4_distance;
						bisector = glm::vec2(-1.0f, 0.0f);
					}

					aabb_1->transform.x += bisector.x * movement.speed * Game::delta_time;
					aabb_1->transform.y += bisector.y * movement.speed * Game::delta_time;
				}
			};
		}
	}
}