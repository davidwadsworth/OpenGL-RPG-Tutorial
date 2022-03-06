#pragma once
#include "component_rect_gjk_physics.h"
#include "component_collider_gjk_boundary.h"
#include "component_collider_gjk_circle.h"
#include "component_collider_gjk_polygon.h"

/*
Smoothly traverse over a boundary using another GJK collider

@author David Wadsworth
*/
namespace Component {
	namespace Collider {
			namespace Physics {
				namespace AABB
					{
						class Smooth : public Component::Rect::GJK::
						{
						public:
							using Component::Rect::IPhys::IPhys;

							/* eg.
													*col_center (displacement * {1,0})

										  e1
								 p1	_______________ p2
								   |               |
								   |               |
								   |               |
								e4 |               | e2
								   |               |
								   |               |
								 p4 ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾ p3
										   e3
							*/

							void resolve(Component::Rectangle& col) override
							{
								auto aabb_1 = static_cast<AABB*>(&col);
								auto aabb_2 = this;

								auto col_a_center = this->get_center() - col.get_center();

								// calculate point set up
								glm::vec2 p1{ aabb_2->x, aabb_2->y };
								glm::vec2 p2{ aabb_2->x + aabb_2->w, aabb_2->y };
								glm::vec2 p3{ aabb_2->x + aabb_2->w, aabb_2->y + aabb_2->h };
								glm::vec2 p4{ aabb_2->x, aabb_2->y + aabb_2->h };

								// calculate the distance between the center of the collder and the edge
								auto e1_distance = glm::dot(p2, col_a_center) + glm::dot(p1, col_a_center);
								auto e2_distance = glm::dot(p3, col_a_center) + glm::dot(p2, col_a_center);
								auto e3_distance = glm::dot(p4, col_a_center) + glm::dot(p3, col_a_center);
								auto e4_distance = glm::dot(p1, col_a_center) + glm::dot(p4, col_a_center);

								auto min_distance = e1_distance;
								auto bisector = glm::vec2(0.0f, -1.0f);
								auto displacement = aabb_1->y + aabb_1->h - aabb_2->y;

								if (e2_distance < min_distance)
								{
									min_distance = e2_distance;
									bisector = glm::vec2(1.0f, 0.0f);
									displacement = (aabb_1->x + aabb_1->w) - aabb_2->x;
								}
								if (e3_distance < min_distance)
								{
									min_distance = e3_distance;
									bisector = glm::vec2(0.0f, 1.0f);
									displacement = aabb_1->y - (aabb_2->y + aabb_2->h);
								}
								if (e4_distance < min_distance)
								{
									min_distance = e4_distance;
									bisector = glm::vec2(-1.0f, 0.0f);
									displacement = aabb_1->x - (aabb_2->x + aabb_2->w);
								}

								aabb_1->x += bisector.x * displacement;
								aabb_1->y += bisector.y * displacement;
							}
						};
					}
				namespace GJK {
					namespace Boundary
						{
							class Smooth : public Component::Collider::GJK::Boundary, public IPhysics
							{
							public:
								using Component::Collider::GJK::Boundary::Boundary;

								void resolve(Component::ICollider& col) override
								{
									auto line = glm::normalize(vertices_[1] - vertices_[0]);
									auto perp_bisector = glm::vec2(line.y, -line.x);

									auto gjk = static_cast<Component::Collider::IGJK*>(&col);

									auto sup = gjk->support(-perp_bisector) - this->support(perp_bisector);
									auto piercing_vec = glm::dot(sup, -perp_bisector) * perp_bisector;

									col.x += piercing_vec.x;
									col.y += piercing_vec.y;
								}
							};
						}
					namespace Circle
						{
							class Smooth : public Component::Collider::GJK::Circle, public IPhysics
							{
							public:
								using Component::Collider::GJK::Circle::Circle;

								void resolve(Component::ICollider& col) override
								{
									auto perp_bisector = glm::normalize(col.get_center() - this->get_center());

									auto gjk = static_cast<Component::Collider::IGJK*>(&col);

									auto sup = gjk->support(-perp_bisector) - this->support(perp_bisector);
									auto piercing_vec = glm::dot(sup, -perp_bisector) * perp_bisector;

									col.x += piercing_vec.x;
									col.y += piercing_vec.y;
								}
							};
						}
					namespace Polygon
						{
							class Smooth : public Component::Collider::GJK::Polygon, public IPhysics
							{
								GLfloat shortest_magnitude_, shortest_distance_;

								void find_closest_distance(float& closest_distance, glm::vec2& closest_vertex, glm::vec2 p1, glm::vec2 p2, glm::vec2 direction, std::size_t index)
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
								Smooth(const Rect& transform, std::vector<glm::vec2> vertices)
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

									col.x += piercing_vec.x;
									col.y += piercing_vec.y;
								}
							};
						}
				}
		}
	}
}