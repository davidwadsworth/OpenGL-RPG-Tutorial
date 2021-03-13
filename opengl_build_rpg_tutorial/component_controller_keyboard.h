#pragma once
#include "component_controller.h"
#include <GLFW/glfw3.h>
#include "game.h"

namespace ComponentController
{
	class Keyboard : public Component::Controller
	{
		enum KeyID
		{
			left = GLFW_KEY_LEFT,
			right = GLFW_KEY_RIGHT,
			down = GLFW_KEY_DOWN,
			up = GLFW_KEY_UP,
		};
	public:
		Keyboard() = default;

		bool key_down_left() override
		{
			return Game::keys[left];
		}
		bool key_down_right() override
		{
			return Game::keys[right];
		}
		bool key_down_down() override
		{
			return Game::keys[down];
		}
		bool key_down_up() override
		{
			return Game::keys[up];
		}
	};
}
