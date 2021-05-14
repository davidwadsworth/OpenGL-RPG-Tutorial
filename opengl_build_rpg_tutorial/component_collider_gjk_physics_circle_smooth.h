#pragma once
#include "component_collider_gjk_circle.h"
#include "physics.h"

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

						void resolve(Component::ICollider& col) override
						{
							auto bisector = glm::normalize(get_center() - col.get_center());

							auto gjk = static_cast<Component::Collider::IGJK*>(&col);

							auto sup = gjk->support(-bisector) - this->support(bisector);
							auto piercing_vec = glm::dot(sup, -bisector) * bisector;

							col.transform.x += piercing_vec.x;
							col.transform.y += piercing_vec.y;
						}
					};
				}
			}
		}
	}
}