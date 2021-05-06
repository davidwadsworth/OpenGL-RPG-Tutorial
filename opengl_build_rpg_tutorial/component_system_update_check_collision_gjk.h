#pragma once
#include "component_movement.h"
#include "component_collider.h"
#include "component_vector.h"
#include "component_collider_physics.h"

namespace Component {
	namespace System {
		namespace Update
		{
			class CheckCollisionGJK : public Component::Sys
			{
				Component::Movement& movement_;
				Component::Col& col_;
				Component::ColliderVector& col_list_;
			public:
				CheckCollisionGJK(Component::Movement& movement, Component::Col& col, Component::ColliderVector& col_list)
					: movement_(movement), col_(col), col_list_(col_list)
				{}

				void execute() override
				{
					for (auto c : col_list_)
					{
						if (c->collide(col_))
						{
							dynamic_cast<Phys*>(c)->resolve(col_, movement_);
							break;
						}
					}
				}
			};
		}
	}
}