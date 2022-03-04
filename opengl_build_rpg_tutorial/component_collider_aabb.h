#pragma once
#include "rect.h"
#include "component_collider.h"
#include <glm/glm.hpp>
#include "aabb.h"
/*
collider information for checking aabb to abbb collisions

@author David Wadsworth
*/

namespace Component {
	namespace Collider
	{
		class AABB : public ICollider
		{
		public:

			virtual bool collide(Component::Collider::AABB& col)
			{
				Rect rect_a( this->x , this->y, this->w, this->h);
				Rect rect_b( col.x, col.y, col.w, col.h);

				return true;
			}

			glm::vec2 get_center()
			{
				return glm::vec2(this->x + this->w / 2.0f,
					this->y + this->h / 2.0f);
			}

		};
	}
}

