#pragma once
#include "component_collider_physics.h"
#include "component_collider_aabb.h"
#include <glm\geometric.hpp>

namespace Component {
	namespace Collider {
		namespace Physics
		{
			class AABBPrevious : public Component::Collider::AABB, public Phys
			{
				glm::vec2 previous_{};
			public:
				using Component::Collider::AABB::AABB;

				bool collide(Component::Col& col) override
				{
					previous_.x = this->transform.x;
					previous_.y = this->transform.y;
					return Component::Collider::AABB::collide(col);
				}

				void resolve(Component::Col& col, Component::Movement& movement) override
				{
					this->transform.x = previous_.x;
					this->transform.y = previous_.y;
				}
			};
		}
	}
}