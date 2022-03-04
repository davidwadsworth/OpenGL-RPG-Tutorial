#pragma once
#include "component_collider_gjk.h"
#include <glm/glm.hpp>
#include <vector>
#include <glad/glad.h>

/*
GJK implementation for at least three pointed non-concave polygons

@author David Wadsworth
*/
namespace Component {
	namespace Collider {
		namespace GJK
		{
			class Polygon : public Component::Collider::IGJK
			{
			protected:
				std::vector<glm::vec2> vertices_;
			private:
				glm::vec2 offset_;

				bool is_convex()
				{
					if (vertices_.size() < 4)
						return true;

					auto sign = false;
					auto n = vertices_.size();

					for (auto i = 0; i < n; i++)
					{
						auto dx1 = vertices_[(i + 2) % n].x - vertices_[(i + 1) % n].x;
						auto dy1 = vertices_[(i + 2) % n].y - vertices_[(i + 1) % n].y;
						auto dx2 = vertices_[i].x - vertices_[(i + 1) % n].x;
						auto dy2 = vertices_[i].y - vertices_[(i + 1) % n].y;
						auto zcrossproduct = dx1 * dy2 - dy1 * dx2;

						if (i == 0)
							sign = zcrossproduct > 0;
						else if (sign != (zcrossproduct > 0))
							return false;
					}

					return true;
				}
			public:
				Polygon(const Rect& transform, std::vector<glm::vec2> vertices)
					: IGJK(transform), vertices_(vertices)
				{
					if (vertices.size() < 3)
						Logger::error("Invalid polygon, not enough vertices", Logger::MEDIUM);
						
					if (!is_convex())
						Logger::error("Invalid polygon, concave.", Logger::MEDIUM);

					// calculate the center of the polygon
					glm::vec2 temp_center = glm::vec2();
					for (auto &v : vertices_)
						temp_center += v;

					// offset all vertices by center of the polygon
					offset_ = temp_center / static_cast<float>(vertices_.size());
					for (auto &v : vertices_)
						v -= offset_;
				}

				glm::vec2 get_center() override
				{
					return glm::vec2(this->x, this->y) + offset_;
				}
				
				glm::vec2 support(glm::vec2 direction) override
				{
					auto furthest_distance = std::numeric_limits<float>::lowest();
					auto furthest_vertex = glm::vec2();

					// calculate the furthest vertex along the direction
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