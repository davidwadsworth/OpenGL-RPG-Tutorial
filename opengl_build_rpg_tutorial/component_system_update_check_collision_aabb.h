#pragma once
#include "component_system.h"
#include "component_movement.h"
#include "component_collider.h"
#include "component_vector.h"
#include "component_collider_physics.h"

/*
System used for checking for and resolving collisions, in general space around the specified object

@author David Wadsworth
*/

namespace Component {
	namespace System {
		namespace Update
		{
			class CheckCollisionAABB : public Component::Sys
			{
				Component::Movement& movement_;
				Component::Collider::AABB& aabb_;
				Component::AABBVector& aabb_list_;
			public:
				CheckCollisionAABB(Component::Movement& movement, Component::Collider::AABB& aabb, Component::AABBVector& col_list)
					: movement_(movement), aabb_(aabb), aabb_list_(col_list)
				{}

				void execute() override
				{
					for (auto rect : aabb_list_)
					{
						if (rect->collide(aabb_))
						{
							dynamic_cast<Phys*>(rect)->resolve(aabb_, movement_);
							break;
						}
					}
				}
			};
		}
	}
}