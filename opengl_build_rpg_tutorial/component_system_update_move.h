#pragma once
#include "component_controller.h"
#include "component_system.h"
#include "component_transform.h"
#include "component_movement.h"

/*
Updates transform position on the game world

@author David Wadsworth
*/


namespace Component {
	namespace System {
		namespace Update 
		{
			constexpr auto ONE_OVER_SQRT_TWO = 0.70710678118f;

			class Move : public Component::ISystem
			{
				Component::Transform& transform_;
				Component::IController& controller_;
				Component::Movement& movement_;
			public:

				Move(Component::Transform& transform, Component::IController& controller, Component::Movement& movement)
					: transform_(transform), controller_(controller), movement_(movement)
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

					transform_.x += x * movement_.speed * Game::delta_time;
					transform_.y += y * movement_.speed * Game::delta_time;
				}
			};
		}
	}
}