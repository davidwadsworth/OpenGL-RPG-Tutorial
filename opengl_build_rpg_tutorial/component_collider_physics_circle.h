#pragma once
#include "component_collider_circle.h"
#include "component_collider_physics.h"
#include "game.h"

namespace Component {
	namespace Collider {
		namespace Physics {
			namespace Circle
			{
				class Smooth : public Component::Collider::Circle, public Phys
				{
				public:
					using Component::Collider::Circle::Circle;

					void resolve(Component::Col& col, Component::Movement& movement) override
					{
						auto prev_direction = glm::normalize(get_center() - col.get_center());

						col.transform.x += prev_direction.x * movement.speed * Game::delta_time;
						col.transform.y += prev_direction.y * movement.speed * Game::delta_time;
					}
				};
			}
		}
	}
}