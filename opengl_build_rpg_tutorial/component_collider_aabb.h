#pragma once
#include "component_transform.h"
#include "component_collider.h"

namespace Component {
	namespace Collider
	{
		class AABB : public Component::Transform, public Col
		{
		public:
			using Component::Transform::Transform;

			bool collide(Col& col, Component::Transform& pos_a, Component::Transform& pos_b) override
			{
				auto aabb_2 = static_cast<AABB*>(&col);
				Rect rect_b{ aabb_2->x + pos_b.x, aabb_2->y + pos_b.y, aabb_2->w * aabb_2->sc, aabb_2->h * aabb_2->sc };
				Rect rect_a{ this->x + pos_a.x, this->y + pos_a.y, this->w * this->sc, this->h * this->sc };



				return (rect_a.x < rect_b.x + rect_b.w && this->x + this->w * this->sc > rectB->x) &&
					(this->y < rectB->y + rectB->h * rectB->sc && this->y + this->h * this->sc > rectB->y);
			}

		};
	}
}

