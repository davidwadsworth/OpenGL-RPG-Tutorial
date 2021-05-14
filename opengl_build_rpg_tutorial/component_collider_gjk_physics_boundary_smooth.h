#pragma once
#include "component_collider_gjk_boundary.h"
#include "physics.h"

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

						void resolve(Component::ICollider& col) override
						{
							auto line = glm::normalize(vertices_[1] - vertices_[0]);
							auto bisector = glm::vec2(line.y, -line.x);

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