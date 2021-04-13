#pragma once
#include <glm/vec2.hpp>
#include "component_transform.h"

namespace Component
{
	class Col : public Comp
	{
	public:
		virtual bool collide(Col& collider, Component::Transform& posA, Component::Transform& posB) = 0;
		virtual glm::vec2 get_center(Component::Transform& transform) = 0;
	};
}

