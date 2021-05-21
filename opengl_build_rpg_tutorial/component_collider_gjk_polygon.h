#pragma once
#include "component_collider_gjk.h"
#include <glm/glm.hpp>
#include <vector>
#include <glad/glad.h>


namespace Component {
	namespace Collider {
		namespace GJK
		{
			class Polygon : public Component::Collider::IGJK
			{
				glm::vec2 offset_;
			protected:
				std::vector<glm::vec2> vertices_;
			public:
				Polygon(Component::Transform& transform, std::vector<glm::vec2> vertices)
					: IGJK(transform), vertices_(vertices)
				{
					glm::vec2 temp_center = glm::vec2();
					for (auto &v : vertices_)
						temp_center += v;

					offset_ = temp_center / static_cast<float>(vertices_.size());
					for (auto &v : vertices_)
						v -= offset_;
				}

				glm::vec2 get_center() override
				{
					return glm::vec2(transform.x, transform.y) + offset_;
				}
				
				glm::vec2 support(glm::vec2 direction) override
				{
					auto furthest_distance = std::numeric_limits<float>::lowest();
					auto furthest_vertex = glm::vec2();

					for (auto& v : vertices_)
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
			};
		}
	}
}