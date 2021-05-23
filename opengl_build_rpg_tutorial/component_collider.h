#pragma once
#include <glm/vec2.hpp>
#include "component_transform.h"
#include <array>

/*
abstract class used to define collision objects in the game world.

@author David Wadsworth
*/
namespace Component
{
	class ICollider : public IComponent
	{
	public:
		Component::Transform& transform;
		
		ICollider(Component::Transform& transform)
			: transform(transform)
		{}

		virtual glm::vec2 get_center() = 0;
	};
}



