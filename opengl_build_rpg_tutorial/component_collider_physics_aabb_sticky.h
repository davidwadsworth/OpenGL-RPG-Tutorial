#pragma once
#include "physics.h"
#include "component_collider_aabb.h"
#include <glm\geometric.hpp>

/*
Physics implementation of AABB collider, which resolves collisions by resetting back to the previous position

@author David Wadsworth
*/

namespace Component {
	namespace Collider {
		namespace Physics {
			namespace AABB
			{
				class Sticky : public Component::Collider::AABB, public IPhysics
				{
					glm::vec2 previous_{};
				public:
					using Component::Collider::AABB::AABB;

					bool collide(Component::Collider::AABB& aabb) override
					{
						if (Component::Collider::AABB::collide(aabb))
							return true;

						previous_.x = aabb.x;
						previous_.y = aabb.y;
						return false;
					}

					void resolve(Component::ICollider& col) override
					{
						col.x = previous_.x;
						col.y = previous_.y;
					}
				};
			}
		}
	}
}