#pragma once
#include "component_collider.h"
#include <glm/glm.hpp>

constexpr GLuint MAX_BOUNDARY = 2;

namespace Component {
	namespace Collider
	{
		class Boundary : public Component::Col
		{
		protected:
			std::array<glm::vec2, MAX_BOUNDARY> vertices_;

			glm::vec2 support(glm::vec2 direction) override
			{
				auto p1 = vertices_[0] + get_center();
				auto p2 = vertices_[1] + get_center();

				auto p1_distance = glm::dot(p1, direction); 
				auto p2_distance = glm::dot(p2, direction);

				if (p1_distance > p2_distance)
					return p1;
				else
					return p2;
			}

		public:
			Boundary(Component::Transform& transform, std::array<glm::vec2, MAX_BOUNDARY> vertices)
				: Col(transform), vertices_(vertices)
			{}

			glm::vec2 get_center()
			{
				return glm::vec2(this->transform.x, this->transform.y) + (vertices_[0] + vertices_[1]) / 2.0f;
			}
		};

	}
}