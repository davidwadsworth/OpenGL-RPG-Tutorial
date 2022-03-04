#pragma once
#include "action.h"
#include "component_collider_gjk_physics_boundary_smooth.h"
#include "component_collider_gjk_physics_circle_smooth.h"
#include "component_collider_gjk_physics_polygon_smooth.h"

/*
I thought long and hard for a solution to retrieving actions from quadtrees, somehow landed on this. Not pretty but gets the job done.
smooth actionable colliders hold a personal record for me as the "most polymorphised class" with 6 different classes inherited.

@author David Wadsworth
*/

namespace Component {
	namespace Collider {
		namespace GJK {
			namespace Action {
				namespace Physics {
					namespace Smooth
					{
						// actionable and smooth collidable
						class Boundary : public Component::Collider::GJK::Physics::Boundary::Smooth, public Action {
						public:
							using Component::Collider::GJK::Physics::Boundary::Smooth::Smooth;
						};
						class Circle : public Component::Collider::GJK::Physics::Circle::Smooth, public Action {
						public:
							using Component::Collider::GJK::Physics::Circle::Smooth::Smooth;
						};
						class Polygon : public Component::Collider::GJK::Physics::Polygon::Smooth, public Action {
						public:
							using Component::Collider::GJK::Physics::Polygon::Smooth::Smooth;
						};
					}
				}
			}
		}
	}
}