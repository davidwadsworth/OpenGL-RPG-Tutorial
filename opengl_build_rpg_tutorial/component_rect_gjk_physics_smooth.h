#pragma once
#include "component_rect_gjk_physics.h"
#include "component_rect_gjk_boundary.h"
#include "component_rect_gjk_circle.h"
#include "component_rect_gjk_polygon.h"

/*
Physics gjk implementation for smoothly gliding over an object

@author David Wadsworth
*/
namespace Component {
	namespace Rectangle {
		namespace GJK {
			namespace Physics {
				namespace Boundary
				{
					template <typename T>
					class Smooth : public Component::Rectangle::GJK::Boundary<T>
					{
					public:
						using Component::Rectangle::GJK::Boundary<T>::Boundary;

						void resolve(Component::Rectangle::IGJK& col) override
						{
							auto line = glm::normalize(this->vertices_[1] - this->vertices_[0]);
							auto perp_bisector = glm::vec2(line.y, -line.x);

							auto sup = col.support(-perp_bisector) - this->support(perp_bisector);
							auto piercing_vec = glm::dot(sup, -perp_bisector) * perp_bisector;

							col.x += piercing_vec.x;
							col.y += piercing_vec.y;
						}
					};
				}
				namespace Circle
				{
					template <typename T>
					class Smooth : public Component::Rectangle::GJK::Circle<T>
					{
					public:
						using Component::Rectangle::GJK::Circle<T>::Circle;

						void resolve(Component::Rectangle::IGJK& col) override
						{
							auto perp_bisector = glm::normalize(col.get_center() - this->get_center());

							auto sup = col.support(-perp_bisector) - this->support(perp_bisector);
							auto piercing_vec = glm::dot(sup, -perp_bisector) * perp_bisector;

							col.x += piercing_vec.x;
							col.y += piercing_vec.y;
						}
					};
				}
				namespace Polygon
				{
					template <typename T>
					class Smooth : public Component::Rectangle::GJK::Polygon<T>
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
									find_closest_distance(closest_distance, closest_vertex, p1, p1 + du * i, direction, this->vertices_.size());
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

							if (index >= this->vertices_.size())
								return;

							p1 = this->vertices_[index];
							p2 = this->vertices_[0];

							if (++index < this->vertices_.size())
								p2 = this->vertices_[index];

							find_closest_distance(closest_distance, closest_vertex, p1, p2, direction, index);
						}

					public:
						Smooth(const Rect& transform, std::vector<glm::vec2> vertices)
							: Component::Rectangle::GJK::Polygon<T>(transform, vertices), shortest_magnitude_(FLT_MAX), shortest_distance_(FLT_MAX)
						{
							// calculate shortest magnitute and distance of all possible sides
							auto i = 0;
							while (i < this->vertices_.size())
							{
								auto p1 = this->vertices_[i];
								auto p2 = this->vertices_[0];
								if (++i < this->vertices_.size())
									p2 = this->vertices_[i];

								auto line = p1 - p2;
								auto temp_mag = line.x * line.x + line.y * line.y;
								if (shortest_magnitude_ > temp_mag)
								{
									shortest_magnitude_ = temp_mag;
									shortest_distance_ = glm::sqrt(shortest_magnitude_);
								}
							}
						}

						void resolve(Component::Rectangle::IGJK& col) override
						{
							auto closest_distance = -FLT_MAX;
							auto perp_bisector = glm::vec2();

							find_closest_distance(closest_distance, perp_bisector, this->vertices_[0], this->vertices_[1], glm::normalize(col.get_center() - this->get_center()), 1);

							// find perpendicular line of the closest side
							perp_bisector = glm::normalize(glm::vec2(perp_bisector.y, -perp_bisector.x));

							auto sup = col.support(-perp_bisector) - this->support(perp_bisector);
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
#define SmoothPhysics Smooth<Component::Rectangle::GJK::IPhysics<Component::Rectangle::IGJK>>
#define SmoothPhysicsAction Smooth<Component::Rectangle::GJK::IPhysics<Component::Rectangle::GJK::Action>>