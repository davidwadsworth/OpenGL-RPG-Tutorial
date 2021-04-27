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
				Component::Col& collider_;
				Component::ColliderVector& col_list_;
			public:
				CheckCollisionAABB(Component::Movement& movement, Component::Col& collider, Component::ColliderVector& col_list)
					: movement_(movement), collider_(collider), col_list_(col_list)
				{}

				void execute() override
				{
					for (auto col : col_list_)
					{
						if (col->collide(collider_))
						{
							dynamic_cast<Phys*>(col)->resolve(collider_, movement_);
							break;
						}
					}
				}
			};
		}
	}
}