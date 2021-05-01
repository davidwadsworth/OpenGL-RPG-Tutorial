#pragma once
#include "component_collider.h"
#include <glm/glm.hpp>
#include <vector>
#include <glad/glad.h>


namespace Component {
	namespace Collider
	{
		class Polygon : public Component::Col
		{
		protected:
			std::vector<glm::vec2> vertices_;

			glm::vec2 get_center() override
			{
				glm::vec2 temp_center = glm::vec2();
				for (auto v : vertices_)
					temp_center += v;
				return temp_center / static_cast<float>(vertices_.size());
			}

			glm::vec2 support(glm::vec2 direction) override
			{
				auto furthest_distance = std::numeric_limits<float>::lowest();
				auto furthest_vertex = glm::vec2();

				for (auto v : vertices_)
				{
					auto distance = glm::dot(v + get_center(), direction);
					if (distance > furthest_distance)
					{
						furthest_distance = distance;
						furthest_vertex = v + get_center();
					}
				}

				return furthest_vertex;
			}
		public:
			Polygon(Component::Transform& transform, std::vector<glm::vec2> vertices)
				: Component::Col(transform), vertices_(vertices)
			{}
		};
	}
}