#pragma once
#include "component_collider_boundary.h"
#include "component_collider_physics.h"
#include "game.h"

namespace Component {
	namespace Collider {
		namespace Physics {
			namespace Boundary
			{
				class Smooth : public Component::Collider::Boundary, public Phys
				{
				public:
					using Component::Collider::Boundary::Boundary;

					void resolve(Component::Col& col, Component::Movement& movement) override
					{
						auto line = glm::normalize(vertices_[1] - vertices_[0]);
						auto bisector = glm::vec2(line.y, -line.x);

						col.transform.x += bisector.x * movement.speed * Game::delta_time;
						col.transform.y += bisector.y * movement.speed * Game::delta_time;
					}
				};
			}
		}
	}
}