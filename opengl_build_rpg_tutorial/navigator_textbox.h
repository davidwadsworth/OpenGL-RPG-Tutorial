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

		NPATH navigate() override
		{
			if (controller_.key_press_action_1())
				return NPATH::CHILD_1;

			return NPATH::STAY;
		}
	};
}

