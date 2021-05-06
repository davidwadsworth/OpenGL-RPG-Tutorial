#pragma once
#include "component_collider.h"
#include <glad/glad.h>
#include <glm/geometric.hpp>


namespace Component {
	namespace Collider
	{
		class Circle : public Component::Col
		{
		protected:	
			GLfloat radius_;
			glm::vec2 center_;

			glm::vec2 support(glm::vec2 direction) override
			{
				return get_center() + radius_ * glm::normalize(direction);
			}

			glm::vec2 get_center()
			{
				return center_ + glm::vec2(transform.x, transform.y);
			}

		public:
			Circle(Component::Transform& transform, GLfloat radius, glm::vec2 center)
				: Component::Col(transform), radius_(radius), center_(center)
			{}
		};
	}
}