#pragma once
#include "navigator.h"
#include "component_controller.h"
#include "game.h"

namespace Navigator
{
	class Textbox : public INavigator
	{
		Component::IController& controller_;
	public:

		Textbox()
			: controller_(*Game::global->get_component<Component::IController>("controller"))
		{}

		int navigate() override
		{
			if (controller_.key_press_action_1())
				return 1;

			if (controller_.key_press_action_2())
				return 2;

			return 0;
		}
	};
}

