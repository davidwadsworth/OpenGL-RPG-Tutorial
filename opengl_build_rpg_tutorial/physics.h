#pragma once
#include <glm/vec2.hpp>
#include "component_collider.h"
#include "component_movement.h"

/*
collision resolution abstract class.

@author David Wadsworth
*/

struct IPhysics
{
	virtual void resolve(Component::ICollider& col, Component::Movement& movement) = 0;
};

