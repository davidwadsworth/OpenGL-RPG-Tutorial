#pragma once
#include "physics.h"
#include "component_collider_aabb.h"

namespace Component {
	namespace Collider {
		namespace Physics
		{
			class AABB : public Component::Collider::AABB, public Phys
			{
			public:
				using Component::Collider::AABB::AABB;

				glm::vec2 perpendicular_bisector(Component::Col& col) override
				{
					auto rectB = static_cast<Component::Collider::AABB*>(&col);

					if (this->x < rectB->x + rectB->w * rectB->sc)
						return glm::vec2(1.0f, 0.0f);
				}
			};
		}
	}
}