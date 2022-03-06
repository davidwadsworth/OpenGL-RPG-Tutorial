#pragma once
#include "component_rect_gjk.h"
#include <glad/glad.h>
#include <glm/geometric.hpp>

/*
GJK implementation for regular circles

@author David Wadsworth
*/
namespace Component {
	namespace Rect {
		namespace GJK
		{
			template <typename T>
			class Circle : public Component::Rect::IGJK<T>
			{
				GLfloat radius_;
				glm::vec2 center_;
			public:
				Circle(const Rect& transform, GLfloat radius, glm::vec2 center)
					: IGJK<T>(transform), radius_(radius), center_(center)
				{}

				glm::vec2 get_center() override
				{
					return center_ + glm::vec2(this->x, this->y);
				}

				glm::vec2 support(glm::vec2 direction) override
				{
					return get_center() + radius_ * glm::normalize(direction);
				}
			};
#define CircleAction Circle<ActionGJK>
#define CirclePhysics Circle<PhysicsGJK>
		}
	}
}