#pragma once
#include "component_system.h"
#include "component_collider.h"
#include "component_vector.h"
#include "component_collider_gjk.h"
#include "physics.h"
#include "component_quadtree.h"

/*
System used for checking for and resolving collisions, in general space around the specified object

@author David Wadsworth
*/

namespace Component {
	namespace System {
		namespace Update
		{
			class CheckCollision : public Component::ISystem
			{
				Component::Collider::IGJK& col_;
				Component::QuadTree& c_quad_tree_;
			public:
				CheckCollision(Component::Collider::IGJK& col, Component::QuadTree& c_quad_tree)
					: col_(col), c_quad_tree_(c_quad_tree)
				{}

				void execute() override
				{
					auto retrieved_rect = c_quad_tree_.retrieve(col_);

					for (auto rect : retrieved_rect)
					{
						auto c = (Component::Collider::IGJK*)rect;
						if (c->collide(col_))
						{
							((IPhysics*)c)->resolve(col_);
							break;
						}
					}
				}
			};
		}
	}
}