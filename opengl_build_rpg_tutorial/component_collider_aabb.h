#pragma once
#include "rect.h"
#include "component_collider.h"
#include <glm/glm.hpp>

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

				return collide(rect_a, rect_b);
			}

			static bool collide(Rect& rect_a, Rect& rect_b)
			{
				return (rect_a.x < rect_b.x + rect_b.w && rect_a.x + rect_a.w > rect_b.x) &&
					(rect_a.y < rect_b.y + rect_b.h && rect_a.y + rect_a.h > rect_b.y);
			}

			glm::vec2 get_center()
			{
				return glm::vec2(this->x + this->w / 2.0f,
					this->y + this->h / 2.0f);
			}

		};
	}
}

