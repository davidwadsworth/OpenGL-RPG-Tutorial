#pragma once
#include <glm/vec2.hpp>
#include "component_collider.h"

struct Phys
{
	virtual glm::vec2 perpendicular_bisector(Component::Col& col) = 0;
};

