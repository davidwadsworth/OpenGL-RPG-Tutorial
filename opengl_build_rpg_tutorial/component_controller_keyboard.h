#pragma once
#include "component_controller.h"
#include <GLFW/glfw3.h>
#include "game.h"
#include "component_array.h"

/*
Keyboard implementation for controller events

keydown(): returns if the corrisponding key is in the down position
keyup(): returns if the corrisponding key is in the up position

keypress(): returns if the corrsisponding key switched from the down position to the up position
keyrelease(): returns if the corrisponding key switched from the up position to the down position

@author David Wadsworth
*/

namespace Component {
	namespace Controller {
		class Keyboard : public Component::IController
		{
			bool action_1_pressed_, action_2_pressed_;
			enum KeyID
			{
				left = GLFW_KEY_LEFT,
				right = GLFW_KEY_RIGHT,
				down = GLFW_KEY_DOWN,
				up = GLFW_KEY_UP,
				action_1 = GLFW_KEY_Z,
				action_2 = GLFW_KEY_X
			};
			Component::KeyboardArray* keys_;
		public:
			Keyboard()
				: action_1_pressed_(false), action_2_pressed_(false), keys_(nullptr)
			{
				keys_ = Game::global->get_component<Component::KeyboardArray>("keyboard");
			}

			bool key_down_left() override
			{
				return (*keys_)[left];
			}
			bool key_down_right() override
			{
				return (*keys_)[right];
			}
			bool key_down_down() override
			{
				return (*keys_)[down];
			}
			bool key_down_up() override
			{
				return (*keys_)[up];
			}

			bool key_press_action_1() override
			{
				if ((*keys_)[action_1])
				{
					if (!action_1_pressed_)
						action_1_pressed_ = true;
					else
						return false;
				}
				else
					action_1_pressed_ = false;

				return action_1_pressed_;
			}

			bool key_press_action_2() override
			{
				if ((*keys_)[action_2])
				{
					if (!action_2_pressed_)
						action_2_pressed_ = true;
					else
						return false;
				}
				else
					action_2_pressed_ = false;

				return action_2_pressed_;
			}
		};
	}
}
