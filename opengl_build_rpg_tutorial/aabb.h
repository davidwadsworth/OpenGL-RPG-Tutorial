#pragma once
#include "component_rect.h"
namespace AABB
{
	bool collide(Component::Rectang& rect_a, Component::Rectang& rect_b);
	bool collide(glm::vec2 point, Component::Rectang& rect_b);
}