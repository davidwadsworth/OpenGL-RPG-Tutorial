#pragma once
#include "component_rect.h"

/*
abstract class used to define collision objects in the game world.

@author David Wadsworth
*/
namespace Component
{
	class ICollider : public Component::Rectangle
	{
	public:
		virtual glm::vec2 get_center() = 0;
	};
}



