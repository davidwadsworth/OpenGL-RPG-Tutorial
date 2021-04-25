#pragma once
#include <glm/vec2.hpp>
#include "component_collider.h"
#include "component_movement.h"

struct Phys
{
	virtual void resolve(Component::Col& col, Component::Movement& movement) = 0;
};

