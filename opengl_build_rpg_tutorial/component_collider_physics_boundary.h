#pragma once
#include "component_collider_boundary.h"
#include "component_collider_physics.h"

namespace Component {
	namespace Collider {
		namespace Physics
		{
			class AABBSticky : public Component::Collider::Boundary, public Phys
			{
				glm::vec2 previous_{};
			public:
				using Component::Collider::Boundary::Boundary;

				void resolve(Component::Col& col, Component::Movement& movement) override
				{
					col.transform.x = previous_.x;
					col.transform.y = previous_.y;
				}
			};
		}
	}
}