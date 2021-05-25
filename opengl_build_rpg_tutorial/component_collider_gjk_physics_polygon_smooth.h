#pragma once
#include "physics.h"
#include "component_collider_gjk_polygon.h"
#include "game.h"

/*
Smoothly traverse over a convex polygon using another GJK collider

@author David Wadsworth
*/
namespace Component {
	namespace Collider {
		namespace GJK {
			namespace Physics {
				namespace Polygon
				{
					class Smooth : public Component::Collider::GJK::Polygon, public IPhysics
					{
						GLfloat shortest_magnitude_, shortest_distance_;

						void find_closest_distance(float& closest_distance, glm::vec2& closest_vertex, glm::vec2 p1, glm::vec2 p2, glm::vec2 direction, int index)
						{
							auto p = p2 - p1;
							auto p_magnitude = p.x * p.x + p.y * p.y;

							// floor square root function 
							auto q_sqrt = [](auto num)
							{
								auto i = 1.0f;
								for (; i * i < num; ++i);
								return static_cast<GLuint>(i);
							};


							// if the length of a side isn't the shortest split it up into x sides with 
							// the length of the shortest side
							if (p_magnitude > shortest_magnitude_)
							{
								auto du = shortest_distance_ * glm::normalize(p);

								auto multiplier = q_sqrt(p_magnitude / shortest_magnitude_);

								for (auto i = 1.0f; i < multiplier; ++i)
									find_closest_distance(closest_distance, closest_vertex, p1, p1 + du * i, direction, vertices_.size());
								find_closest_distance(closest_distance, closest_vertex, p2 - du, p2, direction, index);

								return;
							}

							// check if line is closest 
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
							: Component::Collider::GJK::Polygon(transform, vertices), shortest_magnitude_(FLT_MAX), shortest_distance_(FLT_MAX)
						{
							// calculate shortest magnitute and distance of all possible sides
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

						void resolve(Component::ICollider& col) override
						{
							auto closest_distance = -FLT_MAX;
							auto perp_bisector = glm::vec2();

							find_closest_distance(closest_distance, perp_bisector, vertices_[0], vertices_[1], glm::normalize(col.get_center() - this->get_center()), 1);

							// find perpendicular line of the closest side
							perp_bisector = glm::normalize(glm::vec2(perp_bisector.y, -perp_bisector.x));

							auto gjk = static_cast<Component::Collider::IGJK*>(&col);

							auto sup = gjk->support(-perp_bisector) - this->support(perp_bisector);
							auto piercing_vec = glm::dot(sup, -perp_bisector) * perp_bisector;

							col.transform.x += piercing_vec.x;
							col.transform.y += piercing_vec.y;
						}
					};
				}
			}
		}
	}
}
