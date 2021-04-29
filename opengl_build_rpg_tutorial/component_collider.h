#pragma once
#include <glm/vec2.hpp>
#include "component_transform.h"
#include <array>

/*
abstract class used to define collision objects in the game world.

@author David Wadsworth
*/

constexpr auto MAX_SIMPLEX = 3;

namespace Component
{
	class Col : public Comp
	{
	protected:
		std::array<glm::vec2, MAX_SIMPLEX> simplex_;
		virtual glm::vec2 get_center() = 0;
		virtual glm::vec2 support(glm::vec2 direction) = 0;
	public:
		Component::Transform& transform;

		Col(Component::Transform& transform)
			: transform(transform), simplex_{}
		{}
		bool collide(Component::Col& col);
	};
}



