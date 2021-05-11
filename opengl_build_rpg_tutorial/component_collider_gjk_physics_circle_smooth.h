#pragma once
#include "component_collider_gjk_circle.h"
#include "physics.h"
#include "game.h"

namespace Component {
	namespace Collider {
		namespace GJK {
			namespace Physics {
				namespace Circle
				{
					class Smooth : public Component::Collider::GJK::Circle, public IPhysics
					{
					public:
						using Component::Collider::GJK::Circle::Circle;

						void resolve(Component::ICollider& col, Component::Movement& movement) override
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
}