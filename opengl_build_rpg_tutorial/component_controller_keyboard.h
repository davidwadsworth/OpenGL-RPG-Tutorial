#pragma once
#include "component_controller.h"
#include <GLFW/glfw3.h>
#include "game.h"
#include "component_array.h"

namespace Component {
	namespace Controller {
		class Keyboard : public Component::IController
		{
			enum KeyID
			{
				left = GLFW_KEY_LEFT,
				right = GLFW_KEY_RIGHT,
				down = GLFW_KEY_DOWN,
				up = GLFW_KEY_UP,
				action_1 = GLFW_KEY_Z,
			};
			Component::KeyboardArray* keys_;
		public:
			Keyboard()
			{
				keys_ = Game::global->get_child("keyboard")->get_component<Component::KeyboardArray>();
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
			bool key_down_action_1() override
			{
				return (*keys_)[action_1];
			}
		};
	}
}
