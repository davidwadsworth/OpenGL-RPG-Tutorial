#pragma once
#include "component_system.h"
#include "component_vector.h"
#include "component_rect_gjk.h"
#include "component_quadtree.h"
#include "gjk.h"

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
				Component::Rectangle::IGJK& col_;
				Component::PhysicsActionGJKQTree& c_quad_tree_;
			public:
				CheckCollision(Component::Rectangle::IGJK& col, Component::PhysicsActionGJKQTree& c_quad_tree)
					: col_(col), c_quad_tree_(c_quad_tree)
				{}

				void execute() override
				{
					auto retrieved_rect = c_quad_tree_.retrieve(col_);

					for (auto c : retrieved_rect)
					{
						if (GJK::collide(col_, *c))
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