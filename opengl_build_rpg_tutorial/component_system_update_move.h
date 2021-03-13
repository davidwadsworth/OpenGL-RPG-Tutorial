#pragma once
#include "component_controller.h"
#include "component_system.h"
#include "component_transform.h"

/*
Updates transform position on the game world

@author David Wadsworth
*/

namespace ComponentSystemUpdate
{
	constexpr auto ONE_OVER_SQRT_TWO = 0.70710678118f;
	constexpr GLfloat SPEED = 4.0f;

	class Move : public Component::System
	{
		Component::Transform& transform_;
		Component::Controller& controller_;
	public:

		Move(Component::Transform& transform, Component::Controller& controller)
			: transform_(transform), controller_(controller)
		{}

		void execute() override
		{
			GLfloat x = 0.0f, y = 0.0f;

			if (controller_.key_down_right()) x++;
			if (controller_.key_down_left()) x--;
			if (controller_.key_down_down()) y++;
			if (controller_.key_down_up()) y--;

			if (!y && !x)
			{
				x *= ONE_OVER_SQRT_TWO;
				y *= ONE_OVER_SQRT_TWO;
			}

			transform_.x += x * SPEED;
			transform_.y += y * SPEED;
		}
	};
}