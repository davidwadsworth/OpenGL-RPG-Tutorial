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
			template <typename T>
			class CheckCollision : public Component::ISystem
			{
				T& col_;
				Component::QuadTree& c_quad_tree_;
			public:
				CheckCollision(T& col, Component::QuadTree& c_quad_tree)
					: col_(col), c_quad_tree_(c_quad_tree)
				{}

				void execute() override
				{
					auto retrieved_col = c_quad_tree_.retrieve(col_);

					for (auto c : dynamic_cast<T*>(retrieved_col_))
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