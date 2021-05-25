#pragma once
#include "component_collider_gjk_boundary.h"
#include "physics.h"

/*
Smoothly traverse over a boundary using another GJK collider

@author David Wadsworth
*/
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
							auto perp_bisector = glm::vec2(line.y, -line.x);

							auto gjk = static_cast<Component::Collider::IGJK*>(&col);

							auto sup = gjk->support(-perp_bisector) - this->support(perp_bisector);
							auto piercing_vec = glm::dot(sup, -perp_bisector) * perp_bisector;

							col.transform.x += piercing_vec.x;
							col.transform.y += piercing_vec.y;
						}
					};
				}
			}
		}
	}
}