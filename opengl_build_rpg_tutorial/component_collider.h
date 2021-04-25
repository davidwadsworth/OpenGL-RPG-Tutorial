#pragma once
#include <glm/vec2.hpp>
#include "component_transform.h"

namespace Component
{
	class Col : public Comp
	{
	public:
		Component::Transform& transform;

		Col(Component::Transform& transform)
			: transform(transform)
		{}

		virtual bool collide(Col& collider) = 0;
		virtual glm::vec2 get_center() = 0;
	};
}

