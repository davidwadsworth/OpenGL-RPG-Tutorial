#pragma once
#include "component_system.h"
#include "component_system_update_animation.h"
#include "component_controller.h"
#include <glm/vec2.hpp>

/*
Animates object to play walk and idle animations on controller move events

@author David Wadsworth
*/

namespace Component{
	namespace System {
		namespace Update {
			class AnimateMove : public Component::Sys
			{
				Component::Cont& controller_;
				Component::System::Update::Animation& animation_;
				glm::ivec2 prev_;
			public:
				AnimateMove(Component::Cont& controller, Component::System::Update::Animation& animation)
					: controller_(controller), animation_(animation), prev_{ 0, 1 }
				{}

				void execute() override
				{
					// get the current state of movement for our character
					auto curr = glm::ivec2{ (int)controller_.key_down_right() - (int)controller_.key_down_left(),
					(int)controller_.key_down_down() - (int)controller_.key_down_up() };

					// if curr.x and curr.y is not 0 then its moving, else its idling
					if (curr.x || curr.y)
					{
						prev_ = curr;
						if (curr.x > 0)
							animation_.play("walk right");
						else if (curr.x < 0)
							animation_.play("walk left");
						if (curr.y > 0)
							animation_.play("walk down");
						else if (curr.y < 0)
							animation_.play("walk up");
					}
					else
					{
						if (prev_.x > 0)
							animation_.play("idle right");
						else if (prev_.x < 0)
							animation_.play("idle left");
						if (prev_.y > 0)
							animation_.play("idle down");
						else if (prev_.y < 0)
							animation_.play("idle up");
					}
				}
			};
		}
	}
}

