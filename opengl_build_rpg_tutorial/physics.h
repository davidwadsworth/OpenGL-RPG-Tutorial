#pragma once
#include <glm/vec2.hpp>
#include "component_collider.h"

struct Phys
{
	virtual glm::vec2 perpendicular_bisector(Component::Col& col, Component::Transform& pos_a, Component::Transform& pos_b) = 0;
};

