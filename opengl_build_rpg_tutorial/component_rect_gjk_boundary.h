#pragma once
#include "component_rect_gjk_physics.h"
#include <glm/glm.hpp>

constexpr GLuint MAX_BOUNDARY = 2;

/*
GJK implementation for two pointed lines

@author David Wadsworth
*/
namespace Component {
	namespace Collider {
		namespace GJK
		{
			template<typename T>
			class Boundary : public Component::Rect::IGJK<T>
			{
				glm::vec2 offset_;
			protected:
				std::array<glm::vec2, MAX_BOUNDARY> vertices_;
			public:
				Boundary(const Rect& transform, std::array<glm::vec2, MAX_BOUNDARY> vertices)
					: Component::Collider::IGJK(transform), vertices_(vertices), offset_()
				{
					offset_ = (vertices_[0] + vertices_[1]) / 2.0f;
					for (auto& v : vertices_)
						v -= offset_;
				}

				glm::vec2 get_center() override
				{
					return glm::vec2(this->x, this->y) + offset_;
				}

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
			};
#define BoundaryAction Boundary<Component::Rect::GJKAction>
#define BoundaryPhysics Boundary<Component::Rect::GJKPhysics>
		}
	}
}