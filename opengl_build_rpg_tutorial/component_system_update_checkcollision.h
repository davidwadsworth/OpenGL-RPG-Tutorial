#pragma once
#include "component_system.h"
#include "component_vector.h"
#include "component_rect_gjk_collider.h"
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
				Component::Rectangle::ColliderGJK& col_;
				Component::PhysicsGJKQTree& c_quad_tree_;
			public:
				CheckCollision(Component::Rectangle::ColliderGJK& col, Component::PhysicsGJKQTree& c_quad_tree)
					: col_(col), c_quad_tree_(c_quad_tree)
				{}

				void execute() override
				{
					auto retrieved_rect = c_quad_tree_.retrieve(col_);

					for (auto c : retrieved_rect)
					{
						if (col_.collide(*c))
						{
							c->resolve(col_);
							break;
						}
					}
				}
			};
		}
	}
}