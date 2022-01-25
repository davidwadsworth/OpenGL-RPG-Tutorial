#pragma once
#include "component_system.h"
#include "component_collider.h"
#include "component_vector.h"
#include "component_collider_gjk.h"
#include "physics.h"

/*
System used for checking for and resolving collisions, in general space around the specified object

@author David Wadsworth
*/

namespace Component {
	namespace System {
		namespace Update
		{
			template <typename T>
			class CheckCollision : public Component::ISystem
			{
				T& col_;
				Component::Vector<T*>& col_list_;
			public:
				CheckCollision(T& col, Component::Vector<T*>& col_list)
					: col_(col), col_list_(col_list)
				{}

				void execute() override
				{
					for (auto c : col_list_)
					{
						if (c->collide(col_))
						{
							dynamic_cast<IPhysics*>(c)->resolve(col_);
							break;
						}
					}
				}
			};
#define CheckCollisionAABB CheckCollision<Component::Collider::AABB>
#define CheckCollisionGJK CheckCollision<Component::Collider::IGJK>
		}
	}
}