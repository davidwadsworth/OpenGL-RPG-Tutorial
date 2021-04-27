#pragma once
#include "component_collider_physics.h"
#include "component_collider_aabb.h"
#include <glm\geometric.hpp>

/*
Physics implementation of AABB collider, which resolves collisions by resetting back to the previous position

@author David Wadsworth
*/

namespace Component {
	namespace Collider {
		namespace Physics
		{
			class AABBSticky : public Component::Collider::AABB, public Phys
			{
				glm::vec2 previous_{};
			public:
				using Component::Collider::AABB::AABB;

				bool collide(Component::Col& col) override
				{
					if (Component::Collider::AABB::collide(col))
						return true;

					previous_.x = col.transform.x;
					previous_.y = col.transform.y;
					return false;
				}

				void resolve(Component::Col& col, Component::Movement& movement) override
				{
					col.transform.x = previous_.x;
					col.transform.y = previous_.y;
				}
			};
		}
	}
}