#pragma once
#include "component_collider_gjk.h"
#include <glad/glad.h>
#include <glm/geometric.hpp>


namespace Component {
	namespace Collider {
		namespace GJK
		{
			class Circle : public Component::Collider::IGJK
			{
				GLfloat radius_;
				glm::vec2 center_;

			public:
				Circle(Component::Transform& transform, GLfloat radius, glm::vec2 center)
					: IGJK(transform), radius_(radius), center_(center)
				{}

				glm::vec2 get_center() override
				{
					return center_ + glm::vec2(transform.x, transform.y);
				}

				glm::vec2 support(glm::vec2 direction) override
				{
					return get_center() + radius_ * glm::normalize(direction);
				}
			};
		}
	}
}