#pragma once
#include "rect.h"
#include "component_collider.h"

/*
collider information for checking aabb to abbb collisions

@author David Wadsworth
*/

namespace Component {
	namespace Collider
	{
		class AABB : public Rect, public Col
		{
		public:
			AABB(Component::Transform& transform, GLfloat x, GLfloat y, GLfloat w, GLfloat h)
				: Col(transform), Rect{ x, y, w, h }
			{}

			AABB(Component::Transform& transform, GLfloat x, GLfloat y, GLfloat s)
				: Col(transform), Rect{ x, y, s, s }
			{}

			AABB(Component::Transform& transform, GLfloat s)
				: Col(transform), Rect{ 0.0f, 0.0f, s, s }
			{}

			virtual bool collide(AABB& other)
			{
				auto pos_a = glm::vec2{ this->transform.x, this->transform.y };
				auto pos_b = glm::vec2{ other.transform.x, other.transform.y };

				Rect rect_a{ this->x + pos_a.x, this->y + pos_a.y, this->w, this->h};
				Rect rect_b{ other.x + pos_b.x, other.y + pos_b.y, other.w, other.h};

				return (rect_a.x < rect_b.x + rect_b.w && rect_a.x + rect_a.w > rect_b.x) &&
					(rect_a.y < rect_b.y + rect_b.h && rect_a.y + rect_a.h > rect_b.y);
			}

			glm::vec2 get_center()
			{
				return glm::vec2(this->transform.x + this->x + this->w / 2.0f,
					this->transform.y + this->y + this->h / 2.0f);
			}

		};
	}
}

