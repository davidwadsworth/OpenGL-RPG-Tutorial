#pragma once
#include "component_collider_physics.h"
#include "component_collider_aabb.h"
#include <glm\geometric.hpp>

namespace Component {
	namespace Collider {
		namespace Physics
		{
			class AABBSmooth : public Component::Collider::AABB, public Phys
			{
			public:
				using Component::Collider::AABB::AABB;

				void resolve(Component::Col& col, Component::Movement& movement) override
				{
					auto pos_a = glm::vec2{ this->transform.x, this->transform.y };
					auto pos_b = glm::vec2{ col.transform.x, col.transform.y };

					auto aabb_2 = static_cast<AABB*>(&col);

					auto col_a_center = this->get_center();

					Rect rect_a{ this->x + pos_a.x, this->y + pos_a.y, this->w * this->sc, this->h * this->sc };
					Rect rect_b{ aabb_2->x + pos_b.x, aabb_2->y + pos_b.y, aabb_2->w * aabb_2->sc, aabb_2->h * aabb_2->sc };

					glm::vec2 p1{ rect_b.x, rect_b.y };
					glm::vec2 p2{ rect_b.x + rect_b.w, rect_b.y };
					glm::vec2 p3{ rect_b.x + rect_b.w, rect_b.y + rect_b.h };
					glm::vec2 p4{ rect_b.x, rect_b.y };

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

					this->transform.x += bisector.x * movement.speed;
					this->transform.y += bisector.y * movement.speed;
				}
			};
		}
	}
}