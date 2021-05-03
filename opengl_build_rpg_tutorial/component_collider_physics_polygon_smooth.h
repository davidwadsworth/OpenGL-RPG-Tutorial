#pragma once
#include "component_collider_physics.h"
#include "component_collider_polygon.h"
#include "game.h"

namespace Component {
	namespace Collider {
		namespace Physics {
			namespace Polygon
			{
				class Smooth : public Component::Collider::Polygon, public Phys
				{
					GLfloat shortest_magnitude_, shortest_distance_;

					void find_closest_distance(float& closest_distance, glm::vec2& closest_vertex, glm::vec2 p1, glm::vec2 p2, glm::vec2 direction, int index)
					{
						auto p = p2 - p1;
						auto p_magnitude = p.x * p.x + p.y * p.y;

						auto q_sqrt = [](auto num)
						{
							auto i = 1.0f;
							for (; i * i < num; ++i);
							return static_cast<GLuint>(i);	
						};

						if (p_magnitude > shortest_magnitude_)
						{
							auto du = shortest_distance_ * glm::normalize(p);

							auto multiplier = q_sqrt(p_magnitude / shortest_magnitude_);

							for (auto i = 1.0f; i < multiplier; ++i)
								find_closest_distance(closest_distance, closest_vertex, p1, p1 + du * i, direction, vertices_.size());
							find_closest_distance(closest_distance, closest_vertex, p2 - du, p2, direction, index);

							return;
						}

						auto distance = glm::dot(p2, direction) + glm::dot(p1, direction);

						if (distance > closest_distance)
						{
							closest_distance = distance;
							closest_vertex = p;
						}

						if (index >= vertices_.size())
							return;

						p1 = vertices_[index];
						p2 = vertices_[0];

						if (++index < vertices_.size())
							p2 = vertices_[index];

						find_closest_distance(closest_distance, closest_vertex, p1, p2, direction, index);
					}

				public:
					Smooth(Component::Transform& transform, std::vector<glm::vec2> vertices)
						: Component::Collider::Polygon(transform, vertices), shortest_magnitude_(FLT_MAX), shortest_distance_(FLT_MAX)
					{
						auto i = 0;
						while (i < vertices_.size())
						{
							auto p1 = vertices_[i];
							auto p2 = vertices_[0];
							if (++i < vertices_.size())
								p2 = vertices_[i];

							auto line = p1 - p2;
							auto temp_mag = line.x * line.x + line.y * line.y;
							if (shortest_magnitude_ > temp_mag)
							{
								shortest_magnitude_ = temp_mag;
								shortest_distance_ = glm::sqrt(shortest_magnitude_);
							}
						}
					}


					void resolve(Component::Col& col, Component::Movement& movement) override
					{
						auto closest_distance = -FLT_MAX;
						auto closest_vertex = glm::vec2();

						find_closest_distance(closest_distance, closest_vertex, vertices_[0], vertices_[1], col.get_center() - this->get_center(), 1);

						closest_vertex = glm::normalize(closest_vertex);

						col.transform.x += closest_vertex.y * movement.speed * Game::delta_time;
						col.transform.y += -closest_vertex.x * movement.speed * Game::delta_time;
					}
				};
			}
		}
	}
}
