#pragma once
#include "component_collider_gjk_boundary.h"
#include "physics.h"
#include "game.h"

namespace Component {
	namespace Collider {
		namespace GJK {
			namespace Physics {
				namespace Boundary
				{
					class Smooth : public Component::Collider::GJK::Boundary, public IPhysics
					{
					public:
						using Component::Collider::GJK::Boundary::Boundary;

						void resolve(Component::ICollider& col, Component::Movement& movement) override
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
}